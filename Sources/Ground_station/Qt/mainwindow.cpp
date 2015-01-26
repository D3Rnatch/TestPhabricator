//Adresse Serveur : 10.5.133.185

#include "libraries.h"
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui (new Ui::MainWindow)
{
    setupUi(this); //initialision de l'IHM

//----------------------------------NETWORK---------------------------------------------

    socket = new QTcpSocket(this);//Création du socket

    connect(socket, SIGNAL(readyRead()),this, SLOT(donneesRecues()));//Lancement de la reception de données Robot
    connect(socket, SIGNAL(connected()),this, SLOT(connecte()));//Lancement de la connexion
    connect(socket, SIGNAL(connected()),this, SLOT(envoie_message_hello()));//On envoie le message "hello" pour obtenir les données du Robot
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte()));//Lancement de la deconnexion
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));//Traitement des erreurs du socket
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(messageErreur()));//Lancement de la Fenêtre Erreur


    tailleMessage = 0;

 //----------------------------------JSON--------------------------------------------------

    QObject::connect(this,SIGNAL(signal_parse()),this,SLOT(parserReception()));//Lancement du parse automatique après reception de données

//------------------------------------MENU----------------------------------------------------

    //Action Fermer
   QObject::connect(actionFermer, SIGNAL(triggered()), this, SLOT(quitterApp()));

    //Action Se Connecter
   QObject::connect(actionSe_connecter_au_Drone, SIGNAL(triggered()),this, SLOT(connexion()));

    //Action Se Deconnecter
   QObject::connect(action_se_deconnecter, SIGNAL(triggered()),this, SLOT(deconnexion()));

    //Action Arrêt d'urgence
   QObject::connect(actionArret_d_urgence, SIGNAL(triggered()),this, SLOT(envoie_arret_urgence()));

   //Bouton Stop
   QObject::connect(boutonStop, SIGNAL(clicked()),this, SLOT(stop()));


   //Gestion de la batterie des hélices latérales
   progressBarBatterieFans->setValue(batterie_index_1);

   //Gestion de la batterie de l'hélice centrale
   progressBarBatterieLevitation->setValue(batterie_index_2);

   //Mode Automatique
   QObject::connect(boutonAuto,SIGNAL(clicked()),this, SLOT(envoieModeAuto()));

   //Mode Manuel
   QObject::connect(boutonManuel,SIGNAL(clicked()),this, SLOT(envoieModeManuel()));

   //--------------------------------------LOGS-----------------------------------------------

   QObject::connect(boutonLog,SIGNAL(clicked()),this,SLOT(creationLogs()));//tests sur la creation de logs
   QObject::connect(this,SIGNAL(signal_ajoutLogs()),this,SLOT(ajoutLogs()));//tests sur l'ajout de logs

   //------------------------------INTERFACE JOYSTICK/SERVEUR-------------------------

   qDebug()<<"\n X: "<<joystick_x <<" Y: "<< joystick_y <<" T: "<< joystick_t <<"\n";

   //----------------------------------COORDONNEES DRONE----------------------------------

     QObject::connect(bouton_coord_refresh,SIGNAL(clicked()),this,SLOT(recuperationCoordonnees())); //Rafraichissement Coordonnées par le bouton

       qDebug()<<"\n initialisation de x y t : "<<joystick_x << joystick_y <<joystick_t <<"\n";
}


//Ce Slot est envoyé lorsque'on clique sur Fermer
void MainWindow::quitterApp()
{
    qDebug()<<"\nquitter App() : exit app\n";

    //Emission des logs
    messageLogs = QDateTime::currentDateTime().toString()+" : Application Fermée \r\n ";
    emit signal_ajoutLogs();

    exit(0);
}


//Ce slot est envoyé lorsqu'on clique sur Se Connecter
void MainWindow::connexion()
{

    socket->abort();//On désactive les connexions précédentes s'il y en a
    socket->connectToHost(adr_ip->text(), numero_port_serveur->value());//On se connecte au serveur demandé

    //Emission des logs
    messageLogs = QDateTime::currentDateTime().toString()+" : connexion au serveur "+adr_ip->text()+"...\r\n";
    emit signal_ajoutLogs();

}

// Ce slot est appelé lorsque la connexion au serveur a réussi
 void MainWindow::connecte()
{
      //On affiche l'état de la connexion
     etat_connexion->setText("Connecté");

     //On nettoie la zone de reception
     message_recu->clear();

     //On nettoie la zone de reception
     message_envoie->clear();

     //Emission des logs
     messageLogs = QDateTime::currentDateTime().toString()+" : connecté au serveur "+adr_ip->text()+"\r\n";
     emit signal_ajoutLogs();
 }


