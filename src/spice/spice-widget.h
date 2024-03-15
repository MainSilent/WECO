#ifndef SPICE_WIDGET_H
#define SPICE_WIDGET_H

#undef signals
#include <glib.h>
#include <glib-object.h>
#include "spice-client.h"

G_BEGIN_DECLS

#define SPICE_DISPLAY_GET_PRIVATE(obj)                                  \
    (G_TYPE_INSTANCE_GET_PRIVATE((obj), SPICE_TYPE_DISPLAY, SpiceDisplayPrivate))

#define SPICE_TYPE_DISPLAY            (spice_display_get_type())
#define SPICE_DISPLAY(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), SPICE_TYPE_DISPLAY, SpiceDisplay))
#define SPICE_DISPLAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), SPICE_TYPE_DISPLAY, SpiceDisplayClass))
#define SPICE_IS_DISPLAY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), SPICE_TYPE_DISPLAY))
#define SPICE_IS_DISPLAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), SPICE_TYPE_DISPLAY))
#define SPICE_DISPLAY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), SPICE_TYPE_DISPLAY, SpiceDisplayClass))

typedef struct _SpiceDisplay SpiceDisplay;
typedef struct _SpiceDisplayClass SpiceDisplayClass;
typedef struct _SpiceDisplayPrivate SpiceDisplayPrivate;

struct _SpiceDisplay {
    SpiceChannel parent;
    SpiceDisplayPrivate *priv;
    /* Do not add fields to this struct */
};

struct _SpiceDisplayClass {
    SpiceChannelClass parent_class;

    /* signals */
    void (*mouse_grab)(SpiceChannel *channel, gint grabbed);
    void (*keyboard_grab)(SpiceChannel *channel, gint grabbed);

    gchar _spice_reserved[SPICE_RESERVED_PADDING];
};

struct _SpiceDisplayPrivate {
    gint                    channel_id;
    gint                    width, height, stride;
    gint                    shmid;
    gpointer                data_origin;
    gpointer                data;
    gint                    mark;
    SpiceSession            *session;
    SpiceMainChannel        *main;
    SpiceChannel            *display;
    SpiceCursorChannel      *cursor;
    uint32_t                key_state[512 / 32];
    enum SpiceSurfaceFmt    format;
    enum SpiceMouseMode     mouse_mode;
};

typedef enum
{
    SPICE_DISPLAY_KEY_EVENT_PRESS = 1,
    SPICE_DISPLAY_KEY_EVENT_RELEASE = 2,
    SPICE_DISPLAY_KEY_EVENT_CLICK = 3,
} SpiceDisplayKeyEvent;

GType	        spice_display_get_type(void);

SpiceDisplay* spice_display_new(SpiceSession *session, int channel_id);

void spice_display_send_keys(SpiceDisplay *display, const guint *keyvals,
                             int nkeyvals, SpiceDisplayKeyEvent kind);

void send_key(SpiceDisplay *display, int scancode, int down);

G_END_DECLS

#endif /* __SPICE_CLIENT_WIDGET_H__ */
