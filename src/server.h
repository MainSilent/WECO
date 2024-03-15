#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDirIterator>
#include <QProcess>
#include <QFile>
#include <QImage>

class Server : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ getConnected WRITE setConnected NOTIFY connectedChanged)

public:
    explicit Server(QString *user, QObject *parent = nullptr);
    Q_INVOKABLE QJsonArray programs();
    Q_INVOKABLE void execute(QString);
    bool getConnected() const;
    void setPrograms(QJsonDocument *doc);
    void makeDesktop(QJsonObject *obj, QString type);

public slots:
    void setConnected(const bool &connectionStatus);
    void newConnection();
    void read();

signals:
    void connectedChanged();

private:
    bool connected = false;
    QTcpServer *server;
    QTcpSocket *socket;
    QVector<QTcpSocket *> mClients;
    QByteArray rawData;
    QString *username;
};

#endif // SERVER_H
