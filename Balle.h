#ifndef BALLE_H
#define BALLE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsItem>

class Balle : public QGraphicsEllipseItem
{
public:
    Balle(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr);
};

#endif // BALLE_H
