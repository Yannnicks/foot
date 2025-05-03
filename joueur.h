#ifndef JOUEUR_H
#define JOUEUR_H

#include <QGraphicsObject>
#include <QBrush>

class Joueur : public QGraphicsObject
{
    Q_OBJECT

public:
    Joueur(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = nullptr, int equipe = 1);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void setBrush(const QBrush &brush);

    void keyPressEvent(QKeyEvent *event) override; // Le keyPressEvent ici pour gérer chaque joueur

private:
    qreal m_width;
    qreal m_height;
    QBrush m_brush;
    int equipe;  // Pour savoir à quel joueur on s'adresse
};

#endif // JOUEUR_H