//Envoie d'un messsage au serveur
void MainWindow::on_boutonEnvoyer_clicked()
{
    message_envoie->clear();

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly); //Message a envoyer. Nom de l'auteur et le texte la meme string

    //On prépare le paquet à envoyer

    QString messageAEnvoyer = "{\"robot\":{\"X\":"+ QString(joystick_x) +",\"Y\":"+ QString(joystick_y) +",\"T\":"+ QString(joystick_t) +"},\"message\":{\"type\":\"system\",\"content\":\"hello\"}} ";


    int envoie = socket->write(messageAEnvoyer.toStdString().c_str()); // On envoie le paquet convertie

    //on afiche le message envoyé dans la zone d'Emission Texte
    message_envoie->setText(messageAEnvoyer);

    if(envoie == -1)
        QMessageBox::critical(this,"Erreur","Message non envoyé");
}


//Reception d'un message provenant du serveur
void MainWindow::donneesRecues()
{
    //Reception du Message
    QDataStream in(socket);
    int size = socket->bytesAvailable();
    char* temp = new char[size];
    int buff_size = in.readRawData(temp, size);

    //Affichage dans la console du Message reçu
   // qDebug()<<temp;
   // qDebug()<<socket->bytesAvailable();

    //conversion du message reçu en QString
    QString toto(temp);

    messageRecu = toto;

    //On affiche le message sur la zone de Reception texte
    message_recu->setText(messageRecu);

    //On remet la taille du message à 0 pour pouvoir recevoir de futurs messages
    tailleMessage = 0;

    qDebug()<<"\nreturn donnees recues messageRecu :\n"<<messageRecu<<"\n";

    //Emission des logs
    messageLogs = QDateTime::currentDateTime().toString()+" : Reception de la trame : "+messageRecu+"+\r\n";
    emit signal_ajoutLogs();
    QStringList list = QString(messageRecu).split(" ");
    message_1 = QString(list.at(0));
    list.removeFirst();
    message_2 = list.join(QString(" "));


   qDebug()<<"\nmessage 1 :\n"<<message_1<<"\n";
   qDebug()<<"\nmessage 2 :\n"<<message_2<<"\n";

    emit signal_parse();//Lancement du signal d'execution du parse
}

//Ce slot demande le déconexion du serveur
void MainWindow::deconnexion()
{
    socket->close();

    //On affiche l'information Deconnecté
    etat_connexion->setText("Déconnecté");

    //On nettoie la zone de reception
    message_envoie->clear();

    //On nettoie la zone de reception
    message_recu->clear();

    qDebug() << "\nDeconnexion\n";

}


//Ce slot est appelé lorsqu'on est déconnecté du serveur
 void MainWindow::deconnecte()
 {
      //On affiche l'état de la connexion
     etat_connexion->setText("Déconnecté");

      //On nettoie la zone de reception
     message_envoie->clear();

      //On nettoie la zone de reception
     message_recu->clear();

     //Emission des logs
     messageLogs = QDateTime::currentDateTime().toString()+" : déconnecté du serveur "+adr_ip->text()+"\r\n";
     emit signal_ajoutLogs();
 }



// Ce slot est appelé lorsqu'il y a une erreur
void MainWindow::erreurSocket(QAbstractSocket::SocketError problem)
{
    // On affiche un message différent selon l'erreur qu'on nous indique
    switch(problem)
     {
         case QAbstractSocket::HostNotFoundError:
            qDebug()<<"ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.";
            break;

         case QAbstractSocket::ConnectionRefusedError:
           qDebug()<<"ERREUR : le serveur a refusé la connexion. Vérifiez si le programme \"serveur\" a bien été lancé. Vérifiez aussi l'IP et le port.";
           break;

         case QAbstractSocket::RemoteHostClosedError:
            qDebug()<<"ERREUR : le serveur a coupé la connexion.";
            break;

          default:
             qDebug()<<"ERREUR : " + socket->errorString();
            break;
     }

    //Emission des logs
    messageLogs = "\n"+QDateTime::currentDateTime().toString()+ " : erreur : "+ socket->errorString()+" \r\n";
    emit signal_ajoutLogs();
}




