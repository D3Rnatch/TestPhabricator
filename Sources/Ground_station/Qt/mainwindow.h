#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//Adresse Serveur : 10.5.133.185

#include <QtGui>
#include <QtNetwork>
#include <QMainWindow>
#include <QDialog>
#include "ui_mainwindow.h"
#include <QDebug>
#include <QLineEdit>
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>


class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);



    private slots:
        void quitterApp();
        void connexion();
        void on_boutonEnvoyer_clicked();
        void donneesRecues();
        void connecte();
        void deconnecte();
        void erreurSocket(QAbstractSocket::SocketError erreur);
        void deconnexion();
        void envoie_arret_urgence();
        void parser();

    private:
        QTcpSocket *socket; // Représente le serveur
        quint16 tailleMessage;
        Ui::MainWindow *ui;
        bool ok;


};


#endif // MAINWINDOW_H













