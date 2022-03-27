#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QMouseEvent>

class Canvas : public QLabel
{
    Q_OBJECT
public:
    Canvas(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event) ;
    void mouseReleaseEvent(QMouseEvent *releaseEvent) ;

signals:
    void sendMouseLoc(QPoint&);


};

#endif // CANVAS_H