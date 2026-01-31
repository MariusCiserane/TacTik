#include "Controleur.h"

#include <iostream>
#include <cassert>

using namespace std;


Controleur::Controleur() {
	console = new ImageConsole;
	graphique = nullptr;
	versionGraphique = false;
	joueurActif = -1;
	running = true;
}

Controleur::Controleur(bool affichageGraphique) {
	versionGraphique = affichageGraphique;
	joueurActif = -1;
	running = true;
	if (versionGraphique) {
		console = nullptr;
		graphique = new Image();
	} else {
		console = new ImageConsole();
		graphique = nullptr;
	}
}

Controleur::~Controleur() {
	if (console != nullptr) {
		delete console;
		console = nullptr;
	}
	if (graphique != nullptr) {
		delete graphique;
		graphique = nullptr;
	}
}

void Controleur::initJeu(int nbJoueurs, array<bool, 6> IA) {
	assert(nbJoueurs == 4 || nbJoueurs == 6);
	if (versionGraphique) graphique->initJeu(nbJoueurs, IA);
	else console->initJeu(nbJoueurs, IA);
}

void Controleur::choixIA(int nbJoueurs, array<bool, 6> &IA) {
	assert(nbJoueurs == 4 || nbJoueurs == 6);
	string mess = (versionGraphique) ? "":" : ";
	string coutMessage = "Inclure des IA parmis les joueurs ? (Oui(o) ; Non(n))" + mess;
	char choix = 'n';
	if (versionGraphique) {
		choix = graphique->choixIA(coutMessage);
		if (choix == '0') running = false;
	}
	else {
		choix = cinProtectionChar(coutMessage);
	}
	if (choix != 'o' && choix != 'O') return;
	
	for (int i = 0 ; i < nbJoueurs ; i++) {
		coutMessage = intToStr(i) + " joué par une IA ? (Oui(o) ; Non(n))" + mess;
		if (versionGraphique) {
			choix = graphique->choixIA(coutMessage);
			if (choix == '0') {
				running = false;
				return;
			}
		}
		else {
			choix = cinProtectionChar(coutMessage);
		}
		if (choix == 'o' || choix == 'O') IA[i] = true;
	}
}

Jeu& Controleur::getJeu() const {
	if (versionGraphique) return graphique->getJeu();
	return console->getJeu();
}

bool Controleur::getRunning() const {
	return running;
}

void Controleur::setJoueurActif(int indJoueurActif) {
	assert(-1 <= joueurActif && joueurActif <= 6);
	joueurActif = indJoueurActif;
}

int Controleur::getIdPion(string coutMessage) {
	if (versionGraphique) return afficherJeu(2, coutMessage);
	return cinProtectionInt(coutMessage);
}

int Controleur::getNbCase7x1(string coutMessage, int idPion) {
	if (versionGraphique) {
		Jeu& jeu = getJeu();
		int ind_case = afficherJeu(3, coutMessage);
		int pos = jeu.getPion(idPion).getPos();
		int nb_cases = jeu.getPlateau().getNbCase();
		
		if (ind_case >= 100) {
			const int * maison = jeu.getJoueur(joueurActif).getMaison();
			if (pos == -2) {
				for (int i = 0 ; i <= ind_case - 100 ; i++) {
					if (maison[i] == idPion) return ind_case - 100 - i;
				}
			} else {
				int case_dep = jeu.getPlateau().getCasesDepart(joueurActif+1);
				return (case_dep + ind_case - 99 - pos + nb_cases) % nb_cases;
			}
		} else if (ind_case >= 0) {
			if (pos == -2) return 0;
			return (ind_case - pos + nb_cases) % nb_cases;
		}
		return 0;
	}
	return cinProtectionInt(coutMessage);
}

int Controleur::jouerJoker(string coutMessage) {
	if (versionGraphique) {
		int val = graphique->selectionnerValJoker(joueurActif, coutMessage);
		if (val == 0) running = false;
		return val;
	}
	return cinProtectionInt(coutMessage);
}

