#ifdef _WIN32
#include <windows.h>
#endif

#include "affichage/Controleur.h"
#include <iostream>

using namespace std;

int main() {
	#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8);
	#endif

	srand(time(NULL));
	
	/* Début de l'espace modifiable */	
	
	jouer(true, true);

	/* Fin de l'espace modifiable */
		
	return 0;
}