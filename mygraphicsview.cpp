#include "mygraphicsview.h"
#include "SousProg.h"
#include "Balle.h"
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <QTimer>
#include <cmath>
#include <QRandomGenerator>
#include "qrandom.h"
#include <QGraphicsScene>
MyGraphicsView::MyGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    // Timer pour les bots (vérifie toutes les 50ms = 20 fois/s)
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MyGraphicsView::botAction);
    timer->start(50);  // 20 fois par seconde, ajustez selon vos besoins

    // Timer pour la balle (vitesse de déplacement de la balle)
    timerDeplacementBalle = new QTimer(this);
    connect(timerDeplacementBalle, &QTimer::timeout, this, &MyGraphicsView::deplacerBalle);
    timerDeplacementBalle->start(20);  // toutes les 20 ms

    // Initialisation des joueurs, etc.
    for (int i = 0; i < 20; ++i) {
        stats[i].interception = QRandomGenerator::global()->bounded(0, 21);
        stats[i].passe = QRandomGenerator::global()->bounded(0, 21);
        stats[i].vitesse = QRandomGenerator::global()->bounded(0, 21);
        stats[i].endurance = QRandomGenerator::global()->bounded(0, 21);
    }

    for (int i = 0; i < 20; ++i) {
        player[i] = nullptr;
    }
    ballon = nullptr;
    joueurAvecBallon = nullptr;
}

void MyGraphicsView::deplacerVers(Joueur* joueur, QPointF cible, qreal vitesse)
{
    QPointF dir = cible - joueur->pos();
    qreal distance = sqrt(dir.x()*dir.x() + dir.y()*dir.y());

    if (distance < vitesse) {
        // Si la distance est petite, on s'arrête directement
        joueur->setPos(cible);
    } else {
        dir /= distance; // Normaliser
        joueur->moveBy(dir.x() * vitesse, dir.y() * vitesse);
    }
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (!ballon) return;

    qreal step = 5;
    QPointF move(0, 0);

    switch (event->key()) {
    case Qt::Key_Left: move.setX(-step); break;
    case Qt::Key_Right: move.setX(step); break;
    case Qt::Key_Up: move.setY(-step); break;
    case Qt::Key_Down: move.setY(step); break;

    case Qt::Key_Q: if (player[1]) player[1]->moveBy(-step, 0); return;
    case Qt::Key_D: if (player[1]) player[1]->moveBy(step, 0); return;
    case Qt::Key_Z: if (player[1]) player[1]->moveBy(0, -step); return;
    case Qt::Key_S: if (player[1]) player[1]->moveBy(0, step); return;

    case Qt::Key_X:
        passeManuelle();
        return;

    default:
        QGraphicsView::keyPressEvent(event);
        return;
    }

    if (player[0] && move != QPointF(0, 0)) {
        player[0]->moveBy(move.x(), move.y());

        // 🔵🔵🔵 Mettre à jour les coordonnées APRÈS avoir bougé
        QList<QGraphicsItem*> collisions = player[0]->collidingItems();
        for (QGraphicsItem* item : collisions) {
            if (item == ballon) {
                float pushStrength = 1; // Ajuste selon ce que tu veux

                // Fait bouger la balle dans la même direction que le joueur
                ballon->moveBy(move.x() * pushStrength, move.y() * pushStrength);
            }
        }

    }

    // Limites pour joueur
    for (int i = 0; i < 20; ++i) {
        if (player[i]->x() >= 1580 || player[0]->x() <= 2 || player[i]->y() <= -405 || player[i]->y() >= 295)
            player[0]->moveBy(-move.x()-0.1, -move.y()-0.1);
    }

    // Limites pour ballon
    if (ballon->x() >= 1580 || ballon->x() <= 2 || ballon->y() <= 4 || ballon->y() >= 980)
        ballon->moveBy(-move.x(), -move.y());
}


void MyGraphicsView::passeManuelle()
{
    if (!ballon) return;

    // Vérifie que player[0] touche le ballon
    QList<QGraphicsItem*> collisions = player[0]->collidingItems();
    bool possedeBalle = false;
    for (QGraphicsItem* item : collisions) {
        if (item == ballon) {
            possedeBalle = true;
            break;
        }
    }

    if (possedeBalle) {
        int joueurCible = 0;
        double distanceMin = 1e9;
        for (int j = 0; j <= 4; ++j) {
            if (j == 0) continue; // Ne pas passer à soi-même
            if (player[j]) {
                double distance = std::hypot(player[j]->x() - player[0]->x(), player[j]->y() - player[0]->y());
                if (distance < distanceMin) {
                    distanceMin = distance;
                    joueurCible = j;
                }
            }
        }

        int newX = player[joueurCible]->x();
        int newY = player[joueurCible]->y();
        if (balleEnMouvement) return; // 🚫 Si déjà une passe, on ne fait rien
        balleEnMouvement = true;
        Animation(700,
                  QString::number(ballon->x()),
                  QString::number(ballon->y()),
                  QString::number(newX),
                  QString::number(newY),
                  qobject_cast<QObject*>(ballon));

        // Au lieu de balleEnMouvement = false ici direct,
        // on attend 700 ms avant de le remettre à false :
        QTimer::singleShot(700, [this]() {
            balleEnMouvement = false;
        });

    }
}