char Controleur::saisirCaractere(string coutMessage, int choix) {
	if (versionGraphique) {
		int val = afficherJeu(4, coutMessage, choix);
		if (val == 0) return '0';
		
		switch (choix)
		{
		case 1:
			return (val == 1) ? 'A':'D';
		
		case 2:
			return (val == 1) ? 'O':'N';
		
		default:
			return '0';
		}
	}
	return cinProtectionChar(coutMessage);
}

void Controleur::afficherMessage(string coutMessage) {
	if (versionGraphique) afficherJeu(-1, coutMessage);
	else message(coutMessage);
}

void Controleur::attenteTour(bool dev) {
	if (versionGraphique) {
		afficherJeu(-1, "Tour de " + intToStr(joueurActif) + " (Entrée pour continuer)");
		if (!running) return ;
	}
	else {
		if (!dev) {
			console->affichageTexte(-1);
			afficherMessage("Tour de " + intToStr(joueurActif) + " (Entrée pour continuer)");
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cin.get();
		}
		console->affichageTexte(joueurActif);
		afficherMessage("\nTour de " + intToStr(joueurActif) + " :");
	}
}

bool Controleur::jouerCarte(int valCarte, bool coequipier, bool joker, bool ia) {
	Jeu &jeu = getJeu();
	int nbJoueurs = jeu.getNbJoueurs();
	assert(valCarte == -4 || (-1 <= valCarte && valCarte <= 13 && valCarte != 0 && valCarte != 4));
	int couleur = joueurActif + 1;
	int nb_possible = 0, idPion = 0, c1 = couleur;
	if (coequipier) couleur = 1 + ((couleur < 5) ? (couleur+1)%4 : 10-couleur);
	const Joueur& joueur = jeu.getJoueur(couleur-1);
	bool est_ia = joueur.estIA();
	string mess = (versionGraphique) ? "":" : ";
	
	// Cas du permutter
	if (valCarte == 11) {
		for (int i = (couleur-1)*4 ; i < couleur*4 ; i++) {
			if (jeu.getPion(i+1).getPos() >= 0) {
				idPion = i+1;
				nb_possible++;
			}
		}
		if (nb_possible > 1 && !ia) idPion = 0;
		nb_possible = 0;
		while (idPion < 1 || idPion > 4*nbJoueurs || jeu.getPion(idPion).getPos() < 0 || (idPion-1)/4 != couleur-1) idPion = getIdPion("Id du pion à permutter (pion du joueur)" + mess);
		int idPion2 = 0;
		while (idPion2 < 1 || idPion2 > 4*nbJoueurs || jeu.getPion(idPion2).estPieu() || idPion2 == idPion) idPion2 = getIdPion("Id du deuxième pion avec lequel permutter" + mess);
		if (!jeu.permutter(idPion, idPion2)) return false;
	} 
	
	// Cas du joker
	else if (valCarte == -1) {
		bool continuer = true;
		while (continuer) {
			do {
				valCarte = jouerJoker("Valeur désirée pour le joker" + mess);
				if (!running) return false;
			} while (valCarte != -4 && (valCarte < 1 || valCarte > 13 || valCarte == 4));
			if (!jeu.carteJouable(c1, valCarte, coequipier, true)) afficherMessage("Action impossible ! Choisissez une autre valeur pour le joker.");
			else continuer = false;
		}
		
		return jouerCarte(valCarte, coequipier, true);
	} 
	
	// Cas du démarrer
	else if (valCarte == 1 || valCarte == 10 || valCarte == 13) {
		char choix = '0';
		if (joueur.getReserve() == 4 || (ia && jeu.demarrer(couleur, true))) choix = 'D';
		else if (!jeu.demarrer(couleur, true) || ia) choix = 'A';
		else if (joueur.getReserve() > 0) {
			int nb = 0;
			for (int id = (couleur-1)*4+1 ; id <= 4*couleur ; id++) {
				if (jeu.avancerPion(valCarte, id, true)) {
					nb = 1;
					break;
				}
			}
			choix = (nb == 0) ? 'D':'0';
		}
		while (choix != 'D' && choix != 'A' && choix != 'd' && choix != 'a') choix = saisirCaractere("Utiliser la carte comme démarrer(D) ou avancer(A)" + mess, 1);
		if ((choix == 'D' || choix == 'd') && !jeu.demarrer(couleur)) return false;
		else if (choix == 'A' || choix == 'a') {
			for (int i = (couleur-1)*4 +1 ; i <= couleur*4 ; i++) {
				if (jeu.avancerPion(valCarte, i, true)) {
					idPion = i;
					nb_possible++;
				}
			}
			if (nb_possible == 0) return false;
			if (nb_possible > 1 && !ia) idPion = 0;
			while (idPion < 1 || idPion > 4*nbJoueurs || (idPion-1)/4 != couleur-1) idPion = getIdPion("Id du pion à avancer" + mess);
			if (!jeu.avancerPion(valCarte, idPion)) return false;
		}
	} 
	
	// Cas du 7x1
	else if (valCarte == 7 && joueur.getReserve() < 3) {
		int val, somme = 0;
		vector<pair<int, int>> coups;
		for (int i = (couleur-1)*4 +1 ; i <= couleur*4 ; i++) {
			val = 1;
			while (val < 7 && jeu.avancerPion(val, i, true)) val++;
			somme += val-1;
			coups.push_back({i, val-1});
			if (val == 7 && jeu.avancerPion(val, i, true)) idPion = i;
		}
		if (somme < 7 && idPion == 0) return false;
		if (somme == 6 && !jeu.avancerPion(7, idPion)) return false;
		else if (somme == 6) ; //Le pion a été avancé dans le if précédent
		else if (est_ia && idPion != 0) jeu.avancerPion(7, idPion);
		else {
			somme = 0;
			int i = 0;
			while (somme < 7) {
				bool continuer = !est_ia;
				if (continuer) afficherMessage("Nombre de déplacements restants : " + to_string(7 - somme));
				while (continuer) {
					idPion = 0;
					while (idPion < 1 || idPion > 4*nbJoueurs || (idPion-1)/4 != couleur-1) {
						idPion = getIdPion("Id du pion à avancer" + mess);
						if (!running) return false;
					}
					val = 0;
					while (val < 1 || somme + val > 7) {
						string message = (versionGraphique) ? "Cliquer sur la case où avancer":"Nombre de cases à avancer : ";
						val = getNbCase7x1(message, idPion);
						if (!running) return false;
					}
					if (!jeu.avancerPion(val, idPion, true)) afficherMessage("Ce déplacement est impossible !");
					else continuer = false;
				}
				if (est_ia) {
					val = coups[i].second;
					idPion = coups[i].first;
					if (val + somme > 7) val = 7 - somme;
					i++;
				}
				jeu.avancerPion(val, idPion, false, true);
				somme += val;
			}
		}
	} 
	
	// Cas du avancer
	else {
		for (int i = (couleur-1)*4 +1 ; i <= couleur*4 ; i++) {
			if (jeu.avancerPion(valCarte, i, true)) {
				idPion = i;
				nb_possible++;
			}
		}
		if (nb_possible == 0) return false;
		if (nb_possible > 1 && !est_ia) idPion = 0;
		while (idPion < 1 || idPion > 4*nbJoueurs || (idPion-1)/4 != couleur-1) idPion = getIdPion("Id du pion à avancer" + mess);
		if (!jeu.avancerPion(valCarte, idPion)) return false;
	}
	
	// Affichage sur le tas
	if (joker) valCarte = -1;
	jeu.setTas(jeu.retirerCarte(joueurActif, valCarte));
	return true;
}

