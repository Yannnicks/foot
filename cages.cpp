#include <QGraphicsItem>
#include <QPainter>
#include <cages.h>

Cage::Cage(qreal x, qreal y, qreal width, qreal height)
    : m_width(width), m_height(height)
{
    setPos(x, y);
}

QRectF Cage::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void Cage::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Couleur des poteaux
    painter->setPen(QPen(Qt::white, 4));

    // Poteaux + barre transversale
    painter->drawLine(0, 0, 0, m_height);             // Poteau gauche
    painter->drawLine(m_width, 0, m_width, m_height);  // Poteau droit
    painter->drawLine(0, 0, m_width, 0);               // Barre transversale

    // Dessiner un filet (des lignes à l'intérieur)
    painter->setPen(QPen(Qt::gray, 1, Qt::DashLine));

    int nbLignes = 5;
    qreal espacementX = m_width / (nbLignes + 1);
    qreal espacementY = m_height / (nbLignes + 1);

    for (int i = 1; i <= nbLignes; ++i) {
        painter->drawLine(espacementX * i, 0, espacementX * i, m_height);
    }

    for (int i = 1; i <= nbLignes; ++i) {
        painter->drawLine(0, espacementY * i, m_width, espacementY * i);
    }
}
