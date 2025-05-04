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
    setFocus();
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event)
{
    int CollisionBallonX=ballon->x();
    int CollisionBallonY=ballon->y();
    int CollisionJoueurX=player1->x();
    int CollisionJoueurY =player1->y();
    int CalculY=CollisionBallonY-CollisionJoueurY;
    int CalculX=CollisionBallonX-CollisionJoueurX;
    qreal step = 5;
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
        if (player1 && ballon) {int newX = ballon->x() + 200,newY=ballon->y();
            QList<QGraphicsItem*> collisions = player1->collidingItems();
                if ((CollisionJoueurX - CollisionBallonX <= 10 && CollisionJoueurX - CollisionBallonX >= -10) || (CollisionJoueurY - CollisionBallonY <= 10 && CollisionJoueurY - CollisionBallonY >= -10)) {
                    if(CollisionJoueurY<CollisionBallonY||CollisionJoueurY>CollisionBallonY)
                    {
                    newY=ballon->y()+CalculY;
                    }
                    Animation(700,
                              QString::number(ballon->x()),
                              QString::number(ballon->y()),
                              QString::number(newX),
                              QString::number(newY),
                              qobject_cast<QObject*>(ballon));
                    break;
                }

        }
        return;
    default:
        QGraphicsView::keyPressEvent(event);
        return;
    }
   /* case Qt::Key_X:
        if ((CollisionJoueurX - CollisionBallonX <= 10 && CollisionJoueurX - CollisionBallonX >= -10) || (CollisionJoueurY - CollisionBallonY <= 10 && CollisionJoueurY - CollisionBallonY >= -10))
        {
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

*/
    float CoefficientEnroule=0.25;
    // Déplacement de player1 uniquement si pas de collision après déplacement
    if (player1 && move != QPointF(0, 0)) {
        player1->moveBy(move.x(), move.y());

        // Vérifier collision après tentative de déplacement
        QList<QGraphicsItem*> collisions = player1->collidingItems();
        for (QGraphicsItem* item : collisions) {
            if (item == ballon) {
          /*      //player1->moveBy(-move.x(), -move.y());
                ballon->moveBy(move.x(), -move.y());                // annule le déplacement
                qDebug() << "Collision bloquée avec la balle.";
                break;
            }*/
              if((CollisionJoueurX<CollisionBallonX|| CollisionJoueurX>CollisionBallonX) && CollisionJoueurY!=CollisionBallonY){
              ballon->moveBy(move.x()+CalculX*CoefficientEnroule-1, move.y());}
              if(CollisionJoueurY<CollisionBallonY||CollisionJoueurY>CollisionBallonY)
              {
                  ballon->moveBy(move.x(), move.y()+CalculY*CoefficientEnroule-1)
              ;
              }
              if((CollisionJoueurX<CollisionBallonX|| CollisionJoueurX>CollisionBallonX )&& CollisionJoueurY==CollisionBallonY){
                  ballon->moveBy(move.x(), move.y());
              }
              /*if(CollisionJoueurY>CollisionBallonY)
              {
                  ballon->moveBy(move.x(), move.y()+CalculY*0.25);
              }*/
            }}
    }

    if (CollisionJoueurX==510||CollisionJoueurX==-540||CollisionJoueurY==-250||CollisionJoueurY==220) {
        player1->moveBy(-move.x(), -move.y());
    }

    if (CollisionBallonX==500||CollisionBallonX==-525||CollisionBallonY==-250||CollisionBallonY==220) {
         ballon->moveBy(-move.x(), -move.y());
    }
}
