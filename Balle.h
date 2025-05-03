#ifndef BALLE_H
#define BALLE_H

#include <QGraphicsEllipseItem>
#include <QObject>

class Balle : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    Balle(qreal x, qreal y, qreal width, qreal height);
    void verifieCollision();
};
#endif // BALLE_H
