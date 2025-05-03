#include "Balle.h"
#include "Joueur.h"
#include <QGraphicsItem>
#include <QDebug>

Balle::Balle(qreal x, qreal y, qreal width, qreal height)
    : QGraphicsEllipseItem(x, y, width, height)
{
    // Constructeur vide ou initialisation spécifique ici
}

void Balle::verifieCollision() {
    QList<QGraphicsItem*> collisions = collidingItems();

    for (QGraphicsItem* item : collisions) {
        Joueur* joueur = dynamic_cast<Joueur*>(item);
        if (joueur) {
            qDebug() << "Collision détectée avec un joueur !";

            // Exemple de réaction : léger déplacement
            setPos(pos().x() - 5, pos().y());
        }
    }
}
