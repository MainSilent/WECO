#include "src/spice/spice.h"
#include "power.h"

Power::Power(QString *user, virConnectPtr _conn, virDomainPtr _domain, QObject *parent) : QObject(parent)
{
    conn = _conn;
    domain = _domain;
    username = *user;
    newPowerStatus(conn, domain, 0, 0, this);
    virConnectDomainEventRegisterAny(conn, domain, VIR_DOMAIN_EVENT_ID_LIFECYCLE, VIR_DOMAIN_EVENT_CALLBACK(newPowerStatus), this, Q_NULLPTR);
}

void Power::newPowerStatus(virConnectPtr conn, virDomainPtr domain, int event, int detail, void *opaque)
{
    int state;
    virDomainGetState(domain, &state, NULL, 0);
    Power *obj = static_cast<Power *>(opaque);

    switch (state) {
        case 1:
            obj->setStatus("running");
            //Spice::getSpice()->connectToGuest();
            break;
        case 3:
        case 7:
           obj->setStatus("paused");
           break;
        case 5:
           obj->setStatus("off");
           break;
        default:
            obj->setStatus("");
    }
}

void Power::action(QString type)
{
    if (type == "start" or type == "create") {
        if (virDomainCreate(domain) == -1)
            domain = Virt::getDomain(conn, username);
    }
    else if (type == "shutdown")
        virDomainShutdown(domain);
    else if (type == "reboot")
        virDomainReboot(domain, 0);
    else if (type == "suspend")
        virDomainSuspend(domain);
    else if (type == "resume")
        virDomainResume(domain);
}

QString Power::getStatus() const
{
    return status;
}

void Power::setStatus(const QString &newStatus)
{
    status = newStatus;
    emit statusChanged();
}

