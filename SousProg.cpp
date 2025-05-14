#include <iostream>
#include <mainwindow.h>
#include <joueur.h>
#include <Balle.h>
#include <QPropertyAnimation>
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QPainter>
using namespace std;
void Animation(int Duree, QString DebutX, QString DebutY, QString FinX, QString FinY, QObject* item)
{
    QPropertyAnimation *animation = new QPropertyAnimation(item, "pos");
    animation->setDuration(Duree);
    animation->setStartValue(QPointF(DebutX.toDouble(), DebutY.toDouble()));
    animation->setEndValue(QPointF(FinX.toDouble(), FinY.toDouble()));

    QObject::connect(animation, &QPropertyAnimation::finished, []() {
        qDebug() << "Animation terminée!";
    });

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

int comparaison (int Y,int X,int Z)
{int plusGrand;
if(Y<X)
    {
        if(Y<Z){
            plusGrand=2;}

        else {plusGrand=4;}
    }

    else
    {
        if(X<Z){
        plusGrand=3;}

        else{ plusGrand=4;}
    }
return plusGrand;
}// Démarrer l'animatio
