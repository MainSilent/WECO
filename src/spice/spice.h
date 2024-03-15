#include "spice-widget.h"
#include "spice-view.h"
#include "src/server.h"
#include <QObject>
#define QT_NO_KEYWORDS

class Spice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ getConnected WRITE setConnected NOTIFY connectedChanged)

public:
    Spice(const QString path, Server *server);
    static Spice *getSpice();
    Q_INVOKABLE void toggleDisplay(int id);
    Q_INVOKABLE void connectToGuest();
    quint32 getKeyboardLockModifiers();
    QVector<SpiceView *> displays;
    QString path;
    bool getConnected() const;

public Q_SLOTS:
    void setConnected(const bool);

Q_SIGNALS:
    void connectedChanged();

private:
    static Spice *instance;
    SpiceSession *session;
    Server *server;
    bool connected = false;
};