void MyGraphicsView::gererPorteur(int porteur)
{
    bool dribblePossible = verifierEspaceLibre(porteur);

    if (dribblePossible) {
        gererDribble(porteur);
    } else {
        essayerPasse(porteur);
    }

    gererPression(porteur);
    gererMarquage(porteur);
}


void MyGraphicsView::avancerVersBalle()
{
    int plusProcheBleu = -1;
    int plusProcheRouge = -1;
    double minDistBleu = 1e9;
    double minDistRouge = 1e9;

    for (int i = 0; i < 20; ++i) {
        if (!player[i]) continue;
        double distance = std::hypot(player[i]->x() - ballon->x(), player[i]->y() - ballon->y());

        if (i < 10) { // Bleu
            if (distance < minDistBleu) {
                minDistBleu = distance;
                plusProcheBleu = i;
            }
        } else { // Rouge
            if (distance < minDistRouge) {
                minDistRouge = distance;
                plusProcheRouge = i;
            }
        }
    }

    if (plusProcheBleu != -1) {
        int vitesse = player[plusProcheBleu]->stats.vitesse;
        double directionX = ballon->x() - player[plusProcheBleu]->x();
        double directionY = ballon->y() - player[plusProcheBleu]->y();
        double norm = std::hypot(directionX, directionY);
        if (norm > 0) {
            directionX /= norm;
            directionY /= norm;
            player[plusProcheBleu]->moveBy(directionX * (vitesse * 0.5), directionY * (vitesse * 0.5));
        }
    }

    if (plusProcheRouge != -1) {
        int vitesse = player[plusProcheRouge]->stats.vitesse;
        double directionX = ballon->x() - player[plusProcheRouge]->x();
        double directionY = ballon->y() - player[plusProcheRouge]->y();
        double norm = std::hypot(directionX, directionY);
        if (norm > 0) {

            directionX /= norm;
            directionY /= norm;
            player[plusProcheRouge]->moveBy(directionX * (vitesse * 0.5), directionY * (vitesse * 0.5));
        }
    }
}





void MyGraphicsView::deplacerBalle()
{
    if (!ballon) return;

    // Déplace la balle
    ballon->moveBy(balleVitesseX, balleVitesseY);

    // Simule un freinage naturel (diminuer la vitesse doucement)
    balleVitesseX *= 0.98;
    balleVitesseY *= 0.98;

    // Si la vitesse devient très petite, arrête
    if (qAbs(balleVitesseX) < 0.1) balleVitesseX = 0;
    if (qAbs(balleVitesseY) < 0.1) balleVitesseY = 0;

    // 🔵 Nouvelle partie : vérifier s'il y a interception
    QList<QGraphicsItem*> collisions = ballon->collidingItems();
    for (QGraphicsItem* item : collisions) {
        for (int i = 0; i < 20; ++i) {
            if (player[i] && player[i] == item) {
                // Tirage de chance d'interception
                int chance = QRandomGenerator::global()->bounded(0, 20);
                if (chance < player[i]->stats.interception) { // 5% par point d'interception
                    qDebug() << "Interception réussie par joueur" << i;

                    balleVitesseX = 0;
                    balleVitesseY = 0;
                    balleEnMouvement = false;

                    return; // On stoppe ici après une interception
                }
            }
        }
    }

}
void MyGraphicsView::continuerMarquage()
{
    if (deuxiemeDefenseur != -1 && cibleDuDeuxiemeDefenseur != -1) {
        Joueur* defenseur = player[deuxiemeDefenseur];
        Joueur* cible = player[cibleDuDeuxiemeDefenseur];

        if (defenseur && cible) {
            int vitesse = defenseur->stats.vitesse;
            double directionX = cible->x() - defenseur->x();
            double directionY = cible->y() - defenseur->y();
            double norm = std::hypot(directionX, directionY);

            if (norm > 0.5) {
                directionX /= norm;
                directionY /= norm;
                defenseur->moveBy(directionX * (vitesse * 0.5), directionY * (vitesse * 0.5));
            }
        }
    }
}

