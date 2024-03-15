#include "spice.h"

G_DEFINE_TYPE(SpiceDisplay, spice_display, SPICE_TYPE_CHANNEL);

static void disconnect_main(SpiceDisplay *display);
static void disconnect_display(SpiceDisplay *display);
static void channel_new(SpiceSession *s, SpiceChannel *channel, gpointer data);
static void sync_keyboard_lock_modifiers(SpiceDisplay *display, guint32 modifiers);

static void spice_display_class_init(SpiceDisplayClass *klass)
{
    g_type_class_add_private(klass, sizeof(SpiceDisplayPrivate));
}

static void spice_display_init(SpiceDisplay *display)
{
    SpiceDisplayPrivate *d;

    d = display->priv = SPICE_DISPLAY_GET_PRIVATE(display);
    memset(d, 0, sizeof(*d));
}

static void update_mouse_mode(SpiceChannel *channel, gpointer data)
{
    SpiceDisplay *display = static_cast<SpiceDisplay*>(data);
    SpiceDisplayPrivate *d = SPICE_DISPLAY_GET_PRIVATE(display);
    if (!d)
        return;
    g_object_get(channel, "mouse-mode", &d->mouse_mode, NULL);
}

void send_key(SpiceDisplay *display, int scancode, int down)
{
    SpiceDisplayPrivate *d = SPICE_DISPLAY_GET_PRIVATE(display);
    uint32_t i, b, m;

    if (!d || !SpiceView::inputs)
        return;

    i = scancode / 32;
    b = scancode % 32;
    m = (1 << b);
    g_return_if_fail(i < SPICE_N_ELEMENTS(d->key_state));

    if (down) {
        spice_inputs_key_press(SpiceView::inputs, scancode);
        d->key_state[i] |= m;
    } else {
        if (!(d->key_state[i] & m)) {
            return;
        }
        spice_inputs_key_release(SpiceView::inputs, scancode);
        d->key_state[i] &= ~m;
    }
}

static void primary_create(SpiceChannel *channel, gint format, gint width, gint height, gint stride, gint shmid, gpointer imgdata, gpointer data) {

    SpiceDisplay *display = static_cast<SpiceDisplay*>(data);
    SpiceDisplayPrivate *d = SPICE_DISPLAY_GET_PRIVATE(display);

    d->format = static_cast<SpiceSurfaceFmt>(format);
    d->stride = stride;
    d->shmid = shmid;
    d->width = width;
    d->height = height;
    d->data_origin = d->data = imgdata;
    Spice::getSpice()->displays[d->channel_id]->settingsChanged(width, height, 4);
}

static void invalidate(SpiceChannel *channel, gint x, gint y, gint w, gint h, gpointer data) {
    SpiceDisplay *display = static_cast<SpiceDisplay*>(data);
    SpiceDisplayPrivate *d = SPICE_DISPLAY_GET_PRIVATE(display);
    if (!d)
        return;
    if (x + w > d->width || y + h > d->height) {
        ;
    } else {
        uchar *img = static_cast<uchar*>(d->data);
        Spice::getSpice()->displays[d->channel_id]->updateImage(img, x, y, w, h);
    }
}

static void mark(SpiceChannel *channel, gint mark, gpointer data) {
    SpiceDisplay *display = static_cast<SpiceDisplay*>(data);
    SpiceDisplayPrivate *d = SPICE_DISPLAY_GET_PRIVATE(display);
    if (!d)
        return;
    d->mark = mark;
    spice_main_set_display_enabled(d->main, d->channel_id, d->mark != 0);
}

static void cursor_reset(SpiceCursorChannel *channel, gpointer data)
{
    int *id = static_cast<int*>(data);
    Spice::getSpice()->displays[*id]->showCursor(true);
}

static void cursor_set(SpiceCursorChannel *channel, gint width, gint height, gint hot_x, gint hot_y, gpointer rgba, gpointer data)
{
    int *id = static_cast<int*>(data);
    Spice::getSpice()->displays[*id]->showCursor(true);
}

static void cursor_hide(SpiceCursorChannel *channel, gpointer data)
{
    int *id = static_cast<int*>(data);
    Spice::getSpice()->displays[*id]->showCursor(false);
}

static void channel_new(SpiceSession *session, SpiceChannel *channel, gpointer data)
{
    SpiceDisplay *display = static_cast<SpiceDisplay*>(data);
    SpiceDisplayPrivate *d = SPICE_DISPLAY_GET_PRIVATE(display);
    if (!d)
        return;

    int id;
    g_object_get(channel, "channel-id", &id, NULL);

    if (SPICE_IS_MAIN_CHANNEL(channel)) {
        d->main = SPICE_MAIN_CHANNEL(channel);
        g_signal_connect(channel, "main-mouse-update",
                                      G_CALLBACK(update_mouse_mode), display);
        update_mouse_mode(channel, display);
        return;
    }

    if (SPICE_IS_DISPLAY_CHANNEL(channel)) {
        if (id != d->channel_id)
            return;
        d->display = channel;
        g_signal_connect(channel, "display-primary-create",
                         G_CALLBACK(primary_create), display);
        g_signal_connect(channel, "display-invalidate",
                         G_CALLBACK(invalidate), display);
        g_signal_connect(channel, "display-mark",
                         G_CALLBACK(mark), display);
        spice_channel_connect(channel);
        return;
    }

    if (SPICE_IS_CURSOR_CHANNEL(channel)) {
        if (id != d->channel_id)
            return;
        d->cursor = SPICE_CURSOR_CHANNEL(channel);
        g_signal_connect(channel, "cursor-set",
                         G_CALLBACK(cursor_set), &d->channel_id);
        g_signal_connect(channel, "cursor-hide",
                         G_CALLBACK(cursor_hide), &d->channel_id);
        g_signal_connect(channel, "cursor-reset",
                         G_CALLBACK(cursor_reset), &d->channel_id);
        spice_channel_connect(channel);
        return;
    }

    if (SPICE_IS_INPUTS_CHANNEL(channel)) {
        if (SpiceView::inputs) return;
        SpiceView::inputs = SPICE_INPUTS_CHANNEL(channel);
        spice_channel_connect(channel);
        sync_keyboard_lock_modifiers(display, Spice::getSpice()->getKeyboardLockModifiers());
        return;
    }

    if (SPICE_IS_PLAYBACK_CHANNEL(channel)) {
        spice_audio_new(session, NULL, NULL);
    }

    return;
}

SpiceDisplay *spice_display_new(SpiceSession *session, int id)
{
    SpiceDisplay *display;
    SpiceDisplayPrivate *d;
    GList *list;
    GList *it;

    display = static_cast<SpiceDisplay*>(g_object_new(SPICE_TYPE_DISPLAY, NULL));
    d = SPICE_DISPLAY_GET_PRIVATE(display);
    d->session = static_cast<SpiceSession*>(g_object_ref(session));
    d->channel_id = id;

    g_signal_connect(session, "channel-new",
                     G_CALLBACK(channel_new), display);
    list = spice_session_get_channels(session);
    for (it = g_list_first(list); it != NULL; it = g_list_next(it)) {
        channel_new(session, static_cast<SpiceChannel*>(it->data), (gpointer*)display);
    }
    g_list_free(list);

    return display;
}

static void sync_keyboard_lock_modifiers(SpiceDisplay *display, guint32 modifiers)
{
    SpiceDisplayPrivate *d = SPICE_DISPLAY_GET_PRIVATE(display);
    if (!d)
        return;

    if (SpiceView::inputs)
        spice_inputs_set_key_locks(SpiceView::inputs, modifiers);
}
