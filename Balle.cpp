// Balle.cpp
#include "Balle.h"
#include <QDebug>
#include<QPen>
#include <QBrush>
Balle::Balle(qreal x, qreal y, qreal width, qreal height)
    : QGraphicsEllipseItem(x, y, width, height), m_width(width), m_height(height)
{
    setBrush(QBrush(Qt::white));   // couleur de remplissage
    setPen(QPen(Qt::black, 2));
}
QRectF Balle::boundingRect() const {
    return QRectF(10,10, m_width, m_height); // réduit seulement 1px de chaque côté
}


QPainterPath Balle::shape() const
{

        QPainterPath path;
        path.addEllipse(QRectF(10, 10, m_width, m_height)); // correspond à la vraie forme
        return path;
}
