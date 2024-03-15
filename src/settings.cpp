#include "settings.h"

Settings::Settings(QString *user, virConnectPtr *_conn, virDomainPtr *_domain, QObject *parent) : QObject(parent)
{
    username = *user;
    conn = *_conn;
    domain = *_domain;

    // udev USB devices
    QThread *usbThread = QThread::create([&]{
        struct udev* udev = udev_new();
        enumerate_devices(udev);
        monitor_devices(udev);
        udev_unref(udev);
    }); usbThread->start();
}

int Settings::getRAM()
{
    QXmlStreamReader reader(getXML());
    while(!reader.atEnd() && !reader.hasError()) {
        if(reader.readNext() == QXmlStreamReader::StartElement && reader.name() == "memory") {
            return reader.readElementText().toInt();
        }
    }

    return 1024;
}

void Settings::setRAM(QString value)
{
    QFile file("/home/"+username+"/.config/ECO/WindowsECO.xml");
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    QDomDocument doc("config");
    doc.setContent(&file);

    QDomElement root = doc.documentElement();
    QDomElement nodeTag = root.firstChildElement("memory");

    QDomElement newNodeTag = doc.createElement(QString("memory"));
    newNodeTag.setAttribute("unit", "MiB");
    QDomText newNodeText = doc.createTextNode(value);
    newNodeTag.appendChild(newNodeText);

    root.replaceChild(newNodeTag, nodeTag);

    file.seek(0);
    file.write(doc.toByteArray());
    file.close();

    virDomainSetMaxMemory(domain, value.toInt() * 1024);
    virDomainSetMemoryFlags(domain, value.toInt() * 1024, VIR_DOMAIN_MEM_CONFIG);
}

int Settings::getCPU()
{
    QXmlStreamReader reader(getXML());
    while(!reader.atEnd() && !reader.hasError()) {
        if(reader.readNext() == QXmlStreamReader::StartElement && reader.name() == "vcpu") {
            return reader.readElementText().toInt();
        }
    }

    return 1;
}

void Settings::setCPU(QString value)
{
    QFile file("/home/"+username+"/.config/ECO/WindowsECO.xml");
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    QDomDocument doc("config");
    doc.setContent(&file);

    QDomElement root = doc.documentElement();
    QDomElement nodeTag = root.firstChildElement("vcpu");

    QDomElement newNodeTag = doc.createElement(QString("vcpu"));
    QDomText newNodeText = doc.createTextNode(value);
    newNodeTag.appendChild(newNodeText);

    root.replaceChild(newNodeTag, nodeTag);

    file.seek(0);
    file.write(doc.toByteArray());
    file.close();

    virDomainSetVcpusFlags(domain, value.toInt(), VIR_DOMAIN_AFFECT_CONFIG | VIR_DOMAIN_VCPU_MAXIMUM);
    virDomainSetVcpusFlags(domain, value.toInt(), VIR_DOMAIN_AFFECT_CURRENT);
}

void Settings::openXML()
{
    QProcess::startDetached("xdg-open /home/"+username+"/.config/ECO/WindowsECO.xml");
}

void Settings::setDevice(QString vendor, QString product)
{
    if (!check_connection(vendor, product)) {
        virDomainAttachDevice(domain, make_device(vendor, product).toLocal8Bit());
    }
    else {
        virDomainDetachDevice(domain, make_device(vendor, product).toLocal8Bit());
    }
}

void Settings::deleteDisk(QString path)
{
    QString xmlConfig;
    QTextStream stream(&xmlConfig);
    QJsonArray disks;
    QDomDocument doc("config");
    doc.setContent(QString(virDomainGetXMLDesc(domain, 0)));
    QDomElement root = doc.documentElement().firstChildElement("devices");
    QDomElement device = root.firstChildElement("disk");
    for (; !device.isNull(); device = device.nextSiblingElement("disk")) {
        if (device.firstChildElement("source").attribute("file") == path) {
            device.save(stream, 0);
            virDomainDetachDeviceFlags(domain, xmlConfig.toLocal8Bit(), VIR_DOMAIN_AFFECT_CONFIG);
            break;
        }
    }
}

void Settings::addDisk(QString path)
{
    QString config = QString("<disk><source file='%1'/><target bus='ide'/></disk>").arg(path);
    virDomainDetachDeviceFlags(domain, config.toLocal8Bit(), VIR_DOMAIN_AFFECT_CONFIG);
}

QString Settings::getXML() {
    QFile config("/home/"+username+"/.config/ECO/WindowsECO.xml");
    config.open(QIODevice::ReadOnly | QIODevice::Text);

    QString config_data = config.readAll();
    config.close();

    return config_data;
}

