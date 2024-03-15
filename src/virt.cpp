#include "virt.h"

Virt::Virt(QString *user, virConnectPtr *conn, virDomainPtr *domain)
{
    username = *user;
    virSetErrorFunc(NULL, handleError);
    virEventRegisterDefaultImpl();
    *conn = virConnectOpen("qemu:///session");
    *domain = virDomainLookupByName(*conn, "WindowsECO");
    //if (domain == NULL)
       //domain = getDomain(conn, username);

    QThread *eventsThread = QThread::create([]{
        while (true)
            virEventRunDefaultImpl();
    }); eventsThread->start();
}

void Virt::handleError(void *userData, virErrorPtr error)
{
    if (error->code == 38)
        QProcess::startDetached(QString("notify-send Error \"%1\"").arg(error->message));
    else
        qDebug() << error->message;
}

virDomainPtr Virt::getDomain(virConnectPtr conn, QString username)
{
    QFile xmlConfig(QString("/home/%1/.config/ECO/WindowsECO.xml").arg(username));
    xmlConfig.open(QIODevice::ReadOnly);
    QTextStream in(&xmlConfig);
    QByteArray array = in.readAll().toLocal8Bit();
    char *buffer = array.data();
    xmlConfig.close();

    return virDomainCreateXML(conn, buffer, 0);
}
