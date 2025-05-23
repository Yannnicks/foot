QT       += core gui \
    quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Balle.cpp \
    SousProg.cpp \
    cages.cpp \
    joueur.cpp \
    main.cpp \
    mainwindow.cpp \
    mygraphicsview.cpp \
    terrainitem.cpp

HEADERS += \
    Balle.h \
    SousProg.h \
    cages.h \
    joueur.h \
    mainwindow.h \
    mygraphicsview.h \
    terrainitem.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    jeufoot_fr_FR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
