#ifndef BALLE_H
#define BALLE_H

#include <QGraphicsEllipseItem>
#include <QObject>
#include <QBrush>
#include<QPen>
class Balle : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)


public:
    Balle(qreal x, qreal y, qreal width, qreal height);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

private:
    qreal m_width;
    qreal m_height;
    QBrush brush;

};

#endif // BALLE_H
