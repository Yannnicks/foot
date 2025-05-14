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
#include <terrainitem.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->terrainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->terrainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->terrainView->setDragMode(QGraphicsView::ScrollHandDrag); // Optionnel pour draguer à la souris
    ui->terrainView->setMinimumSize(1200, 800);
    ui->terrainView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(-100, -100, 2000, 1400);


    scene->addItem(new TerrainItem());

    ui->terrainView->setScene(scene);
    ui->terrainView->setRenderHint(QPainter::Antialiasing);
    // ❌ Ne fais PAS fitInView
    // ❌ PAS de resizeEvent qui refait fitInView

  int JoueurX[20] = {
        150, 150, 150, 150,   // Gardiens / Défenseurs gauche
        300, 300, 300,        // Milieux gauche
        450, 450, 450,        // Attaquants gauche

        1450, 1450, 1450, 1450, // Gardiens / Défenseurs droite
        1300, 1300, 1300,       // Milieux droite
        1150, 1150, 1150        // Attaquants droite
    };

    int JoueurY[20] = {
        150, 350, 700, 750,   // Gardiens / Défenseurs gauche
        250, 500, 750,        // Milieux gauche
        200, 500, 800,        // Attaquants gauche

        150, 350, 700, 750,   // Gardiens / Défenseurs droite
        250, 500, 750,        // Milieux droite
        200, 500, 800         // Attaquants droite
    };

    int balleX = 680, balleY = 500; // Centre du terrain (1600 / 2, 1000 / 2)



    // Créer la scène (TAILLE TERRAIN)

    // Créer les joueurs
    for (int i = 0; i < 20; ++i) {
        player[i] = new Joueur(JoueurX[i], JoueurY[i], 15, 15);
        if (i < 10) {
            player[i]->setBrush(Qt::blue);
            player[i]->setTeam(1);
                // Joueurs du FC Barcelone
                static const QStringList nomsBarca = {
                    "Balde", "Christensen", "Araujo", "Kounde",
                    "Gündogan", "De Jong", "Pedri",
                    "Felix", "Lewandowski", "Yamal"
                };
                static const QVector<int> numerosBarca = {
                    5, 15, 4, 3,
                    22, 21, 8,
                    14, 9, 11
                };
                player[i]->setNom(nomsBarca[i]);
                player[i]->setNumero(numerosBarca[i]);

                ;
        } else {
            player[i]->setBrush(Qt::red);
            player[i]->setTeam(2);

            // Joueurs du Real Madrid
            static const QStringList nomsReal = {
                "Carvajal", "Rüdiger", "Nacho", "Mendy",
                "Kroos", "Valverde", "Camavinga",
                "Rodrygo", "Vinicius", "Bellingham"
            };
            static const QVector<int> numerosReal = {
                2, 22, 6, 23,
                8, 15, 12,
                11, 7, 5
            };
            int j = i - 10; // index de 0 à 9 pour Real Madrid
            player[i]->setNom(nomsReal[j]);
            player[i]->setNumero(numerosReal[j]);
        }
        scene->addItem(player[i]);
    }
    player[0]->poste = LateralGauche;
    player[3]->poste = LateralDroit;
    player[2]->poste = Defenseur;
    player[1]->poste = Defenseur;
    player[4]->poste = Milieu;
    player[5]->poste = Milieu;
    player[6]->poste = Milieu;
    player[9]->poste = AilierGauche;
    player[7]->poste = AilierDroit;
    player[8]->poste = AttaquantCentral;
    player[13]->poste = LateralGauche;
    player[10]->poste = LateralDroit;
    player[12]->poste = Defenseur;
    player[11]->poste = Defenseur;
    player[14]->poste = Milieu;
    player[15]->poste = Milieu;
    player[16]->poste = Milieu;
    player[19]->poste = AilierGauche;
    player[17]->poste = AilierDroit;
    player[18]->poste = AttaquantCentral;

    // Créer la balle
    ballon = new Balle(balleX, balleY, 7.5, 7.5);
    scene->addItem(ballon);
    MyGraphicsView *customView = new MyGraphicsView(this);
    customView->setScene(scene);


    // Donner la balle et les joueurs au customView
    for (int i = 0; i < 20; ++i) {
        customView->player[i] = player[i];
    }
    customView->ballon = ballon;


    // Timer pour mise à jour
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this,&MainWindow::updateCoordinates);
    timer->start(16); // environ 60 FPS


    updateCoordinates();



    // Mettre la scène dans l'UI
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCoordinates()
{QPointF move(0, 0); // Ou ton vrai déplacement voulu

    for (int i = 0; i < 20; ++i) {
        player[i]->moveBy(move.x(), move.y()); // Bouge
        // Dribbles possibles
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 20; ++j) {
                if (i != j)
                    player[i]->dribbleAgainst(player[j]);
            }
        }

        player[i]->checkBoundaries(); // Vérifie limites
    }

    ballon->moveBy(move.x(), move.y()); // Bouge la balle
    ballon->checkBoundaries(); // Vérifie limites

    if (ui->labelPlayer1 && player[0])
        ui->labelPlayer1->setText("Joueur 1: (" + QString::number(player[0]->x()) + ", " + QString::number(player[0]->y()) + ")");

    if (ui->labelPlayer2 && player[1])
        ui->labelPlayer2->setText("Joueur 2: (" + QString::number(player[1]->x()) + ", " + QString::number(player[1]->y()) + ")");

    if (!ballon) return; // sécurité

    int BallonX = ballon->x();
    int BallonY = ballon->y();
    static int Score = 0; // static pour garder la valeur

    if (ui->labelBallon)
        ui->labelBallon->setText("Ballon: (" + QString::number(ballon->x()) + ", " + QString::number(ballon->y()) + ")");
ui->terrainView->centerOn(ballon);
    // But détecté
    if (850 < BallonX && BallonX < 880 && -50 < BallonY && BallonY < 45) {
        QTimer::singleShot(2000, this, [=]() {
            ballon->setPos(10, 10);
            Score++;
            ui->lcdNumber->display(Score);
            qDebug() << "2 secondes sont passées.";
        });
    }

}


