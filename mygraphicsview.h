#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H
#include <Balle.h>
#include <QGraphicsView>
#include <QKeyEvent>
#include "Joueur.h"
class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    MyGraphicsView(QWidget *parent = nullptr);

    Joueur *player1;
    Joueur *player2;
    Balle *ballon;

protected:
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // MYGRAPHICSVIEW_H
