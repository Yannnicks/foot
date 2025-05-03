#ifndef SOUSPROG_H
#define SOUSPROG_H
#include"Balle.h"
#include <joueur.h>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QMainWindow>
#include <QGraphicsItemAnimation>
void Animation(int Duree, QString DebutX, QString DebutY, QString FinX, QString FinY, QObject* item);
// Dans SousProg.h
void Mouvement(Joueur* joueur, int equipe, QKeyEvent* event);
#endif // SOUSPROG_H
