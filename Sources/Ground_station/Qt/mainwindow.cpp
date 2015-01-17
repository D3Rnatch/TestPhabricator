//Adresse Serveur : 10.5.133.185

#include "mainwindow.h"
#include "libraries.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui (new Ui::MainWindow)
{
    setupUi(this); //initialision de l'IHM

    socket = new QTcpSocket(this);//Création du socket

    connect(socket, SIGNAL(readyRead()),this, SLOT(donneesRecues()));//Lancement de la reception de données Robot
    connect(socket, SIGNAL(connected()),this, SLOT(connecte()));//Lancement de la connexion
    connect(socket, SIGNAL(connected()),this, SLOT(envoie_message_hello()));//On envoie le message "hello" pour obtenir les données du Robot
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte()));//Lancement de la deconnexion
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));//Traitement des erreurs du socket
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(messageErreur()));//Lancement de la Fenêtre Erreur




     tailleMessage = 0;

//----------DATE & HEURE---------------

    //Actualisation de l'heure
  QDateTime date_heure = QDateTime::currentDateTime();

    //Tests sur l'heure actuelle
   QTextStream out(stdout);
   out << "Current date is " << date_heure.toString() << endl;

//------------MENU----------------------

    //Action Fermer
   QObject::connect(actionFermer, SIGNAL(triggered()), this, SLOT(quitterApp()));

    //Action Se Connecter
   QObject::connect(actionSe_connecter_au_Drone, SIGNAL(triggered()),this, SLOT(connexion()));

    //Action Se Deconnecter
   QObject::connect(action_se_deconnecter, SIGNAL(triggered()),this, SLOT(deconnexion()));

    //Action Arrêt d'urgence
   QObject::connect(actionArret_d_urgence, SIGNAL(triggered()),this, SLOT(envoie_arret_urgence()));

   //Gestion de la batterie
   //progressBarBatterie->setValue();

   //Gestion de la vitesse
   //ProgressBarVitesse->
}


void MainWindow::quitterApp()
{
    qDebug()<<"\nquitter App() : exit app\n";
    exit(0);
}



void MainWindow::connexion()
{

    socket->abort();//On désactive les connexions précédentes s'il y en a
    socket->connectToHost(adr_ip->text(), numero_port_serveur->value());//On se connecte au serveur demandé

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

     }

//Envoie d'un messsage au serveur
void MainWindow::on_boutonEnvoyer_clicked()
{
    message_envoie->clear();

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly); //Message a envoyer. Nom de l'auteur et le texte la meme string

    //On prépare le paquet à envoyer
    QString messageAEnvoyer = "{\"robot\":{\"X\":10,\"Y\":10,\"T\":20},\"message\":{\"type\":\"system\",\"content\":\"hello\"}}";

    socket->write(messageAEnvoyer.toStdString().c_str()); // On envoie le paquet convertie

    //on afiche le message envoyé dans la zone d'Emission Texte
    message_envoie->setText(messageAEnvoyer);

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
    qDebug()<<temp;
    qDebug()<<socket->bytesAvailable();

    //conversion du message reçu en QString
    QString messageRecu(temp);


    //On affiche le message sur la zone de Reception texte
    message_recu->setText(messageRecu);

    //On remet la taille du message à 0 pour pouvoir recevoir de futurs messages
    tailleMessage = 0;


}


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
}




//Ce slot est envoyé des qu'on clique sur le bouton d'arret d'urgence
void MainWindow::envoie_arret_urgence()
{

    QByteArray paquet_urgence;
    QDataStream out(&paquet_urgence, QIODevice::WriteOnly); //Message a envoyer. Nom de l'auteur et le texte la meme string

    //On prépare le paquet à envoyer
    QString messageAEnvoyer_urgence = "{\"robot\":{\"X\":0,\"Y\":0,\"T\":0},\"message\":{\"type\":\"system\",\"content\":\"stop\"}}";

    // On envoie le paquet convertie
    socket->write(messageAEnvoyer_urgence.toStdString().c_str());

    //on afiche le message envoyé dans la zone d'Emission Texte
    message_envoie->setText(messageAEnvoyer_urgence);

    //On nettoie la zone de reception
    message_recu->clear();
}


void MainWindow::messageErreur()
{
    //Lacement de la Pop-Up message d'Erreur
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
    QString messageHello = "{\"robot\":{\"X\":10,\"Y\":10,\"T\":20},\"message\":{\"type\":\"system\",\"content\":\"hello\"}}";

    socket->write(messageHello.toStdString().c_str()); // On envoie le paquet convertie

    //on afiche le message envoyé dans la zone d'Emission Texte
    message_envoie->setText(messageHello);

}










/*
void MainWindow::parser()
{
    bool ok;
    // json is a QString containing the JSON data
    QtJson::JsonObject result = QJson::parse(json, ok).toMap();

    if(!ok)
    {
    qFatal("An error occurred during parsing");
    }
    qDebug() << "encoding:" << result["encoding"].toString();
    qDebug() << "plugins:";

    foreach(QVariant plugin, result["plug-ins"].toList())
    {
        qDebug() << "  -" << plugin.toString();
    }

    QtJson::JsonObject nested = result["indent"].toMap();
    qDebug() << "length:" << nested["length"].toInt();
    qDebug() << "use_space:" << nested["use_space"].toBool();

}
*/