QJsonArray Settings::getUsbDevices() const
{
    return usbDevices;
}

QJsonArray Settings::getDiskDevices() const
{
    QJsonArray disks;
    QDomDocument doc("config");
    doc.setContent(QString(virDomainGetXMLDesc(domain, 0)));
    QDomElement root = doc.documentElement().firstChildElement("devices");
    QDomElement device = root.firstChildElement("disk");

    for (; !device.isNull(); device = device.nextSiblingElement("disk")) {
        disks.append(device.firstChildElement("source").attribute("file"));
    }
    return disks;
}

QString Settings::make_device(QString vendor, QString product)
{
    return QString("<hostdev mode='subsystem' type='usb' managed='yes'><source><vendor id='0x%1'/><product id='0x%2'/></source></hostdev>")
           .arg(vendor).arg(product);
}

bool Settings::check_connection(QString vendor, QString product)
{
    QDomDocument doc("config");
    doc.setContent(QString(virDomainGetXMLDesc(domain, 0)));
    QDomElement root = doc.documentElement().firstChildElement("devices");
    QDomElement device = root.firstChildElement("hostdev");

    for (; !device.isNull(); device = device.nextSiblingElement("hostdev")) {
        if (device.firstChildElement("source").firstChildElement("vendor").attribute("id").contains(vendor) &&
            device.firstChildElement("source").firstChildElement("product").attribute("id").contains(product))
            return true;
    }

    return false;
}

void Settings::monitor_devices(udev *udev)
{
    struct udev_monitor* mon = udev_monitor_new_from_netlink(udev, "udev");

    udev_monitor_filter_add_match_subsystem_devtype(mon, SUBSYSTEM, NULL);
    udev_monitor_enable_receiving(mon);

    int fd = udev_monitor_get_fd(mon);

    while (1) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        int ret = select(fd+1, &fds, NULL, NULL, NULL);
        if (ret <= 0)
            break;

        if (FD_ISSET(fd, &fds)) {
            struct udev_device* dev = udev_monitor_receive_device(mon);
            check_device(dev);
        }
    }
}

void Settings::enumerate_devices(udev *udev)
{
    struct udev_enumerate* enumerate = udev_enumerate_new(udev);

    udev_enumerate_add_match_subsystem(enumerate, SUBSYSTEM);
    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry* entry;

    udev_list_entry_foreach(entry, devices) {
        const char* path = udev_list_entry_get_name(entry);
        struct udev_device* dev = udev_device_new_from_syspath(udev, path);
        check_device(dev);
    }

    udev_enumerate_unref(enumerate);
}

void Settings::check_device(udev_device *dev)
{
    if (dev) {
        if (udev_device_get_devnode(dev))
            add_device(dev);

        udev_device_unref(dev);
    }
}

void Settings::add_device(udev_device *dev)
{
    QString action = QString(udev_device_get_action(dev));
    if (action == "")
        action = "exists";

    const char* vendor = udev_device_get_sysattr_value(dev, "idVendor");
    if (!vendor)
        vendor = "0000";

    const char* product = udev_device_get_sysattr_value(dev, "idProduct");
    if (!product)
        product = "0000";

    QString device_number = udev_device_get_property_value(dev, "DEVNUM");
    QString path = udev_device_get_property_value(dev, "DEVPATH");
    QString name1 = udev_device_get_property_value(dev, "ID_VENDOR_FROM_DATABASE");
    QString name2 = QString(udev_device_get_property_value(dev, "ID_MODEL")).replace('_', ' ');
    QString name = name1 + " " + name2;

    if (device_number != "001") {
        if (action == "exists" || action == "add") {
            QJsonObject newUSB = QJsonObject({
                {"name", name},
                {"path", path},
                {"vendor", vendor},
                {"product", product},
                {"connected", check_connection(vendor, product)}
            });
            usbDevices.append(newUSB);
            emit usbDevicesChanged();
        }
        else if (action == "remove") {
            for (int i = 0; i < usbDevices.size(); i++) {
                if (usbDevices[i].toObject().value("path").toString() == path) {
                    usbDevices.removeAt(i);
                    break;
                }
            }
            emit usbDevicesChanged();
        }
    }
}

void Settings::setUsbDevices(const QJsonArray &newUsbDevices)
{
    usbDevices = newUsbDevices;
    emit usbDevicesChanged();
}

void Settings::setDiskDevices(const QJsonArray &newDiskDevices)
{
    diskDevices = newDiskDevices;
    emit diskDevicesChanged();
}
