// Fonction Arduino pour la r�ception des donn�es

// R�cup�re les donn�es du port s�rie pour la position du joystick (toutes � 0 si probl�me d�tect�)
void get_joystick(char &x, char &y, char &y){

	// Ressources
	char get_data = 1;
	char data;
	char increment = 1

	// Tant qu'il y a des donn�es � lire, on les lit
	while (Serial.available() && get_data)
	{
		// Lecture des donn�es
		data = Serial.read();

		// En focntion de la position des donn�es dans le protocol
		switch (increment){
		case 1: // premi�re donn�e
			*x = data;
			break;
		case 2: // deuxi�me donn�e
			*y = data;
			break;
		case 3: // troisi�me donn�e
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
		default: // probl�me
			*x = 0;
			*y = 0;
			*z = 0;
			// On quitte
			get_data = 0;
		}

		// Donn�e suivante
		increment++;
	}
}

// Traitement des donn�es r�cup�r�es
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