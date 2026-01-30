# LIFAPCD - Tac-Tik C++

![Language](https://img.shields.io/badge/language-C++-blue.svg)
![Build](https://img.shields.io/badge/build-Make%20%7C%20CMake-green)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

## 📝 Description

Ce projet est une réécriture en C++ du jeu de société **Tac-Tik**, un jeu de stratégie combinant hasard et tactique (similaire aux Petits Chevaux mais joués avec des cartes).

L’objectif est de fournir une expérience fidèle au jeu de plateau original, tout en mettant en œuvre une architecture **MVC (Modèle-Vue-Contrôleur)** et des concepts avancés de programmation orientée objet.

### Fonctionnalités
* **Deux modes de jeu :**
    * 🖥️ **Console :** Accessible via le terminal, légère et rapide.
    * 🎮 **Graphique (SDL2) :** Interface visuelle complète avec souris et animations.
* **Documentation complète :**
    * [Présentation du projet et choix techniques (PDF)](Presentation.pdf)
    * [Règles officielles du jeu (PDF)](Règles_du_jeu_Tac-Tik-1.pdf)
    * [Planning de réalisation (Gantt)](CC_DiagrammeGantt.pdf)

## 📂 Architecture du projet

```text
.
├── bin/                 # Exécutables générés
├── obj/                 # Fichiers objets temporaires (Linux)
├── obj_win/             # Fichiers objets temporaires (Windows)
├── data/                # Ressources (Assets)
│   ├── cartes/          # Sprites des cartes
│   └── plateau/         # Images des plateaux
├── doc/                 # Documentation Doxygen
├── src/                 # Code Source
│   ├── core/            # Logique du jeu
│   └── affichage/       # Gestion des Vues et du Contrôleur
│       ├── sdl/         # Implémentation Graphique
│       ├── txt/         # Implémentation Console
│       └── Controleur   # Lien Modèle-Vue
├── SDL2-*/              # Bibliothèques pour compilation Windows (MinGW)
├── CMakeLists.txt       # Configuration CMake
├── Makefile             # Configuration Make
└── README.md
```

## ⚙️ Installation et Exécution (Linux)

### Prérequis
* Compilateur C++ (g++)
* Make ou CMake
* Bibliothèques SDL2 :

```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-gfx-dev
```

### Méthode 1 : Via Makefile (Recommandé)

Compilez les différents modules à l'aide des commandes suivantes :

```bash
make txt       # Compile la version console
make sdl       # Compile la version graphique SDL2
make doc       # Génère la documentation Doxygen
make test      # Vérifie les fuites mémoires avec Valgrind
```

### Lancer le jeu :

```bash
./bin/mainTXT   # Version console
./bin/mainSDL   # Version graphique
```

### Méthode 2 : Via CMake

```bash
mkdir build && cd build
cmake ..
make
./mainSDL
```

## 🪟 Compilation pour Windows (Cross-Compilation)

Le projet permet de générer des exécutables `.exe` pour Windows depuis un environnement Linux (nécessite `MinGW`).

### Prérequis :

```bash
sudo apt-get install mingw-w64
```

### Commandes de compilation :
```bash
make mainTXTWindows   # Génère bin/mainTXT.exe
make mainSDLWindows   # Génère bin/mainSDL.exe
make mainDEVWindows   # Génère bin/mainDEV.exe (Debug)
```

## 🧹 Nettoyages

Pour supprimer les fichiers objets et les exécutables :

```bash
make clean        # Supprime les .o et les exécutables
make veryclean    # Supprime aussi la documentation générée
```

## 👥 Contributeurs

Ce projet a été réalisé dans le cadre de l'unité d'enseignement LIFAPCD à l'Université Lyon 1.

* **Marius CISERANE**
* **Valentin LAPORTE**