void MyGraphicsView::gererTacle(int defenseur, int porteur)
{
    if (!player[defenseur] || !player[porteur]) return;

    double distance = std::hypot(player[porteur]->x() - player[defenseur]->x(),
                                 player[porteur]->y() - player[defenseur]->y());

    if (distance < 50) {
        int statTacle = player[defenseur]->stats.tacle;
        int statDribble = player[porteur]->stats.dribble;

        double chanceDeTacle = statTacle / (statTacle + statDribble * 1.2);
        double random = QRandomGenerator::global()->bounded(1.0);

        if (random < chanceDeTacle) {
            // Réussite du tacle 🎯
            qDebug() << "Tacle réussi !";

            // Animation simple : petite secousse de la balle pour simuler l'impact
            Animation(300,
                      QString::number(ballon->x()),
                      QString::number(ballon->y()),
                      QString::number(player[defenseur]->x()),
                      QString::number(player[defenseur]->y()),
                      qobject_cast<QObject*>(ballon));

        } else {
            // Échec du tacle ❌
            qDebug() << "Tacle raté !";

            // Animation : petit recul du défenseur pour montrer l'échec
            Animation(300,
                      QString::number(player[defenseur]->x()),
                      QString::number(player[defenseur]->y()),
                      QString::number(player[defenseur]->x() - 10),
                      QString::number(player[defenseur]->y()),
                      qobject_cast<QObject*>(player[defenseur]));
        }
    }
}

bool MyGraphicsView::verifierEspaceLibre(int porteur)
{
    for (int i = 0; i < 20; ++i) {
        if (!player[i] || i == porteur) continue;

        bool adversaire = (i < 10 && porteur >= 10) || (i >= 10 && porteur < 10);
        if (!adversaire) continue;

        double dx = player[i]->x() - player[porteur]->x();
        double dy = player[i]->y() - player[porteur]->y();
        double distance = std::hypot(dx, dy);

        // Si un adversaire est à moins de 150 pixels => il est considéré gênant
        if (distance < 150) {
            return false; // PAS libre
        }
    }
    return true; // LIBRE
}


void MyGraphicsView::gererDribble(int porteur)
{

    int vitesse = player[porteur]->stats.vitesse;
    int dribble = player[porteur]->stats.dribble;
    double avanceX = (porteur >= 10) ? -1 : 1;
    double avanceY = 0;

    double facteurDribble = 1 + (0.6 * (dribble / 100.0));
    double deplacement = vitesse * facteurDribble * 0.9;

    player[porteur]->moveBy(avanceX * deplacement, avanceY * deplacement);
    ballon->moveBy(avanceX * deplacement, avanceY * deplacement);
}

void MyGraphicsView::essayerPasse(int porteur)
{
    int meilleurCible = -1;
    double distanceMin = 1e9;

    for (int j = 0; j < 20; ++j) {
        if (!player[j] || j == porteur) continue;

        bool memeEquipe = (porteur < 10 && j < 10) || (porteur >= 10 && j >= 10);
        if (!memeEquipe) continue;

        if (estLibrePourPasse(porteur, j)) {
            double distance = std::hypot(player[j]->x() - player[porteur]->x(), player[j]->y() - player[porteur]->y());
            if (distance < distanceMin) {
                distanceMin = distance;
                meilleurCible = j;
            }
        }
    }

    if (meilleurCible != -1) {
       // equipeBalle=player[meilleurCible]->getTeam();
        int newX = player[meilleurCible]->x();
        int newY = player[meilleurCible]->y();

        balleEnMouvement = true;
        Animation(700,
                  QString::number(ballon->x()),
                  QString::number(ballon->y()),
                  QString::number(newX),
                  QString::number(newY),
                  qobject_cast<QObject*>(ballon));
    } else {
        // 🔥 Aucun joueur libre : on dégage la balle
        degageBalle(porteur);
    }
}


void MyGraphicsView::gererPression(int porteur)
{
    int presseur = trouverPresseur(porteur);
    if (presseur != -1) {
        int vitesse = player[presseur]->stats.vitesse;
        double directionX = player[porteur]->x() - player[presseur]->x();
        double directionY = player[porteur]->y() - player[presseur]->y();
        double norm = std::hypot(directionX, directionY);
        if (norm > 0) {
            directionX /= norm;
            directionY /= norm;
            player[presseur]->moveBy(directionX * (vitesse * 0.5), directionY * (vitesse * 0.5));
        }
    }
}

int MyGraphicsView::trouverPresseur(int porteur)
{
    int presseur = -1;
    double distanceMinPression = 1e9;
    for (int i = 0; i < 20; ++i) {
        if (!player[i] || i == porteur) continue;

        bool adversaire = (i < 10 && porteur >= 10) || (i >= 10 && porteur < 10);
        if (!adversaire) continue;

        double distance = std::hypot(player[i]->x() - player[porteur]->x(), player[i]->y() - player[porteur]->y());
        if (distance < distanceMinPression) {
            distanceMinPression = distance;
            presseur = i;
        }
    }
    return presseur;
}

