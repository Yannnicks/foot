/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "mygraphicsview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    MyGraphicsView *graphicsView;
    QFrame *line;
    QLCDNumber *lcdNumber;
    QLabel *labelPlayer1;
    QLabel *labelPlayer2;
    QLabel *labelBallon;
    QLCDNumber *lcdNumber_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1772, 892);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        graphicsView = new MyGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(9, 50, 1071, 490));
        graphicsView->setMaximumSize(QSize(1100, 500));
        graphicsView->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(34,139, 34);"));
        graphicsView->setFrameShape(QFrame::Shape::Box);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setGeometry(QRect(430, 60, 20, 501));
        line->setFrameShape(QFrame::Shape::VLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);
        lcdNumber = new QLCDNumber(centralwidget);
        lcdNumber->setObjectName("lcdNumber");
        lcdNumber->setGeometry(QRect(420, 90, 64, 23));
        lcdNumber->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(0, 0, 0);"));
        lcdNumber->setProperty("intValue", QVariant(3));
        labelPlayer1 = new QLabel(centralwidget);
        labelPlayer1->setObjectName("labelPlayer1");
        labelPlayer1->setGeometry(QRect(20, 160, 151, 20));
        labelPlayer2 = new QLabel(centralwidget);
        labelPlayer2->setObjectName("labelPlayer2");
        labelPlayer2->setGeometry(QRect(10, 70, 141, 20));
        labelBallon = new QLabel(centralwidget);
        labelBallon->setObjectName("labelBallon");
        labelBallon->setGeometry(QRect(20, 110, 131, 20));
        lcdNumber_2 = new QLCDNumber(centralwidget);
        lcdNumber_2->setObjectName("lcdNumber_2");
        lcdNumber_2->setGeometry(QRect(200, 90, 64, 23));
        lcdNumber_2->setDigitCount(5);
        lcdNumber_2->setProperty("value", QVariant(1.000000000000000));
        MainWindow->setCentralWidget(centralwidget);
        line->raise();
        graphicsView->raise();
        lcdNumber->raise();
        labelPlayer1->raise();
        labelPlayer2->raise();
        labelBallon->raise();
        lcdNumber_2->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1772, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "JeuFoot", nullptr));
        labelPlayer1->setText(QCoreApplication::translate("MainWindow", "labelPlayer", nullptr));
        labelPlayer2->setText(QCoreApplication::translate("MainWindow", "labelPlay", nullptr));
        labelBallon->setText(QCoreApplication::translate("MainWindow", "labelBall", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
