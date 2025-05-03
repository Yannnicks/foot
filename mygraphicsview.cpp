#include "mygraphicsview.h"
#include "SousProg.h"
#include "Balle.h"
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QDebug>

MyGraphicsView::MyGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setFocusPolicy(Qt::StrongFocus); // Important
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event)
{
    qreal step = 10;
    QPointF move(0, 0);

    switch (event->key()) {
    case Qt::Key_Left:
        move.setX(-step);
        break;
    case Qt::Key_Right:
        move.setX(step);
        break;
    case Qt::Key_Up:
        move.setY(-step);
        break;
    case Qt::Key_Down:
        move.setY(step);
        break;
    case Qt::Key_Q:
        if (player2) player2->moveBy(-step, 0);
        return;
    case Qt::Key_D:
        if (player2) player2->moveBy(step, 0);
        return;
    case Qt::Key_Z:
        if (player2) player2->moveBy(0, -step);
        return;
    case Qt::Key_S:
        if (player2) player2->moveBy(0, step);
        return;
    case Qt::Key_X:
        if (player1 && ballon) {
            QList<QGraphicsItem*> collisions = player1->collidingItems();
            for (QGraphicsItem* item : collisions) {
                if (item == ballon) {
                    int newX = ballon->x() + 200;
                    Animation(700,
                              QString::number(ballon->x()),
                              QString::number(ballon->y()),
                              QString::number(newX),
                              QString::number(ballon->y()),
                                qobject_cast<QObject*>(ballon));
                    break;
                }
            }
        }
        return;
    default:
        QGraphicsView::keyPressEvent(event);
        return;
    }

    // Déplacement de player1 uniquement si pas de collision après déplacement
    if (player1 && move != QPointF(0, 0)) {
        player1->moveBy(move.x(), move.y());

        // Vérifier collision après tentative de déplacement
        QList<QGraphicsItem*> collisions = player1->collidingItems();
        for (QGraphicsItem* item : collisions) {
            if (item == ballon) {
                player1->moveBy(-move.x(), -move.y()); // annule le déplacement
                qDebug() << "Collision bloquée avec la balle.";
                break;
            }
        }
    }
}
