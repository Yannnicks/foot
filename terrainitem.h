#pragma once

#include <QGraphicsItem>
#include <QObject>        // <-- AJOUT IMPORTANT
#include <QTimer>

class TerrainItem : public QObject, public QGraphicsItem  // <-- hériter de QObject ET QGraphicsItem
{
    Q_OBJECT             // <-- nécessaire pour que connect() fonctionne
public:
    TerrainItem();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool filetSecoue = false;
    qreal filetOscillation = 0;
};
