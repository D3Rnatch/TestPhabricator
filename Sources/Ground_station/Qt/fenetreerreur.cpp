#include "mainwindow.h"

#include <QDebug>


FenetreErreur::FenetreErreur() : QWidget()
{

    //taille de la fenêtre
    setFixedSize(300,200);

    //construction du bouton recommencer
    bouton_recommencer = new QPushButton("Recommencer",this);

    bouton_recommencer->setFont(QFont("Arial",9));
    bouton_recommencer->setCursor(Qt::PointingHandCursor);
    bouton_recommencer->move(30,140);

    //construction du bouton annuler
    bouton_annuler = new QPushButton("Annuler",this);

    bouton_annuler->setFont(QFont("Arial",9));
    bouton_annuler->setCursor(Qt::PointingHandCursor);
    bouton_annuler->move(170,140);

    QObject::connect(bouton_recommencer, SIGNAL(clicked()),this, SLOT(reconnexion()));
    QObject::connect(bouton_annuler, SIGNAL(clicked()), qApp, SLOT(quit()));

}

void FenetreErreur::reconnexion()
{
   qDebug()<< "nouvelle tentative de connexion";

}
