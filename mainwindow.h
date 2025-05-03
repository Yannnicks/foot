#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QKeyEvent>  // Pour capturer les événements de touches
#include "mygraphicsview.h"
class Joueur;  // Déclaration anticipée de la classe Joueur
class Balle;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Joueur *player1;  // Déclaration de player1
    Joueur *player2;  // Déclaration de player2
public slots:
    void updateCoordinates();

};


#endif // MAINWINDOW_H