void MyGraphicsView::gererMarquage(int porteur)
{
    QList<std::pair<double, int>> attaquantsDistances;
    QList<std::pair<double, int>> defenseursDistances;

    // Séparer les attaquants et les défenseurs
    for (int i = 0; i < 20; ++i) {
        if (!player[i]) continue;

        bool memeEquipe = (i < 10 && porteur < 10) || (i >= 10 && porteur >= 10);  // Vérifier si le joueur fait partie de la même équipe que le porteur
        double dist = std::hypot(player[i]->x() - ballon->x(), player[i]->y() - ballon->y());  // Distance par rapport au ballon

        if (memeEquipe) {
            attaquantsDistances.append({dist, i});  // Attaquant
        } else {
            defenseursDistances.append({dist, i});  // Défenseur
        }
    }

    // Trier les attaquants et les défenseurs par distance croissante
    std::sort(attaquantsDistances.begin(), attaquantsDistances.end());
    std::sort(defenseursDistances.begin(), defenseursDistances.end());

    // Assigner les défenseurs centraux pour marquer les attaquants
    if (attaquantsDistances.size() >= 2 && defenseursDistances.size() >= 2) {
        // Les deux premiers attaquants les plus proches
        int premierAttaquant = attaquantsDistances[0].second;
        int deuxiemeAttaquant = attaquantsDistances[1].second;

        // Les deux premiers défenseurs centraux
        int defenseur1 = defenseursDistances[0].second;
        int defenseur2 = defenseursDistances[1].second;

        // Déplacer les défenseurs centraux vers les attaquants
        Joueur* defenseur1Joueur = player[defenseur1];
        Joueur* defenseur2Joueur = player[defenseur2];
        Joueur* attaquant1 = player[premierAttaquant];
        Joueur* attaquant2 = player[deuxiemeAttaquant];

        if (defenseur1Joueur && defenseur2Joueur && attaquant1 && attaquant2) {
            int vitesse1 = defenseur1Joueur->stats.vitesse;
            int vitesse2 = defenseur2Joueur->stats.vitesse;

            // Calculer la direction vers le premier attaquant
            double dx1 = attaquant1->x() - defenseur1Joueur->x();
            double dy1 = attaquant1->y() - defenseur1Joueur->y();
            double norm1 = std::hypot(dx1, dy1);

            // Calculer la direction vers le deuxième attaquant
            double dx2 = attaquant2->x() - defenseur2Joueur->x();
            double dy2 = attaquant2->y() - defenseur2Joueur->y();
            double norm2 = std::hypot(dx2, dy2);

            // Normaliser et déplacer les défenseurs
            if (norm1 > 0.5) {
                dx1 /= norm1;
                dy1 /= norm1;
                defenseur1Joueur->moveBy(dx1 * (vitesse1 * 0.5), dy1 * (vitesse1 * 0.5));
            }

            if (norm2 > 0.5) {
                dx2 /= norm2;
                dy2 /= norm2;
                defenseur2Joueur->moveBy(dx2 * (vitesse2 * 0.5), dy2 * (vitesse2 * 0.5));
            }
        }
    }

    // Gérer le marquage pour les autres défenseurs (3ème défenseur et plus)
    for (int i = 2; i < defenseursDistances.size(); ++i) {
        int defenseurIndex = defenseursDistances[i].second;
        int joueurIndex = attaquantsDistances[i].second;

        Joueur* defenseur = player[defenseurIndex];
        Joueur* cible = player[joueurIndex];

        if (defenseur && cible) {
            int vitesse = defenseur->stats.vitesse;
            double dx = cible->x() - defenseur->x();
            double dy = cible->y() - defenseur->y();
            double norm = std::hypot(dx, dy);

            if (norm > 0.5) {
                dx /= norm;
                dy /= norm;
                defenseur->moveBy(dx * (vitesse * 0.5), dy * (vitesse * 0.5));
            }
        }
    }
}


