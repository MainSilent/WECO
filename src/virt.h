#ifndef VIRT_H
#define VIRT_H
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QProcess>
#include <QTextStream>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

class Virt
{
public:
    Virt(QString *user, virConnectPtr *conn, virDomainPtr *domain);
    static void handleError(void *, virErrorPtr);
    static virDomainPtr getDomain(virConnectPtr, QString);

private:
    QString username;
};

#endif // VIRT_H
