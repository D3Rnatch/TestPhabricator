// Fonction Arduino pour la réception des données

// Récupère les données du port série pour la position du joystick (toutes à 0 si problème détecté)
void get_joystick(char &x, char &y, char &y){

	// Ressources
	char get_data = 1;
	char data;
	char increment = 1

	// Tant qu'il y a des données à lire, on les lit
	while (Serial.available() && get_data)
	{
		// Lecture des données
		data = Serial.read();

		// En focntion de la position des données dans le protocol
		switch (increment){
		case 1: // première donnée
			*x = data;
			break;
		case 2: // deuxième donnée
			*y = data;
			break;
		case 3: // troisième donnée
			*z = data;
			break;
		case 4: // fin de paquet
			if (data != '\n')
			{
				*x = 0;
				*y = 0;
				*z = 0;
			}
			// On quitte
			get_data = 0;
			break;
		default: // problème
			*x = 0;
			*y = 0;
			*z = 0;
			// On quitte
			get_data = 0;
		}

		// Donnée suivante
		increment++;
	}
}

// Traitement des données récupérées
void thread_data(&x, &y, &z)
{
	if (*x == 0 && *y == 0 && *z == 0)
	{
		// nothing to do
	}
	else
	{
		// adapt value
		*x = *x - 79;
		*y = *y - 79;
		*z = *z - 79;
	}
}