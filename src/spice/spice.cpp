#include "spice.h"

static void main_channel_event(SpiceChannel *channel, SpiceChannelEvent event, gpointer data)
{
    switch (event) {
        case SPICE_CHANNEL_OPENED:
            Spice::getSpice()->setConnected(true);
            qDebug() << "main channel: connected";
            break;
        case SPICE_CHANNEL_CLOSED:
            Spice::getSpice()->setConnected(false);
            qDebug() << "main channel: connection lost";
            Spice::getSpice()->connectToGuest();
            break;
        case SPICE_CHANNEL_ERROR_CONNECT:
            qDebug() << "main channel: failed to connect";
            break;
    }
}

static void port_event(SpicePortChannel *channel, gint event, gpointer server)
{
    Server *s = static_cast<Server*>(server);

    if (event == SPICE_PORT_EVENT_OPENED)
        s->setConnected(true);
    else if (event == SPICE_PORT_EVENT_CLOSED)
        s->setConnected(false);
}

static void port_data(SpicePortChannel *port, gpointer data, int size, gpointer server)
{
    qDebug() << "daaaaa";
}

static void channel_new(SpiceSession *session, SpiceChannel *channel, gpointer server)
{
    int id;
    g_object_get(channel, "channel-id", &id, NULL);

    if (SPICE_IS_MAIN_CHANNEL(channel))
        g_signal_connect(channel, "channel-event", G_CALLBACK(main_channel_event), 0);

    if (SPICE_IS_DISPLAY_CHANNEL(channel)) {
        qDebug() << QString("display %1 created").arg(id+1);
        SpiceView *view = new SpiceView();
        view->id = id;
        Spice::getSpice()->displays.append(view);

        Spice::getSpice()->displays[view->id]->display = spice_display_new(session, view->id);
        if (id == 0)
            Spice::getSpice()->displays[view->id]->setWindowTitle("Main Display");
    }

    if (SPICE_IS_PORT_CHANNEL(channel)) {
        g_signal_connect(channel, "port-event",
                         G_CALLBACK(port_event), server);
        g_signal_connect(channel, "port-data",
                         G_CALLBACK(port_data), server);
        spice_channel_connect(channel);
    }
}

Spice *Spice::instance = NULL;

Spice::Spice(const QString _path, Server *_server)
{
    path = _path;
    server = _server;
    session = NULL;
    instance = this;
}

Spice *Spice::getSpice()
{
    if (instance)
        return instance;
    instance = new Spice("", NULL);
    return instance;
}

void Spice::toggleDisplay(int id)
{
    if (displays.length() == 0) {
        connectToGuest();
        return;
    }

    --id;
    if (Spice::getSpice()->displays[id]->isVisible())
        Spice::getSpice()->displays[id]->hide();
    else
        Spice::getSpice()->displays[id]->show();
}

void Spice::connectToGuest()
{
    if (connected) return;

    if (displays.length() != 0) {
        foreach (SpiceView *v, displays) {
            v->hide();
        }
        displays[0]->inputs = NULL;
    }
    displays = {};

    session = spice_session_new();
    g_object_set(session, "unix-path", \
                 path.toLatin1().constData(), NULL);
    g_signal_connect(session, "channel-new",
                                 G_CALLBACK(channel_new), server);
    spice_session_connect(session);
}

quint32 Spice::getKeyboardLockModifiers()
{
    XKeyboardState keyboard_state;
    Display *x_display = QX11Info::display();
    quint32 modifiers = 0;

    XGetKeyboardControl(x_display, &keyboard_state);

    if (keyboard_state.led_mask & 0x01)
        modifiers |= SPICE_INPUTS_CAPS_LOCK;
    if (keyboard_state.led_mask & 0x02)
        modifiers |= SPICE_INPUTS_NUM_LOCK;
    if (keyboard_state.led_mask & 0x04)
        modifiers |= SPICE_INPUTS_SCROLL_LOCK;

    return modifiers;
}

bool Spice::getConnected() const
{
    return connected;
}

void Spice::setConnected(const bool newStatus)
{
    connected = newStatus;
    emit connectedChanged();
}
