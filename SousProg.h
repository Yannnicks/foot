#ifndef SOUSPROG_H
#define SOUSPROG_H
#include <joueur.h>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QMainWindow>
#include <QGraphicsItemAnimation>
void Animation(int Duree,int DebutX,int DebutY,int FinX,int FinY,Joueur* Joueur1);
// Dans SousProg.h
void Mouvement(Joueur* joueur, int equipe, QKeyEvent* event);
#endif // SOUSPROG_H