int Controleur::choixCarte(string coutMessage, const Joueur& joueur) {
	int valCarte = 0;
	if (versionGraphique) return afficherJeu(1, coutMessage);
	while (!joueur.estDansMain(valCarte)) {
		valCarte = cinProtectionInt(coutMessage);
	}
	return valCarte;
}

void Controleur::echangeDeCartes() {
	Jeu &jeu = getJeu();
	int valCarte, couleur, nbJoueurs = jeu.getNbJoueurs();
	int ordre[6] = {1, 2, 5, 3, 4, 6};
	int echange_carte[3] = {0, 0, 0};
	string mess = (versionGraphique) ? "":" : ";
	for (int i = 0 ; i < nbJoueurs ; i++) {
		couleur = (nbJoueurs == 6) ? ordre[i]:i+1;
		setJoueurActif(couleur-1);
		attenteTour();
		
		if (jeu.getJoueur(joueurActif).estIA()) {
			valCarte = jeu.getJoueur(joueurActif).getCarte(0)->getValeur();
		} else {
			valCarte = choixCarte("Carte à donner à " + intToStr(((couleur < 5) ? (couleur+1)%4 : 10-couleur)) + mess, jeu.getJoueur(joueurActif));
		}

		if (!running) return ;

		if (i < nbJoueurs/2) echange_carte[i] = valCarte;
		else {
			int indJ1 = (couleur == 6) ? 4:couleur-3;
			jeu.echangerCartes(indJ1, joueurActif, echange_carte[i-nbJoueurs/2], valCarte);
		}
	}
}

