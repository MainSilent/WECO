#ifndef SPICEVIEW_H
#define SPICEVIEW_H

#include "spice-util.h"
#include "spice-widget.h"

#include <QWidget>
#include <QCursor>
#include <QtCore>
#include <QtGui>
#include <QWidget>
#include <QDebug>
#include <QX11Info>
#include <X11/XKBlib.h>
#undef Bool

class SpiceView : public QWidget
{
    Q_OBJECT
public:
    explicit SpiceView(QWidget *parent = nullptr);
    void updateImage(uchar *data, int x, int y, int width, int height);
    void settingsChanged(int width, int height, int bpp);
    void spiceResize(int w, int h);
    void showCursor(bool visible)
    {
        //if (visible)
            setCursor(Qt::ArrowCursor);
        //else
        //    setCursor(Qt::BlankCursor);
    }
    int id;
    SpiceDisplay *display;
    static SpiceInputsChannel *inputs;
protected:
    void resizeEvent(QResizeEvent* event);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
private:
    static QMap<int, int> *keymap;
    static QMap<int, int> *getKeymap();
    QImage         img;
    int            dataWidth;
    int            dataHeight;
    double         rate;
    bool           initSize;
};

#endif // SPICEVIEW_H
