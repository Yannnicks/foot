#include "TerrainItem.h"
#include <QPainter>
#include <QTimer>
#include <QtMath> // pour sin()

TerrainItem::TerrainItem()
{
    // Timer pour animer les filets
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        if (filetSecoue) {
            filetOscillation += 0.5;
            if (filetOscillation > 5.0) {
                filetOscillation = 0;
                filetSecoue = false;
            }
            update(); // Redessine
        }
    });
    timer->start(30); // toutes les 30ms
}

QRectF TerrainItem::boundingRect() const
{
    return QRectF(0, 0, 1200, 800); // Grand terrain
}

void TerrainItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Fond gazon
    painter->setBrush(QBrush(Qt::green));
    painter->drawRect(boundingRect());

    // Bandes alternées
    for (int i = 0; i < 10; ++i) {
        if (i % 2 == 0) {
            painter->setBrush(QColor(0, 170, 0));
        } else {
            painter->setBrush(QColor(0, 140, 0));
        }
        painter->drawRect(i * 160, 0, 160, 1000);
    }

    QPen whitePen(Qt::white);
    whitePen.setWidth(4);
    painter->setPen(whitePen);

    // Bordures
    painter->drawRect(0, 0, 1600, 1000);

    // Ligne médiane
    painter->drawLine(800, 0, 800, 1000);

    // Cercle central
    painter->drawEllipse(700, 400, 200, 200);
    painter->drawEllipse(795, 495, 10, 10);

    // Surfaces de réparation
    // Gauche
    painter->drawRect(0, 320, 180, 360);
    painter->drawRect(0, 400, 60, 200);
    painter->drawArc(QRectF(130, 400, 140, 200), 90 * 16, 180 * 16);

    // Droite
    painter->drawRect(1420, 320, 180, 360);
    painter->drawRect(1540, 400, 60, 200);
    painter->drawArc(QRectF(1330, 400, 140, 200), 270 * 16, 180 * 16);

    // Points de penalty
    painter->drawEllipse(120, 495, 10, 10);
    painter->drawEllipse(1470, 495, 10, 10);

    // Arcs de corners
    int r = 30;
    painter->drawArc(QRectF(0, 0, r, r), 0 * 16, 90 * 16);
    painter->drawArc(QRectF(1570, 0, r, r), 90 * 16, 90 * 16);
    painter->drawArc(QRectF(0, 970, r, r), 270 * 16, 90 * 16);
    painter->drawArc(QRectF(1570, 970, r, r), 180 * 16, 90 * 16);

    // ---------- CAGES + FILETS + OMBRES ----------

    int cageDepth = 50; // Profondeur 3D de la cage
    int cageHeight = 120;
    int cageY = 440;

    QPen cagePen(QColor(200, 200, 200)); // Gris clair pour poteaux
    cagePen.setWidth(4);
    painter->setPen(cagePen);
    painter->setBrush(Qt::NoBrush);

    // --- Cage gauche ---
    // Poteaux
    painter->drawLine(0, cageY, -cageDepth, cageY);
    painter->drawLine(0, cageY + cageHeight, -cageDepth, cageY + cageHeight);
    // Haut de la cage
    painter->drawLine(-cageDepth, cageY, -cageDepth, cageY + cageHeight);

    // Filet verticals
    QPen filetPen(Qt::white);
    filetPen.setStyle(Qt::DotLine);
    painter->setPen(filetPen);
    for (int i = 0; i <= 5; ++i) {
        int x1 = -i * (cageDepth / 5) + (filetSecoue ? qSin(filetOscillation + i) * 2 : 0);
        painter->drawLine(x1, cageY, x1, cageY + cageHeight);
    }

    // Ombre au sol
    QBrush ombreBrush(QColor(50, 50, 50, 100));
    painter->setBrush(ombreBrush);
    painter->setPen(Qt::NoPen);
    painter->drawRect(-cageDepth, cageY + cageHeight, cageDepth, 10);

    // --- Cage droite ---
    painter->setPen(cagePen);
    painter->setBrush(Qt::NoBrush);
    painter->drawLine(1600, cageY, 1600 + cageDepth, cageY);
    painter->drawLine(1600, cageY + cageHeight, 1600 + cageDepth, cageY + cageHeight);
    painter->drawLine(1600 + cageDepth, cageY, 1600 + cageDepth, cageY + cageHeight);

    // Filet verticals
    painter->setPen(filetPen);
    for (int i = 0; i <= 5; ++i) {
        int x1 = 1600 + i * (cageDepth / 5) + (filetSecoue ? qSin(filetOscillation + i) * 2 : 0);
        painter->drawLine(x1, cageY, x1, cageY + cageHeight);
    }

    // Ombre au sol
    painter->setBrush(ombreBrush);
    painter->setPen(Qt::NoPen);
    painter->drawRect(1600, cageY + cageHeight, cageDepth, 10);
}
