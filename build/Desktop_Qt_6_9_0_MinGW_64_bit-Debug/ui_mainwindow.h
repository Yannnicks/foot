/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *labelBallon;
    QLabel *labelPlayer2;
    QLabel *labelPlayer1;
    QLCDNumber *lcdNumber_2;
    QLCDNumber *lcdNumber;
    QGraphicsView *terrainView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1600, 1000);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(30, 0, 1551, 51));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        labelBallon = new QLabel(horizontalLayoutWidget);
        labelBallon->setObjectName("labelBallon");

        horizontalLayout->addWidget(labelBallon);

        labelPlayer2 = new QLabel(horizontalLayoutWidget);
        labelPlayer2->setObjectName("labelPlayer2");

        horizontalLayout->addWidget(labelPlayer2);

        labelPlayer1 = new QLabel(horizontalLayoutWidget);
        labelPlayer1->setObjectName("labelPlayer1");

        horizontalLayout->addWidget(labelPlayer1);

        lcdNumber_2 = new QLCDNumber(horizontalLayoutWidget);
        lcdNumber_2->setObjectName("lcdNumber_2");
        lcdNumber_2->setDigitCount(5);
        lcdNumber_2->setProperty("value", QVariant(1.000000000000000));

        horizontalLayout->addWidget(lcdNumber_2);

        lcdNumber = new QLCDNumber(horizontalLayoutWidget);
        lcdNumber->setObjectName("lcdNumber");
        lcdNumber->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(0, 0, 0);"));
        lcdNumber->setProperty("intValue", QVariant(3));

        horizontalLayout->addWidget(lcdNumber);

        lcdNumber->raise();
        lcdNumber_2->raise();
        labelPlayer1->raise();
        labelPlayer2->raise();
        labelBallon->raise();
        terrainView = new QGraphicsView(centralwidget);
        terrainView->setObjectName("terrainView");
        terrainView->setGeometry(QRect(1, 121, 2121, 881));
        terrainView->setMinimumSize(QSize(999, 800));
        terrainView->setMaximumSize(QSize(1200000, 12000000));
        terrainView->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContentsOnFirstShow);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "JeuFoot", nullptr));
        labelBallon->setText(QCoreApplication::translate("MainWindow", "labelBall", nullptr));
        labelPlayer2->setText(QCoreApplication::translate("MainWindow", "labelPlay", nullptr));
        labelPlayer1->setText(QCoreApplication::translate("MainWindow", "labelPlayer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
