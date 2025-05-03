#include "mainwindow.h"
#include "SousProg.h"
#include "ui_mainwindow.h"
#include "mygraphicsview.h"
#include "Joueur.h"
#include "Balle.h"
#include <QLabel>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation des coordonnées
    int Joueur2X = 100, Joueur1X = -100, Joueur1Y = 0, Joueur2Y = 0, balleX = 10, balleY = 10;

    // Création de la scène
    scene = new QGraphicsScene(this);

    // Création des joueurs
    player1 = new Joueur(Joueur1X, Joueur1Y, 30, 30);
    player1->setBrush(QBrush(Qt::blue));
    scene->addItem(player1);

    player2 = new Joueur(Joueur2X, Joueur2Y, 30, 30);
    player2->setBrush(QBrush(Qt::red));
    scene->addItem(player2);

    // Création de la balle
    Balle *ballon = new Balle(balleX, balleY, 10, 10);
    scene->addItem(ballon);

    // Remplace le QGraphicsView standard par MyGraphicsView
    MyGraphicsView *customView = new MyGraphicsView(this);
    customView->setScene(scene);
    customView->player1 = player1;
    customView->player2 = player2;

    // Remplace l'ancien widget dans l'UI par le nouveau MyGraphicsView
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFocus(); // Donne le focus à la vue

    // Création d'un timer pour mise à jour
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateCoordinates);
    timer->start(10);

    // Mise à jour initiale
    updateCoordinates();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCoordinates()
{
    if (ui->labelPlayer1)
        ui->labelPlayer1->setText("Joueur 1: (" + QString::number(player1->x()) + ", " + QString::number(player1->y()) + ")");
    if (ui->labelPlayer2)
        ui->labelPlayer2->setText("Joueur 2: (" + QString::number(player2->x()) + ", " + QString::number(player2->y()) + ")");

    // Trouver la balle
    Balle *ballon = nullptr;
    for (QGraphicsItem *item : scene->items()) {
        ballon = dynamic_cast<Balle*>(item);
        if (ballon) break;
    }

    if (ballon && ui->labelBallon) {
        ui->labelBallon->setText("Ballon: (" + QString::number(ballon->x()) + ", " + QString::number(ballon->y()) + ")");

        }
    /*int CollisionJoueur=player1->x()+20;
    if (ballon->x() == CollisionJoueur) {
        int newX = ballon->x() + 200;
        Animation(700,QString::number(ballon->x()),QString::number(ballon->y()),QString::number(newX),QString::number(ballon->y()),ballon);}

*/
}