bool MyGraphicsView::estLibrePourPasse(int porteur, int cible)
{
    if (!player[porteur] || !player[cible]) return false;

    for (int i = 0; i < 20; ++i) {
        if (!player[i] || i == porteur || i == cible) continue;

        bool adversaire = (i < 10 && porteur >= 10) || (i >= 10 && porteur < 10);
        if (!adversaire) continue;

        double distToTrajectoire = distancePointSegment(
            player[i]->x(), player[i]->y(),
            player[porteur]->x(), player[porteur]->y(),
            player[cible]->x(), player[cible]->y()
            );

        if (distToTrajectoire < 30) { // ⚡ Si l'adversaire est à moins de 30 px de la trajectoire
            return false;
        }
    }
    return true;
}
double MyGraphicsView::distancePointSegment(double px, double py, double ax, double ay, double bx, double by)
{
    double abx = bx - ax;
    double aby = by - ay;
    double apx = px - ax;
    double apy = py - ay;

    double ab_ap = abx * apx + aby * apy;
    double ab_ab = abx * abx + aby * aby;
    double t = (ab_ab == 0) ? 0 : ab_ap / ab_ab;
    t = std::max(0.0, std::min(1.0, t));

    double closestX = ax + t * abx;
    double closestY = ay + t * aby;

    double dx = px - closestX;
    double dy = py - closestY;

    return std::hypot(dx, dy);
}
void MyGraphicsView::degageBalle(int porteur)
{
    double avanceX = (porteur >= 10) ? -1 : 1;
    double avanceY = 0;
    int puissanceDegagement = 300; // Distance du dégagement

    qreal newX = ballon->x() + avanceX * puissanceDegagement;
    qreal newY = ballon->y();


    balleEnMouvement = true;
    balleEnLAir = true; // 🚀 La balle est en l'air

    Animation(500,
              QString::number(ballon->x()),
              QString::number(ballon->y()),
              QString::number(newX),
              QString::number(newY),
              qobject_cast<QObject*>(ballon));

    QTimer::singleShot(2000, this, [this]() {
        balleEnLAir = false; // Après 2 secondes, la balle peut être interceptée à nouveau
    });
}

void MyGraphicsView::placerLateralGauche(Joueur* lateralGauche,Joueur* ailierGauche, qreal vitesse)
{
    QRectF carreNoir(50, 0, 848, 256);
    QRectF carreJaune(448, 0, 512, 256);

    QPointF cible = carreNoir.center(); QPointF ballePos = ballon->pos();
    QPointF joueurPos = lateralGauche->pos();

    // Calculer la direction vers la balle
    QPointF direction = ballePos - joueurPos;

    // Normaliser la direction (faire en sorte que la direction soit une unité)
    qreal distance = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (distance > 0) {
        direction /= distance;
    }

    // Calculer la nouvelle position du joueur en se déplaçant vers la balle
    QPointF nouvellePosition = joueurPos + direction * vitesse;

    // Vérifier que la nouvelle position reste dans la zone définie par le carré
    if (carreNoir.contains(nouvellePosition)) {
        lateralGauche->setPos(nouvellePosition);  // Déplacer le joueur
    } else {
        // Si la position est en dehors de la zone, on doit rester dans le carré.
        // Donc, on déplace le joueur vers le point le plus proche du bord du carré.
        qreal x = std::clamp(nouvellePosition.x(), carreNoir.left(), carreNoir.right());
        qreal y = std::clamp(nouvellePosition.y(), carreNoir.top(), carreNoir.bottom());

        lateralGauche->setPos(x, y);  // Mettre à jour la position du joueur
    }

    // Vérifier si l'ailier gauche est dans la zone "carreJaune" et le laisser dans sa zone
    bool joueurDansCarreJaune = carreJaune.contains(ailierGauche->pos());
    if (!joueurDansCarreJaune) {
        // Déplacer le latéral gauche pour interagir avec l'ailier si nécessaire
        // Cette partie peut être ajustée en fonction de la logique souhaitée
        // Pour l'instant, il continue à être guidé par la balle.
    }
}
void MyGraphicsView::placerLateralDroit(Joueur* lateralDroit,Joueur* Ailierdroit,qreal vitesse)
{
    QRectF carreBleu(50, 800, 848, 256);
/*Premier paramètre (100) : C'est la coordonnée X du coin supérieur gauche du rectangle. Cela signifie que le coin supérieur gauche du rectangle sera à 100 pixels en horizontal par rapport à l'origine (l'origine étant généralement le coin supérieur gauche de la scène).

Deuxième paramètre (0) : C'est la coordonnée Y du coin supérieur gauche du rectangle. Cela signifie que le coin supérieur gauche sera positionné à 0 pixels en vertical par rapport à l'origine de la scène, donc tout en haut.

Troisième paramètre (448) : C'est la largeur du rectangle. Ici, le rectangle aura une largeur de 448 pixels.

Quatrième paramètre (256) : C'est la hauteur du rectangle. Dans cet exemple, la hauteur du rectangle sera de 256 pixels.*/
QRectF carreBlanc(448, 344, 812, 256);
        // Récupérer la position de la balle
    QPointF ballePos = ballon->pos();
    QPointF joueurPos = lateralDroit->pos();

    // Calculer la direction vers la balle
    QPointF direction = ballePos - joueurPos;

    // Normaliser la direction (garder une direction unitaire)
    qreal distance = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    if (distance > 0) {
        direction /= distance;
    }

    // Calculer la nouvelle position du joueur en se déplaçant vers la balle
    QPointF nouvellePosition = joueurPos + direction * vitesse;

    // Vérifier que la nouvelle position reste dans la zone définie par le carré
    if (carreBleu.contains(nouvellePosition)) {
        lateralDroit->setPos(nouvellePosition);  // Déplacer le joueur
    } else {
        // Si la position est en dehors de la zone, on doit rester dans le carré.
        // Donc, on déplace le joueur vers le point le plus proche du bord du carré.
        qreal x = std::clamp(nouvellePosition.x(), carreBleu.left(), carreBleu.right());
        qreal y = std::clamp(nouvellePosition.y(), carreBleu.top(), carreBleu.bottom());

        lateralDroit->setPos(x, y);  // Mettre à jour la position du joueur
    }
}
void MyGraphicsView::placerMilieu(Joueur* joueur, qreal vitesse)
{
    if (joueur->poste == Milieu)
        {
            QRectF carreRose(200, 200, 800, 600); // Exemple de zone cible pour les milieux
            {
                deplacerVers(joueur, carreRose.center(), vitesse);
            }
        }
}



