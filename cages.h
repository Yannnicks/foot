#ifndef cages_H
#define cages_H

#include <QGraphicsItem>
#include <QPainter>

class Cage : public QGraphicsItem
{
public:
    Cage(qreal x, qreal y, qreal width, qreal height);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    qreal m_width;
    qreal m_height;
};

#endif // CAGE_H
