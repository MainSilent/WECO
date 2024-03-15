#include "server.h"

Server::Server(QString *user, QObject *parent) : QObject(parent)
{
    username = user;
    server = new QTcpServer(this);

    connect(server, &QTcpServer::newConnection, this, &Server::newConnection);

    if(!server->listen(QHostAddress::Any, 40110))
        qDebug() << "Service server could not start";
    else
        qDebug() << "Service server started!";
}

QJsonArray Server::programs()
{
    QJsonArray list;
    QDirIterator applications("/home/"+*username+"/.local/share/ECO/applications");
    while (applications.hasNext()) {
        QFile file(applications.next());

        if (file.open(QIODevice::ReadOnly) && file.fileName().split(".").last() == "desktop") {
            QString name, exec;
            while (!file.atEnd()) {
                QString line = file.readLine();
                if (line.startsWith("Name"))
                    name = line.split("=").last().trimmed();
                else if (line.startsWith("Exec"))
                    exec = line.split("=").last().trimmed();
            }

            list.append(QJsonObject({
                {"name", name},
                {"exec", exec}
            }));
        }
    }

    return list;
}

void Server::execute(QString command)
{
    QProcess::startDetached(command);
}

bool Server::getConnected() const
{
    return connected;
}

void Server::setPrograms(QJsonDocument *doc)
{
    QJsonObject object = doc->object();
    QJsonObject::iterator i;
    for (i = object.begin(); i != object.end(); ++i) {
        if (i.key() == "data") {
            QJsonObject object = i->toObject();
            QJsonObject::iterator i;
            for (i = object.begin(); i != object.end(); ++i) {
                if (i.key() == "programs") {
                    QJsonObject object = i->toObject();
                    makeDesktop(&object, "program");
                }
                else if (i.key() == "storeApps") {
                    QJsonObject object = i->toObject();
                    makeDesktop(&object, "storeApp");
                }
            }
        }
    }
}

void Server::makeDesktop(QJsonObject *obj, QString type)
{
    QString xdgPath = "/home/"+*username+"/.local/share/ECO/";;
    QString name = (*obj)["name"].toString();
    QFile desktop(xdgPath+"applications/"+name+".desktop");
    desktop.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&desktop);
    stream << "[Desktop Entry]\n";
    stream << "Type=Application\n";
    stream << "Name="+name+"\n";
    stream << "Icon="+name+"\n";

    if (type == "program")
        stream << QString("Exec=ecopen p \"%1\"\n").arg((*obj)["path"].toString());

    else if (type == "storeApp")
        stream << QString("Exec=ecopen s \"%1\" \"%2\"\n").arg((*obj)["AppId"].toString()).arg((*obj)["PFN"].toString());

    desktop.close();

    // make the icon file
    QByteArray iconData = QByteArray::fromBase64((*obj)["icon"].toString().toUtf8());
    QImage icon = QImage::fromData(iconData);

    if(!icon.isNull())
        icon.save(xdgPath+"icons/"+name+".png", "PNG");
}

void Server::setConnected(const bool &connectionStatus)
{
    connected = connectionStatus;
    emit connectedChanged();
}

void Server::newConnection()
{
    socket = server->nextPendingConnection();
    //qDebug() << "New Connection " + socket->peerAddress().toString();
    socket->write("{\"op\":0, \"interval\":1000}");
    connect(socket, &QTcpSocket::readyRead, this, &Server::read);

    mClients.push_back(socket);
}

void Server::read()
{
    //rawData += socket->readAll();
    for(auto &client : mClients) {
        rawData += client->readAll();
    }

    QJsonParseError parseError;
    QJsonDocument data = QJsonDocument::fromJson(rawData, &parseError);

    if (parseError.error == QJsonParseError::NoError) {
        switch (data["op"].toInt()) {
            // code 1: Store Program
            case 0:
                setPrograms(&data);
                break;

            // code 2: Open Program
            case 1:
                for(auto &client : mClients) {
                    client->write(rawData);
                }
                break;

            // code 3: window
            case 2:
                qDebug() << data;
                break;
        }
        rawData = "";
    }
}
