#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H
#include <Balle.h>
#include <QGraphicsView>
#include <QKeyEvent>
#include "Joueur.h"
#include <QTimer>

struct StatsJoueur {
    int passe;
    int interception;
    int vitesse;
    int endurance;
    int tacle;
    // Tu peux en ajouter d'autres
};
class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT


public:
    MyGraphicsView(QWidget *parent = nullptr);
    StatsJoueur stats[20]; // Un tableau de 20 joueurs (comme ton tableau player[20])
    bool balleEnMouvement = false; // 🆕

    Joueur* player[20];
    Balle* ballon = nullptr;
    void keyPressEvent(QKeyEvent *event) override;

    void botAction();

    void passeManuelle();
    void deplacerBalle();
    Joueur* joueurAvecBallon = nullptr;
    int cibleDefenseur[20];
    int vitesse;
    void avancerVersBalle();
    // À ajouter dans la classe pour garder la cible entre les frames :
    int deuxiemeDefenseur = -1;
    void gererPorteur(int porteur);
    void gererTacle(int defenseur, int porteur);
    int cibleDuDeuxiemeDefenseur = -1;
    void continuerMarquage();
    bool tenterDribble(int porteur);
    void tenterPasseOuDegagement(int porteur);
    void gererPression(int porteur);
    void gererMarquage(int porteur);
    bool verifierEspaceLibre(int porteur);
    void gererDribble(int porteur);
    int trouverPresseur(int porteur);
    void degageBalle(int porteur);
    int ancienneEquipe = 0; // 0 = personne, 1 = Barça, 2 = Real
    qreal facteurAvance();
    void placerDefenseursCentraux(Joueur* defenseur1, Joueur* defenseur2, Joueur* attaquant, qreal vitesse);
    int Possesion(Joueur* player[20]);
    void dessinerTerrain();
    bool estLibrePourPasse(int porteur, int cible);
    double distancePointSegment(double px, double py, double ax, double ay, double bx, double by);
    void essayerPasse(int porteur);
    void placerLateralGauche(Joueur* lateralGauche,Joueur* Ailiergauche, qreal vitesse);
    void placerLateralDroit(Joueur* lateralDroit,Joueur* Ailierdroit,qreal vitesse);
    void placerMilieu(Joueur* joueur, qreal vitesse);
    void placerDefenseur(Joueur* joueur, qreal vitesse);
    void placerAilierGauche(Joueur* ailierGauche, qreal vitesse);
    void placerAilierDroit(Joueur* ailierDroit, qreal vitesse);

    void placerAttaquantCentral(Joueur* attaquantCentral, qreal vitesse);

    void placerEquipeAvecBallon(Joueur* joueurs[20]);

    void deplacerVers(Joueur* joueur, QPointF cible, qreal vitesse);


    void dribblerSiPossible(int porteur);
    int equipeBalle;
protected:
private:

    QList<int> enAttente;
    QTimer *timerDeplacementBalle;
    qreal balleVitesseX = 0;
    qreal balleVitesseY = 0;
    bool balleEnLAir = false; // 🟰 ballon actuellement en l'air ?

};
#endif // _H