void MyGraphicsView::placerDefenseursCentraux(Joueur* defenseur1, Joueur* defenseur2, Joueur* attaquant, qreal vitesse)
{
    // Position de l'attaquant le plus avancé
    QPointF positionAttaquant = attaquant->pos();

    // Position en X des défenseurs, ils doivent être 300px derrière l'attaquant
    qreal xDefenseur = positionAttaquant.x() - 200;

    // Les défenseurs doivent être espacés de 300px sur l'axe Y
    qreal yDefenseur2 = positionAttaquant.y() - 75;  // Défenseur 1 plus haut
    qreal yDefenseur1 = positionAttaquant.y() + 75;  // Défenseur 2 plus bas

    // Placer le premier défenseur central
    QPointF positionDefenseur2(xDefenseur, yDefenseur2);
    deplacerVers(defenseur2, positionDefenseur2, vitesse);

    // Placer le second défenseur central
    QPointF positionDefenseur1(xDefenseur, yDefenseur1);
    deplacerVers(defenseur1, positionDefenseur1, vitesse);
}



void MyGraphicsView::placerAilierGauche(Joueur* ailierGauche, qreal vitesse)
{
    // Définir la zone de l'ailier gauche
    QRectF carreJaune(208, 800, 512, 256);

    // Déterminer la position cible en fonction du ballon
    QPointF cible = carreJaune.center();  // Par défaut, on place l'ailier au centre de sa zone
    qreal distanceAuBallon = std::hypot(ballon->x() - ailierGauche->x(), ballon->y() - ailierGauche->y());

    // Si l'ailier est assez loin du ballon, il se rapproche
    if (distanceAuBallon > 200) {
        // Déplacer vers le ballon en ajustant la vitesse
        cible = ballon->pos();
    }

    // Faire en sorte que l'ailier reste dans les limites de la zone
    if (cible.x() < carreJaune.left()) {
        cible.setX(carreJaune.left());
    } else if (cible.x() > carreJaune.right()) {
        cible.setX(carreJaune.right());
    }

    if (cible.y() < carreJaune.top()) {
        cible.setY(carreJaune.top());
    } else if (cible.y() > carreJaune.bottom()) {
        cible.setY(carreJaune.bottom());
    }

    // Déplacer l'ailier vers la cible calculée
    deplacerVers(ailierGauche, cible, vitesse);
}


void MyGraphicsView::placerAilierDroit(Joueur* ailierDroit, qreal vitesse)
{
    // Définir la zone de l'ailier droit
    QRectF carreBlanc(208, 0, 512, 256);

    // Déterminer la position cible en fonction du ballon
    QPointF cible = carreBlanc.center();  // Par défaut, on place l'ailier au centre de sa zone
    qreal distanceAuBallon = std::hypot(ballon->x() - ailierDroit->x(), ballon->y() - ailierDroit->y());

    // Si l'ailier est assez loin du ballon, il se rapproche
    if (distanceAuBallon > 200) {
        // Déplacer vers le ballon en ajustant la vitesse
        cible = ballon->pos();
    }

    // Faire en sorte que l'ailier reste dans les limites de la zone
    if (cible.x() < carreBlanc.left()) {
        cible.setX(carreBlanc.left());
    } else if (cible.x() > carreBlanc.right()) {
        cible.setX(carreBlanc.right());
    }

    if (cible.y() < carreBlanc.top()) {
        cible.setY(carreBlanc.top());
    } else if (cible.y() > carreBlanc.bottom()) {
        cible.setY(carreBlanc.bottom());
    }

    // Déplacer l'ailier vers la cible calculée
    deplacerVers(ailierDroit, cible, vitesse);
}