void Controleur::tourJoueur(bool dev) {
	Jeu &jeu = getJeu();
	if (jeu.getJoueur(joueurActif).mainVide()) return ;
	char choix = 'o';
	int valCarte;
	bool peut_jouer = true, coequipier = (jeu.getJoueur(joueurActif).maisonRemplie());
	IA ia;
	string mess = (versionGraphique) ? "":" : ";
	
	attenteTour(dev);
	
	if (jeu.getJoueur(joueurActif).estIA()) {
		valCarte = ia.jouerCarte(jeu, joueurActif);
		if (valCarte != 0) jouerCarte(valCarte, false, false, true);
	}
	else {
		if (!jeu.peutJouer(joueurActif + 1, coequipier)) {
			choix = saisirCaractere("Aucune carte ne peut être jouée. Défausser toutes les cartes ? (Oui(o) ; Non(n))" + mess, 2);
			if (choix == 'o' || choix == 'O') {
				jeu.defausserJoueur(joueurActif + 1);
				return ;
			}
			peut_jouer = false;
		}
		do {
			valCarte = choixCarte(((peut_jouer) ? "Carte à jouer" : "Carte à défausser") + mess, jeu.getJoueur(joueurActif));
			if (!running) return ;

			if (!jeu.carteJouable(joueurActif + 1, valCarte, coequipier) && peut_jouer) {
				afficherMessage("Cette carte ne peut pas être jouée ! Choisissez-en une autre.");
				choix = 'n';
			} else choix = 'o';

		} while (choix == 'n');

		if (!jeu.carteJouable(joueurActif + 1, valCarte, coequipier)) jeu.defausserCarte(valCarte, joueurActif + 1);
		else jouerCarte(valCarte, coequipier);
	}
}

void Controleur::afficherVainqueur(int couleurVainqueur) const {
	int vainqueurs = couleurVainqueur;
	int j1 = 4, j2 = 5;
	if (vainqueurs < 5) {
		j1 = (vainqueurs+3)%4;
		j2 = (vainqueurs+1)%4;
	}
	string message = "\nLes " + intToStr(j1) + "s et " + intToStr(j2) + "s ont gagné !\n";
	if (versionGraphique) {
		graphique->afficherFin(message);
	} else {
		cout << message << endl;
	}
}