//Ce slot est envoyé des qu'on clique sur le bouton d'arret d'urgence
void MainWindow::envoie_arret_urgence()
{

    QByteArray paquet_urgence;
    QDataStream out(&paquet_urgence, QIODevice::WriteOnly); //Message a envoyer. Nom de l'auteur et le texte la meme string

    //On prépare le paquet à envoyer
    QString messageAEnvoyer_urgence = "{\"robot\":{\"X\":"+ QString(joystick_x) +",\"Y\":"+QString(joystick_y) +",\"T\":"+QString(joystick_t)+"},\"message\":{\"type\":\"system\",\"content\":\"stop\"}} ";

    // On envoie le paquet convertie
    int envoie = socket->write(messageAEnvoyer_urgence.toStdString().c_str());

    //on afiche le message envoyé dans la zone d'Emission Texte
    message_envoie->setText(messageAEnvoyer_urgence);

    //On nettoie la zone de reception
    message_recu->clear();

    if(envoie == -1)
        QMessageBox::critical(this,"Erreur","Message non envoyé");

    //Emission des logs
    messageLogs = QDateTime::currentDateTime().toString()+" : arrêt d'urgence enclenché !\r\n";
    emit signal_ajoutLogs();

}



//Ce slot est envoyé lorsqu'un message d'erreur de connexion apparait
void MainWindow::messageErreur()
{
    //Lancement de la Pop-Up message d'Erreur
    int reponse = QMessageBox::question(this,"Connexion impossible","Impossible de se connecter au serveur\nRecomencer?");

        if (reponse == QMessageBox::Yes)//Si l'utlisateur désire sereconnecter

                this->connexion();
                qDebug()<<"Nouvelle connexion";

        if (reponse == QMessageBox::No)//Si il ne désire pas se reconnecter

                qDebug()<<"Fermeture de la fenêtre d'Erreur";
}




//Slot envoyé systematiquement à chaque nouvelle connexion
void MainWindow::envoie_message_hello()
{
    message_envoie->clear();

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly); //Message a envoyer. Nom de l'auteur et le texte la meme string

    //On prépare le paquet à envoyer
    QString messageHello = "{\"robot\":{\"X\":"+QString(joystick_x) +",\"Y\":"+ QString(joystick_y) +",\"T\":"+ QString(joystick_t) +"},\"message\":{\"type\":\"system\",\"content\":\"hello\"}} ";


    int envoie = socket->write(messageHello.toStdString().c_str()); // On envoie le paquet converti

    //on afiche le message envoyé dans la zone d'Emission Texte
    message_envoie->setText(messageHello);

    //Test Erreur
    if(envoie == -1)
        QMessageBox::critical(this,"Erreur","Message non envoyé");

    //Emission des logs
    messageLogs = QDateTime::currentDateTime().toString()+" : Emission du message \"Hello\" au serveur "+adr_ip->text()+"\r\n";
    emit signal_ajoutLogs();
}




//Ce slot est envoyé lorsqu'uon appuie sur le bouton Stop
void MainWindow::stop()
{
    qDebug()<<"Bouton Stop cliqué";

    QString messageStop = "{\"robot\":{\"X\":"+ QString(joystick_x) +",\"Y\":"+QString(joystick_y)+",\"T\":"+QString(joystick_t)+"},\"message\":{\"type\":\"system\",\"content\":\"stop\"}} ";

    QByteArray paquet_stop;
    QDataStream out(&paquet_stop, QIODevice::WriteOnly); //Message a envoyer. Nom de l'auteur et le texte la meme string

    //On envoie le paquet convertie
    //socket->write(messageStop.toStdString().c_str());

    //retour de fonction testé dans le "if"
    int envoie = socket->write(messageStop.toStdString().c_str());

    //On afiche le message envoyé dans la zone d'Emission Texte
    message_envoie->setText(messageStop);

    //Test erreur
    if(envoie == -1)
        QMessageBox::critical(this,"Erreur","Message non envoyé");

    //Emission des logs
    messageLogs = QDateTime::currentDateTime().toString()+" : demande d'arrêt du drone \r\n";
    emit signal_ajoutLogs();

}



