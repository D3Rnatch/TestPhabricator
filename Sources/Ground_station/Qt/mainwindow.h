#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//Adresse Serveur : 10.5.133.185

#include "libraries.h"


class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);

        void CreerLog();
        void qInstallMsgHandler();

        float getJoystick_x(){return joystick_x;}
        float getJoystick_y(){return joystick_y;}
        float getJoystick_t(){return joystick_t;}

        void setJoystick_x(float x){joystick_x=x;}
        void setJoystick_y(float y){joystick_y=y;}
        void setJoystick_t(float t){joystick_t=t;}


    public slots:
        void quitterApp();
        void connexion();
        void on_boutonEnvoyer_clicked();
        void donneesRecues();
        void connecte();
        void deconnecte();
        void erreurSocket(QAbstractSocket::SocketError erreur);
        void deconnexion();
        void envoie_arret_urgence();
        void messageErreur();
        void envoie_message_hello();
        void stop();
        void parserReception();
        void envoieModeAuto();
        void envoieModeManuel();
        void creationLogs();
        void ajoutLogs();
        void recuperationCoordonnees();
        void timer();


    signals:
        void signal_parse();
        void signal_ajoutLogs();
        void signal_recup_coord_ok();


    private:
        QTcpSocket *socket; // Représente le serveur
        quint16 tailleMessage;
        Ui::MainWindow *ui;
        bool ok;
        QString messageRecu;
        QString message_1;
        QString message_2;
        int batterie_index_1;
        int batterie_index_2;
        QFile file;
        QString messageLogs;
        QString nomLog;
        QTime heure;
        int joystick_x;
        int joystick_y;
        int joystick_t;

};

#endif // MAINWINDOW_H
