#include "Joueur.h"
#include <QPainter>
#include <QKeyEvent>
#include "QTimer"
#include <QRandomGenerator>
Joueur::Joueur(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent, int equipe)
    : QGraphicsObject(parent), m_width(width), m_height(height), equipe(equipe)
{
    setPos(x, y);
    m_brush = QBrush(Qt::blue); // Couleur par défaut
    setFlag(QGraphicsItem::ItemIsFocusable); // Pour rendre le joueur focusable
    setFocus();
    etiquette = new QGraphicsTextItem(this);
    updateEtiquette(); // Pour afficher le texte dès la création

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

void Joueur::checkBoundaries()
{
    if (x() < terrainMinX) setX(terrainMinX);
    if (x() > terrainMaxX) setX(terrainMaxX);
    if (y() < terrainMinY) setY(terrainMinY);
    if (y() > terrainMaxY) setY(terrainMaxY);
}
void Joueur::dribbleAgainst(Joueur* defenseur) {
    if (this->getTeam() == defenseur->getTeam()) return;

    qreal dx = this->x() - defenseur->x();
    qreal dy = this->y() - defenseur->y();
    qreal distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 5.0) return; // trop loin

    int dribbleStat = this->stats.dribble;
    int tacleStat = defenseur->stats.tacle;

    int dribbleWeight = dribbleStat;
    int tacleWeight = tacleStat * 1.5;  // pondération pour que tacle ait +60% si égalité à 20

    int total = dribbleWeight + tacleWeight;

    int random = QRandomGenerator::global()->bounded(total);

    if (random < dribbleWeight) {
        // Dribbleur réussit, défenseur stun
        defenseur->stun(2000);
    } else {
        // Défenseur gagne le duel, dribbleur stun
        this->stun(2000);
    }
}

void Joueur::stun(int duration_ms) {
    if (isStunned) return;
    isStunned = true;

    setOpacity(0.5); // 💡 rend le joueur semi-transparent

    if (!stunTimer) stunTimer = new QTimer(this);
    stunTimer->setSingleShot(true);
    connect(stunTimer, &QTimer::timeout, this, [=]() {
        isStunned = false;
        setOpacity(1.0); // 🔁 revient à l'opacité normale
    });
    stunTimer->start(duration_ms);
}
#include <limits>   // pour std::numeric_limits
#include <cmath>    // pour std::hypot

#include <limits>
#include <cmath>

int trouverAdversaireLePlusProche(Joueur* porteur, Joueur* joueurs[20]) {
    int indexPlusProche = -1;
    qreal distanceMin = std::numeric_limits<qreal>::max();

    for (int i = 0; i < 20; ++i) {
        Joueur* autre = joueurs[i];

        if (autre == nullptr || autre == porteur)
            continue;

        if (autre->getTeam() == porteur->getTeam())
            continue;

        qreal dx = porteur->x() - autre->x();
        qreal dy = porteur->y() - autre->y();
        qreal distance = std::hypot(dx, dy);

        if (distance < distanceMin) {
            distanceMin = distance;
            indexPlusProche = i;
        }
    }

    return indexPlusProche; // -1 s’il n’y a aucun adversaire trouvé
}