//Ce slot est envoyer lorsqu'on appuis sur le bouton parser
void MainWindow::parserReception()
{

    qDebug()<<"\nParser Reception";
    qDebug()<<"\nmessage Recu 1 avant parse=\n"<<message_1;
    qDebug()<<"\nmessage Recu 2 avant parse=\n"<<message_2;

    QJsonDocument doc_1 = QJsonDocument::fromJson(message_1.toUtf8());
    QJsonDocument doc_2 = QJsonDocument::fromJson(message_2.toUtf8());

    qDebug()<< doc_1.isNull();
    qDebug()<< doc_2.isNull();

    QJsonObject JsonObj_1= doc_1.object();
    QJsonObject JsonObj_2= doc_2.object();

    //Message 1
    //Informations Coordonnées
    int X_value_1 = JsonObj_1["robot"].toObject()["X"].toInt();
    qDebug() << "\nX="<<X_value_1;
    int Y_value_1 = JsonObj_1["robot"].toObject()["Y"].toInt();
    qDebug() << "\nY="<<Y_value_1;
    int R_value_1 = JsonObj_1["robot"].toObject()["R"].toInt();
    qDebug() << "\nR="<<R_value_1;


    //Informations Batteries
    int batterie_index_1 = JsonObj_1["batteries"].toObject()["batterie"].toInt();
    qDebug() << "\nindex batterie="<<batterie_index_1;
    int batterie_value_1 = JsonObj_1["batteries"].toObject()["value"].toInt();//Inutile car le serveur ne renvoie pas la valeur de la batterie
    qDebug() << "\nvalue_batterie="<<batterie_value_1;

    //Informations angle
    int scaner_angle_1 = JsonObj_1["scaner"].toObject()["angle"].toInt();
    qDebug() << "\nscaner angle ="<<scaner_angle_1;
    int image_in_binary_state_1 = JsonObj_1["scaner"].toObject()["angle"].toInt();
    qDebug() << "\nmessage content="<<image_in_binary_state_1;

    //Informations message
    QString type_of_message_1 = JsonObj_1["message"].toObject()["type"].toString();
    qDebug() << "\ntype of the message="<<type_of_message_1;
    QString message_content_1 = JsonObj_1["message"].toObject()["content"].toString();
    qDebug() << "\nmessage content="<<message_content_1;


    //Message 2
    //Informations Coordonnées
    int X_value_2 = JsonObj_2["robot"].toObject()["X"].toInt();
    qDebug() << "\nX="<<X_value_2;
    int Y_value_2 = JsonObj_2["robot"].toObject()["Y"].toInt();
    qDebug() << "\nY="<<Y_value_2;
    int R_value_2 = JsonObj_2["robot"].toObject()["R"].toInt();
    qDebug() << "\nR="<<R_value_2;

    //Informations Batteries
    int batterie_index_2 = JsonObj_2["robot"].toObject()["batterie"].toInt();
    qDebug() << "\nindex batterie="<<batterie_index_2;
    int batterie_value_2 = JsonObj_2["robot"].toObject()["value"].toInt();
    qDebug() << "\nvalue_batterie="<<batterie_value_2;

    //Informations angle
    int scaner_angle_2 = JsonObj_2["scanner"].toObject()["angle"].toInt();
    qDebug() << "\nscanner angle ="<<scaner_angle_2;
    int image_in_binary_state_2 = JsonObj_2["scanner"].toObject()["angle"].toInt();
    qDebug() << "\nmessage content="<<image_in_binary_state_2;

    //Informations message
    QString type_of_message_2 = JsonObj_2["message"].toObject()["type"].toString();
    qDebug() << "\ntype of the message="<<type_of_message_2;
    QString message_content_2 = JsonObj_2["message"].toObject()["content"].toString();
    qDebug() << "\nmessage content="<<message_content_2;

    //Afichage des données dans l'IHM
    box_message_content_1->setText(message_content_1);
    box_message_content_2->setText(message_content_2);
    box_x->setValue(X_value_1);
    box_y->setValue(Y_value_1);
    box_t->setValue(joystick_t);
    box_r->setValue(R_value_1);
}


//Ce slot est envoyé lorsqu'on clique sur le bouton Manuel
void MainWindow::envoieModeAuto()
{
    qDebug()<<"Bouton Auto cliqué";
    QString messageAuto = "{\"message\":{\"type\":\"set_ai\",\"content\":\"auto\"}} ";

    QByteArray paquet_auto;
    QDataStream out(&paquet_auto, QIODevice::WriteOnly); //Message a envoyer. Nom de l'auteur et le texte la meme string

    //retour de fonction testé dans le "if"
    int envoie = socket->write(messageAuto.toStdString().c_str());

    //On afiche le message envoyé dans la zone d'Emission Texte
    message_envoie->setText(messageAuto);

    //Test erreur
    if(envoie == -1)
        QMessageBox::critical(this,"Erreur","Message non envoyé");

    //Emission des logs
    messageLogs = QDateTime::currentDateTime().toString()+" : mode automatique enclenché \r\n";
    emit signal_ajoutLogs();
}



