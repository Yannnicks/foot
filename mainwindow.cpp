#include "mainwindow.h"
#include "SousProg.h"
#include "ui_mainwindow.h"
#include "mygraphicsview.h"
#include "Joueur.h"
#include "Balle.h"
#include "cages.h"
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
    scene = new QGraphicsScene(-400, -300, 800, 600, this);
    // Ajouter une belle cage à gauche
    Cage *cageGauche = new Cage(-530, -50, 50, 100);
    scene->addItem(cageGauche);

    // Ajouter une belle cage à droite
    Cage *cageDroite = new Cage(480, -50, 50, 100);
    scene->addItem(cageDroite);


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
    customView->ballon = ballon;


    // Remplace l'ancien widget dans l'UI par le nouveau MyGraphicsView
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFocus(); // Donne le focus à la vue

    // Création d'un timer pour mise à jour
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateCoordinates);
    timer->start(1);

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
    int BallonX=(ballon->x());
    int BallonY=(ballon->y());
    int Score=0;
    if (ballon && ui->labelBallon) {
        ui->labelBallon->setText("Ballon: (" + QString::number(ballon->x()) + ", " + QString::number(ballon->y()) + ")");
        }
    if (455<BallonX &&BallonX<500&&-50<BallonY&&BallonY<45){
            Score=Score+1;
       ui->lcdNumber->display(Score);
            QTimer::singleShot(2000, this, [=]() {
           ballon->setPos(10, 10);
           qDebug() << "2 secondes sont passées.";
            });
    }

}
