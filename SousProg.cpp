#include <iostream>
#include <mainwindow.h>
#include <joueur.h>
#include <Balle.h>
#include <QPropertyAnimation>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QPainter>
using namespace std;
void Animation(int Duree,int DebutX,int DebutY,int FinX,int FinY,Joueur* Joueur1)
{
QPropertyAnimation *animation = new QPropertyAnimation(Joueur1, "pos");
animation->setDuration(Duree);
animation->setStartValue(QPointF(DebutX,DebutY));
animation->setEndValue(QPointF(FinX, FinY));
// Signal pour détecter quand l'animation est terminée
QObject::connect(animation, &QPropertyAnimation::finished, [](){
    qDebug() << "Animation terminée!";
});

// Démarrer l'animation
animation->start(QAbstractAnimation::DeleteWhenStopped);
}
void Mouvement(Joueur* joueur, int equipe, QKeyEvent* event)
{
    qreal step = 10; // nombre de pixels par déplacement

    if (equipe == 1)  // Joueur 1
    {
        switch (event->key()) {
        case Qt::Key_Left:
            joueur->moveBy(-step, 0);
            break;
        case Qt::Key_Right:
            joueur->moveBy(step, 0);
            break;
        case Qt::Key_Up:
            joueur->moveBy(0, -step);
            break;
        case Qt::Key_Down:
            joueur->moveBy(0, step);
            break;
        default:
            break;
        }
    }
    else if (equipe == 2)  // Joueur 2
    {
        switch (event->key()) {
        case Qt::Key_Q:
            joueur->moveBy(-step, 0);
            break;
        case Qt::Key_D:
            joueur->moveBy(step, 0);
            break;
        case Qt::Key_W:
            joueur->moveBy(0, -step);
            break;
        case Qt::Key_S:
            joueur->moveBy(0, step);
            break;
        default:
            break;
        }
    }
}
