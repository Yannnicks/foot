#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Joueur.h"
#include "SousProg.h"
#include "Balle.h"
#include <QGraphicsScene>
#include <QKeyEvent>  // Inclure pour capturer les événements de touches

int posXj1 = -100, posXj2 = 100, posXBallon = 0, posYballon = 0, posYj1 = 0, posYj2 = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    // Premier joueur
    player1 = new Joueur(posXj1, posYj1, 30, 30);
    player1->setBrush(QBrush(Qt::blue));
    scene->addItem(player1);

    // Deuxième joueur
    player2 = new Joueur(posXj2, posYj2, 30, 30);
    player2->setBrush(QBrush(Qt::red));
    scene->addItem(player2);

    // Assurer que les deux joueurs sont focusables et peuvent recevoir les événements
    player1->setFocus();
    player2->setFocus();

    // Ajouter la balle à la scène
    Balle *ballon = new Balle(posXBallon, posYballon, 10, 10);
    scene->addItem(ballon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Redéfinition de keyPressEvent pour gérer les touches
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Appeler la fonction Mouvement pour chaque joueur
    player1->keyPressEvent(event);  // Déplacer le joueur 1
    player2->keyPressEvent(event);  // Déplacer le joueur 2
}