void MyGraphicsView::placerAttaquantCentral(Joueur* attaquantCentral, qreal vitesse)
{
    QRectF carreMarron(960-112, 0, 112, 512);
    deplacerVers(attaquantCentral, carreMarron.center(), vitesse);
}
int detecterEquipeAvecBallon(QVector<Joueur*> joueurs, Balle* ballon, qreal distanceSeuil = 30.0)
{
    Joueur* joueurLePlusProche = nullptr;
    qreal distanceMin = std::numeric_limits<qreal>::max(); // une distance énorme au début

    QPointF positionBallon = ballon->pos();

    for (Joueur* joueur : joueurs)
    {
        if (!joueur) continue;

        QPointF positionJoueur = joueur->pos();
        qreal distance = QLineF(positionBallon, positionJoueur).length();

        if (distance < distanceMin)
        {
            distanceMin = distance;
            joueurLePlusProche = joueur;
        }
    }

    if (joueurLePlusProche && distanceMin <= distanceSeuil)
    {
        return joueurLePlusProche->getTeam(); // L'équipe du joueur le plus proche
    }
    else
    {
        return 0; // 0 = personne n'a la balle
    }
}

void MyGraphicsView::placerEquipeAvecBallon(Joueur* joueurs[20])
{
    int Nbrdef = 0, Nbrmil = 0, lateralGauche = 0, lateralDroit = 0, defenseur1 = 0, defenseur2 = 0;
    int milieu1 = 0, milieu2 = 0, milieu3 = 0, ailierGauche = 0, ailierDroit = 0, attaquantCentral = 0;
    int Nbrdef2 = 0, Nbrmil2 = 0, lateralGauche2 = 0, lateralDroit2 = 0, defenseur12 = 0, defenseur22 = 0;
    int milieu12 = 0, milieu22 = 0, milieu32 = 0, ailierGauche2 = 0, ailierDroit2 = 0, attaquantCentral2 = 0;
    int vitesse = 5;

    int porteur = -1;

    // Trouver le porteur
    for (int i = 0; i < 20; ++i) {
        QList<QGraphicsItem*> collisions = joueurs[i]->collidingItems();
        for (QGraphicsItem* item : collisions) {
            if (item == ballon) {
                porteur = i;
                break;
            }
        }
        if (porteur != -1)
            break;
    }

    // Répartition des postes
    for (int i = 0; i < 20; ++i) {
        if (i <= 9) {
            switch (joueurs[i]->poste) {
            case LateralGauche: lateralGauche = i; break;
            case LateralDroit: lateralDroit = i; break;
            case Defenseur:
                if (Nbrdef == 0) defenseur1 = i;
                else if (Nbrdef == 1) defenseur2 = i;
                Nbrdef++;
                break;
            case Milieu:
                if (Nbrmil == 0) milieu1 = i;
                else if (Nbrmil == 1) milieu2 = i;
                else if (Nbrmil == 2) milieu3 = i;
                Nbrmil++;
                break;
            case AilierGauche: ailierGauche = i; break;
            case AilierDroit: ailierDroit = i; break;
            case AttaquantCentral: attaquantCentral = i; break;
            }
        } else {
            switch (joueurs[i]->poste) {
            case LateralGauche: lateralGauche2 = i; break;
            case LateralDroit: lateralDroit2 = i; break;
            case Defenseur:
                if (Nbrdef2 == 0) defenseur12 = i;
                else if (Nbrdef2 == 1) defenseur22 = i;
                Nbrdef2++;
                break;
            case Milieu:
                if (Nbrmil2 == 0) milieu12 = i;
                else if (Nbrmil2 == 1) milieu22 = i;
                else if (Nbrmil2 == 2) milieu32 = i;
                Nbrmil2++;
                break;
            case AilierGauche: ailierGauche2 = i; break;
            case AilierDroit: ailierDroit2 = i; break;
            case AttaquantCentral: attaquantCentral2 = i; break;
            }
        }
    }

    // Trouver le porteur du ballon
    for (int i = 0; i < 20; ++i) {
        QList<QGraphicsItem*> collisions = joueurs[i]->collidingItems();
        for (QGraphicsItem* item : collisions) {
            if (item == ballon) {
                porteur = i;
                break;
            }
        }
        if (porteur != -1) break;
    }

    int equipe = -1;
    if (porteur != -1) {
        equipe = joueurs[porteur]->getTeam();
    } else {
        equipe = Possesion(joueurs); // Fallback si pas trouvé
    }

    // ⚽ Placer l'équipe avec le ballon
    if (equipe == 1) { // Équipe 1 (bleu)
        // Placements de l'équipe 1 en fonction du poste
        if (lateralGauche != porteur) placerLateralGauche(joueurs[lateralGauche], joueurs[ailierGauche], vitesse);
        if (lateralDroit != porteur) placerLateralDroit(joueurs[lateralDroit], joueurs[ailierDroit], vitesse);

        // Milieu
        if (milieu1 != porteur) placerMilieu(joueurs[milieu1], vitesse);
        if (milieu2 != porteur) placerMilieu(joueurs[milieu2], vitesse);
        if (milieu3 != porteur) placerMilieu(joueurs[milieu3], vitesse);

        // Défenseurs centraux
        if (defenseur1 != porteur && defenseur2 != porteur) {
            placerDefenseursCentraux(joueurs[defenseur2], joueurs[defenseur1], joueurs[attaquantCentral2], vitesse);
        }

        // Ailiers et attaquants
        if (ailierGauche != porteur) placerAilierGauche(joueurs[ailierGauche], vitesse);
        if (ailierDroit != porteur) placerAilierDroit(joueurs[ailierDroit], vitesse);
        if (attaquantCentral != porteur) placerAttaquantCentral(joueurs[attaquantCentral], vitesse);

    } else if (equipe == 2) { // Équipe 2 (rouge)
        // Placements de l'équipe 2 en fonction du poste
        if (lateralGauche2 != porteur) placerLateralGauche(joueurs[lateralGauche2], joueurs[ailierGauche2], vitesse);
        if (lateralDroit2 != porteur) placerLateralDroit(joueurs[lateralDroit2], joueurs[ailierDroit2], vitesse);

        // Milieu
        if (milieu12 != porteur) placerMilieu(joueurs[milieu12], vitesse);
        if (milieu22 != porteur) placerMilieu(joueurs[milieu22], vitesse);
        if (milieu32 != porteur) placerMilieu(joueurs[milieu32], vitesse);

        // Défenseurs centraux
        if (defenseur12 != porteur && defenseur22 != porteur) {
            placerDefenseursCentraux(joueurs[defenseur22], joueurs[defenseur12], joueurs[attaquantCentral], vitesse);
        }

        // Ailiers et attaquants
        if (ailierGauche2 != porteur) placerAilierGauche(joueurs[ailierGauche2], vitesse);
        if (ailierDroit2 != porteur) placerAilierDroit(joueurs[ailierDroit2], vitesse);
        if (attaquantCentral2 != porteur) placerAttaquantCentral(joueurs[attaquantCentral2], vitesse);
    }
}

