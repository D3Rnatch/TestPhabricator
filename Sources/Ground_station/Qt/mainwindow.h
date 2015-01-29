#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//Adresse Serveur : 10.5.133.185

#include "libraries.h"
#include "map.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);

         Ui::MainWindow *ui;

        void CreerLog();
        void qInstallMsgHandler();

        float getJoystick_x(){return joystick_x;}
        float getJoystick_y(){return joystick_y;}
        float getJoystick_t(){return joystick_t;}

        void setJoystick_x(float x){joystick_x=x;}
        void setJoystick_y(float y){joystick_y=y;}
        void setJoystick_t(float t){joystick_t=t;}

         int getCoord_X(){return Coord_X;}
         int getCoord_Y(){return Coord_Y;}
         int getR_value(){return joystick_t;}

         void setCoord_X(float x){Coord_X=x;}
         void setCoord_Y(float y){Coord_Y=y;}
         void setR_value(float t){R_value_1=t;}


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
        void envoie_coordonnees();
        void stopTimer();


        void joystick();
        //void pause();

    signals:
        void signal_parse();
        void signal_ajoutLogs();
        void signal_recup_coord_ok();
        void signal_deconnecte();

        void signal_sdl_mode_manuel();
        void signal_sdl_quitter();
        void signal_sdl_mode_auto();


    private:
        QTcpSocket *socket; // Représente le serveur
        quint16 tailleMessage;

        Map *carte;

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

        float x;
        float y;
        float t;

        int x_old;
        int y_old;

        int Coord_X;
        int Coord_Y;
        int R_value_1;

        QTimer *timer1 ;

};

#endif // MAINWINDOW_H
