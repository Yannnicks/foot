#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QKeyEvent>  // Pour capturer les événements de touches
#include "mygraphicsview.h"
class Joueur;  // Déclaration anticipée de la classe Joueur
class Balle;
class Terrainitem;

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
    QGraphicsScene *scene;
    Joueur* player[20];
    Balle* ballon;
    Ui::MainWindow *ui;


public slots:
    void updateCoordinates();
};


#endif // MAINWINDOW_H