//Ce slot est envoyé lorsqu'on clique sur le bouton Manuel
void MainWindow::envoieModeManuel()
{
    qDebug()<<"Bouton Manuel cliqué";
    QString messageManuel = "{\"message\":{\"type\":\"set_ai\",\"content\":\"manual\"}} ";

    QByteArray paquet_manuel;
    QDataStream out(&paquet_manuel, QIODevice::WriteOnly); //Message a envoyer. Nom de l'auteur et le texte la meme string

    //retour de fonction testé dans le "if"
    int envoie = socket->write(messageManuel.toStdString().c_str());

    //On afiche le message envoyé dans la zone d'Emission Texte
    message_envoie->setText(messageManuel);

    //Test erreur
    if(envoie == -1)
        QMessageBox::critical(this,"Erreur","Message non envoyé");

    //Emission des logs
    messageLogs = QDateTime::currentDateTime().toString()+" : mode manuel enclenché \r\n";
    emit signal_ajoutLogs();
}



//Ce slot est appelé à chaque fois que l'on désire creer de nouveaux logs
void MainWindow::creationLogs()
{

    //Création d'un nom de log
    nomLog = "Logs.txt";

    qDebug()<<"nom du nouveau fichier :"<< nomLog;

    //création d'un object QFile
    QFile file(nomLog);

    // On ouvre notre fichier en lecture seule et on vérifie l'ouverture
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"fichier non créé";
        return;
    }
    // Création d'un objet QTextStream à partir de notre objet QFile
    QTextStream flux(&file);

    // On choisit le codec correspondant au jeu de caractère que l'on souhaite ; ici, UTF-8
    flux.setCodec("UTF-8");

    // Écriture des différentes lignes dans le fichier
    flux << "Fichier Log Hovercraft" << endl;

    qDebug()<<"Ecriture dans le fichier log : C:/Users/Damien/Documents/PFE/build-Hovercraft-Desktop_Qt_5_4_0_MinGW_32bit-Debug";
}


//Ce slot en envoyé systematique lorsqu'il y a un evenement
void MainWindow::ajoutLogs()
{
    qDebug()<<"\nAjout d'un nouveau log\n";

    file.setFileName("Logs.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream out(&file);

    out << endl << messageLogs << endl;
}


//Ce slot est envoyé lorsqu'on désire récupérer les coordonéees du robot
void MainWindow::recuperationCoordonnees()
{

void donneesRecues();

//Parsing des coordonnées

    qDebug()<<"\nParser Coordonnées";
    qDebug()<<"\nmessage Recu 1 avant parse=\n"<<message_1;

    QJsonDocument doc_1 = QJsonDocument::fromJson(message_1.toUtf8());

    qDebug()<< doc_1.isNull();

    QJsonObject JsonObj_1= doc_1.object();

    //Informations Coordonnées
    int X_value_1 = JsonObj_1["robot"].toObject()["X"].toInt();
    qDebug() << "\nX="<<X_value_1;
    int Y_value_1 = JsonObj_1["robot"].toObject()["Y"].toInt();
    qDebug() << "\nY="<<Y_value_1;
    int R_value_1 = JsonObj_1["robot"].toObject()["R"].toInt();
    qDebug() << "\nR="<<R_value_1;

//Afichage des données dans l'IHM

    box_x->setValue(X_value_1);
    box_y->setValue(Y_value_1);
    box_t->setValue(joystick_t);
    box_r->setValue(R_value_1);

    //on relance le timer
    timer();
}



//Ce slot est envoyé lorsqu'on désire lancer le timer
void MainWindow::timer()
{

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(recuperationCoordonnees()));
    connect(timer, SIGNAL(timeout()), this, SLOT(envoie()));
    timer->start(1000);
}


void MainWindow::envoie_coordonnees()
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly); //Message a envoyer. Nom de l'auteur et le texte la meme string

    //On prépare le paquet à envoyer
    QString messageCoordonnees = "{\"robot\":{\"X\":"+QString(joystick_x) +",\"Y\":"+ QString(joystick_y) +",\"T\":"+ QString(joystick_t) +"}";


    int envoie = socket->write(messageCoordonnees.toStdString().c_str()); // On envoie le paquet converti

    //on afiche le message envoyé dans la zone d'Emission Texte
    message_envoie->setText(messageCoordonnees);

    //Test Erreur
    if(envoie == -1)
        QMessageBox::critical(this,"Erreur","Message non envoyé");

    //Emission des logs
    messageLogs = QDateTime::currentDateTime().toString()+" : Emission des coordonnées au serveur "+adr_ip->text()+"\r\n";
    emit signal_ajoutLogs();
}