int MyGraphicsView::Possesion(Joueur* player[20])
{
    int porteur = -1;
    const qreal rayonConflit = 20.0;
    int equipe1Proche = -1;
    int equipe2Proche = -1;

    for (int i = 0; i < 20; ++i) {
        // Vérifie distance directe avec le ballon
        qreal dx = player[i]->x() - ballon->x();
        qreal dy = player[i]->y() - ballon->y();
        qreal distance = std::sqrt(dx * dx + dy * dy);

        if (distance < rayonConflit) {
            if (player[i]->getTeam() == 1)
                equipe1Proche = i;
            else if (player[i]->getTeam() == 2)
                equipe2Proche = i;
        }

        // Vérifie collision physique (bounding box)
        QList<QGraphicsItem*> collisions = player[i]->collidingItems();
        for (QGraphicsItem* item : collisions) {
            if (item == ballon) {
                porteur = i;
                break;
            }
        }
        if (porteur != -1)
            break;
    }

    // Si les deux équipes sont proches → personne n'a clairement la balle
    if (equipe1Proche != -1 && equipe2Proche != -1) {
        return 0;
    }

    if (porteur == -1) {
        return 0; // Aucun contact ni proximité claire
    }

    return player[porteur]->getTeam();
}


void MyGraphicsView::botAction()
{
    avancerVersBalle();  // Déplacement vers la balle
    continuerMarquage();  // Logique de marquage des adversaires

    int porteur = -1;
    for (int i = 0; i < 20; ++i) {
        if (!player[i]) continue;
        QList<QGraphicsItem*> collisions = player[i]->collidingItems();
        for (QGraphicsItem* item : collisions) {
            if (item == ballon) {
                porteur = i;
                break;
            }
        }
        if (porteur != -1) break;
    }

    if (porteur != -1) {
        gererPorteur(porteur);  // Mise à jour du porteur de ballon
        ancienneEquipe = player[porteur]->getTeam();  // Mise à jour de l'équipe du porteur
    } else {
        avancerVersBalle();  // Si personne n'a le ballon, continue de s'approcher
    }

    int nouvelleEquipe = Possesion(player);  // Mise à jour de l'équipe qui possède le ballon

    // Si la possession change, mettre à jour l'état et replacer
    if (nouvelleEquipe > 0 && nouvelleEquipe != ancienneEquipe) {
        ancienneEquipe = nouvelleEquipe;
        placerEquipeAvecBallon(player);  // Placer l'équipe après la passe
    }

    // Toujours placer l’équipe avec la dernière possession connue
    if (ancienneEquipe > 0) {
        placerEquipeAvecBallon(player);
    }
}

qreal MyGraphicsView::facteurAvance() {
    // ballonX varie de 0 (gauche) à 1600 (droite)
    // Normaliser entre 0 et 1
    return ballon->x() / 1600.0;
}