int Controleur::afficherJeu(int etapeActuel, string coutMessage, int choix) {
	int val = -3;
	if (versionGraphique) {
		SDL_Event event;
		if (etapeActuel == -1) graphique->setTextureCartes(-1);
		while (val == -3) {
			while (SDL_PollEvent(&event)) {
				val = gestionEvent(event, etapeActuel);
			}
			if (!running) return val;

			graphique->afficher(joueurActif, coutMessage);
			if (etapeActuel == 4) graphique->afficherBoutons(choix);
		}
		if (etapeActuel == -1) graphique->setTextureCartes(joueurActif);
	}
	else console->affichageTexte(joueurActif);
	return val;
}

int Controleur::gestionEvent(SDL_Event event, int etapeActuel) {
	
	int val = -3;

	if (event.type == SDL_QUIT) {
		running = false;
		return val;
	}

	if (event.type == SDL_KEYUP) {
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			running = false;
			return val;
		}
		if (event.key.keysym.sym == SDLK_t) {
			graphique->grossissement(true);
		}
		if (event.key.keysym.sym == SDLK_q) {
			graphique->grossissement(false);
		}

		if (etapeActuel == -1 && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE)) {
			val = -1;
			return val;
		}
	}
	if (event.type == SDL_MOUSEBUTTONDOWN && joueurActif >= 0) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			if (event.button.x > graphique->getImgWidth() && etapeActuel == 1) {
				int indiceCase = event.button.y / (250 * graphique->getZoom());
				Carte* carte = getJeu().getJoueur(joueurActif).getCarte(indiceCase);
				if (indiceCase < 4 && carte) {
					val = carte->getValeur();
				}
				return val;
			} else if (etapeActuel == 2) {
				val = graphique->getIndicePion(event.button.x, event.button.y);
			} else if (etapeActuel == 3) {
				val = graphique->getIndiceCase(event.button.x, event.button.y, joueurActif);
			} else if (etapeActuel == 4) {
				val = graphique->getBouton(event.button.x, event.button.y);
			}
		}
	}
	return val;
}

int Controleur::afficherMenu(string coutMessage) {
	if (versionGraphique) {
		int val = graphique->afficherMenu(coutMessage);
		if (val == 0) running = false;
		return val;
	}
	return cinProtectionInt(coutMessage);
}


void jouer(bool versionGraphique, bool dev){
	srand(time(NULL));
	
	int nbJoueurs = 0;
	array<bool, 6> IA;
	IA.fill(false);
	string mess = (versionGraphique) ? "":" : ";
	Controleur controleur(versionGraphique);
	
	while (nbJoueurs != 4 && nbJoueurs != 6) {
		nbJoueurs = controleur.afficherMenu("Nombre de joueurs (4 ou 6)" + mess);
		if (!controleur.getRunning()) return;
	}
	
	controleur.choixIA(nbJoueurs, IA);
	if (!controleur.getRunning()) return;

	controleur.initJeu(nbJoueurs, IA);
	Jeu &jeu = controleur.getJeu();
	int ordre[6] = {1, 2, 5, 3, 4, 6};
	int tour = 0;
	while (true) {
		if (!dev) {
			jeu.distribuer();
			controleur.echangeDeCartes();
			if (!controleur.getRunning()) return ;
		} else {
			for (int i = 0 ; i < 4 ; i++) {
				for (int j = 0 ; j < nbJoueurs ; j++) {
					jeu.attribuerCarte(-1, j+1);
				}
			}
		}

		controleur.afficherJeu(-1, "Tour " + to_string(tour+1));
		
		for (int i = 0 ; i < 4 ; i++) {
			for (int j = tour ; j < nbJoueurs + tour ; j++) {
				int couleur = (nbJoueurs == 6) ? ordre[j%nbJoueurs]:(j%nbJoueurs)+1;
				controleur.setJoueurActif(couleur-1);
				controleur.tourJoueur(dev);
				if (!controleur.getRunning()) return ;
				if (jeu.partieGagnee()) {
					cout << "Nombre de tours de jeu : " << tour+1 << endl;
					controleur.setJoueurActif(6);
					controleur.afficherJeu(6);
					return controleur.afficherVainqueur(couleur);
				}
			}
		}

		tour++;
	}
}
