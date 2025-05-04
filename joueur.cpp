#include "Joueur.h"
#include <QPainter>
#include <QKeyEvent>

Joueur::Joueur(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent, int equipe)
    : QGraphicsObject(parent), m_width(width), m_height(height), equipe(equipe)
{
    setPos(x, y);
    m_brush = QBrush(Qt::blue); // Couleur par défaut
    setFlag(QGraphicsItem::ItemIsFocusable); // Pour rendre le joueur focusable
     // Nécessaire pour qu'il reçoive les événements clavier
}

QRectF Joueur::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

void Joueur::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(m_brush);
    painter->drawEllipse(0, 0, m_width, m_height);
}

void Joueur::setBrush(const QBrush &brush)
{
    m_brush = brush;
    update(); // Redessiner l'objet
}
