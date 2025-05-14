#ifndef JOUEUR_H
#define JOUEUR_H
#include <QFont>
#include <QGraphicsTextItem>
#include <QGraphicsObject>
#include <QBrush>
struct Statistiques {
    int interception = 10;
    int passe = 10;
    int vitesse = 10;
    int dribble = 10;
    int tacle =10;
};
enum Poste {
    LateralGauche,
    LateralDroit,
    Defenseur,
    Milieu,
    AilierGauche,
    AilierDroit,
    AttaquantCentral
};

class Joueur : public QGraphicsObject
{
    Q_OBJECT

public:
    Joueur(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr, int equipe = 1);
    Poste poste;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void setBrush(const QBrush &brush);
    int getTeam() const { return team; }
    void setTeam(int t) { team = t; }
    QString nom;
    int numero;
    void setNom(const QString& n) { nom = n; updateEtiquette(); }
    void setNumero(int num) { numero = num; updateEtiquette(); }
    QString getNom() const { return nom; }
    int getNumero() const { return numero; }
    //void keyPressEvent(QKeyEvent *event) override; // Le keyPressEvent ici pour gérer chaque joueur
Statistiques stats;

    int terrainMinX = 2;
    int terrainMaxX = 1580;
    int terrainMinY = 4;
    int terrainMaxY = 980;
    void checkBoundaries();
private:
    qreal m_width;
    qreal m_height;
    QBrush m_brush;
    int team;
    int equipe;
    QGraphicsTextItem* etiquette;

    void updateEtiquette() {
        if (!etiquette)
            etiquette = new QGraphicsTextItem(this);

        etiquette->setFont(QFont("Arial", 8));
        etiquette->setDefaultTextColor(Qt::white);

        // Centrer l’étiquette au centre du joueur
        etiquette->setPlainText(QString::number(numero) + " - " + nom);
        etiquette->setPos(m_width / 2 - 10, m_height / 2 - 10);
    }
public:
    bool isStunned = false;
    void dribbleAgainst(Joueur* defenseur);
    void stun(int duration_ms);  // immobilise temporairement
    Joueur* trouverAdversaireLePlusProche(Joueur* porteur, Joueur* joueurs[20]) ;

private:
    QTimer* stunTimer = nullptr;


    // Pour savoir à quel joueur on s'adresse
};

#endif // JOUEUR_H
