// Balle.cpp
#include "Balle.h"
#include <QDebug>
#include<QPen>
#include <QBrush>
Balle::Balle(qreal x, qreal y, qreal width, qreal height)
    : QGraphicsEllipseItem(0, 0, width, height), m_width(width), m_height(height)
{
    setBrush(QBrush(Qt::white));
    setPen(QPen(Qt::black, 2));
    setPos(x, y); // <-- très important : place le ballon globalement
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();

}

QRectF Balle::boundingRect() const {
    return QRectF(0, 0, m_width, m_height);
}


QPainterPath Balle::shape() const
{
    QPainterPath path;
    path.addEllipse(QRectF(0, 0, m_width, m_height));
    return path;
}

void Balle::checkBoundaries()
{
    int terrainMinX = 2;
    int terrainMaxX = 1580;
    int terrainMinY = 4;
    int terrainMaxY = 980;

    if (x() < terrainMinX) setX(terrainMinX);
    if (x() > terrainMaxX) setX(terrainMaxX);
    if (y() < terrainMinY) setY(terrainMinY);
    if (y() > terrainMaxY) setY(terrainMaxY);
}


