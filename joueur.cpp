#include "Joueur.h"
#include <QPainter>
#include <QKeyEvent>

Joueur::Joueur(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent, int equipe)
    : QGraphicsObject(parent), m_width(width), m_height(height), equipe(equipe)
{
    setPos(x, y);
    m_brush = QBrush(Qt::blue); // Couleur par défaut
    setFlag(QGraphicsItem::ItemIsFocusable); // Pour rendre le joueur focusable
    setFocus(); // Nécessaire pour qu'il reçoive les événements clavier
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

void Joueur::keyPressEvent(QKeyEvent *event)
{
    qreal step = 10; // nombre de pixels par déplacement

    if (equipe == 1)  // Joueur 1 (Bleu)
    {
        switch (event->key()) {
        case Qt::Key_Left:
            moveBy(-step, 0);
            break;
        case Qt::Key_Right:
            moveBy(step, 0);
            break;
        case Qt::Key_Up:
            moveBy(0, -step);
            break;
        case Qt::Key_Down:
            moveBy(0, step);
            break;
        default:
            break;
        }
    }
    else if (equipe == 2)  // Joueur 2 (Rouge)
    {
        switch (event->key()) {
        case Qt::Key_Q:
            moveBy(-step, 0);
            break;
        case Qt::Key_D:
            moveBy(step, 0);
            break;
        case Qt::Key_W:
            moveBy(0, -step);
            break;
        case Qt::Key_S:
            moveBy(0, step);
            break;
        default:
            break;
        }
    }
}
