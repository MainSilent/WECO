#include <QCoreApplication>
#include <QTcpSocket>
#include <QObject>
#include <QDebug>

int main(int argc, char *argv[])
{
    if (argc <= 2) return 1;

    QCoreApplication a(argc, argv);

    QTcpSocket *socket = new QTcpSocket();

    QObject::connect(socket, &QTcpSocket::connected, [&]() {
        QString sendData;
        if (QString(argv[1]) == "p") {
            QString path = QString(argv[2]).replace("\\", "\\\\");
            sendData = "{\"op\":2, \"t\":\"p\", \"path\":\""+path+"\"}";
            socket->write(sendData.toUtf8());
        }
        else if (QString(argv[1]) == "s") {
            sendData = "{\"op\":2, \"t\":\"s\", \"appId\":\""+QString(argv[2])+"\", \"pfn\":\""+QString(argv[3])+"\"}";
            socket->write(sendData.toUtf8());
        }
        socket->waitForBytesWritten();
    });

    socket->connectToHost("127.0.0.1", 40110);
    socket->waitForConnected();

    return 0;
}
