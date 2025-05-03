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

// Démarrer l'animatio
