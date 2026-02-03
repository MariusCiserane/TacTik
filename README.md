# LIFAPCD - Tac-Tik C++

![Language](https://img.shields.io/badge/language-C++-blue.svg)
![Build](https://img.shields.io/badge/build-Make%20%7C%20CMake-orange)
![License](https://img.shields.io/badge/license-MIT-lightgrey.svg)

## 📝 Description

Ce projet est une réécriture en C++ du jeu de société **Tac-Tik**, un jeu de stratégie combinant hasard et tactique (similaire aux Petits Chevaux mais joués avec des cartes).

L’objectif est de fournir une expérience fidèle au jeu de plateau original, tout en mettant en œuvre une architecture **MVC (Modèle-Vue-Contrôleur)** et des concepts avancés de programmation orientée objet.

### Fonctionnalités
* **Deux modes de jeu :**
    * 🖥️ **Console :** Accessible via le terminal, légère et rapide.
    * 🎮 **Graphique (SDL2) :** Interface visuelle complète avec souris et animations.
* **Documentation complète :**
    * [Présentation du projet et choix techniques (PDF)](project-files/Presentation.pdf)
    * [Règles officielles du jeu (PDF)](project-files/Règles_du_jeu_Tac-Tik-1.pdf)
    * [Planning de réalisation (Gantt)](project-files/CC_DiagrammeGantt.pdf)

## 📂 Architecture du projet

```text
.
├── bin/                 # Exécutables générés
├── obj/                 # Fichiers objets temporaires (Linux)
├── obj_win/             # Fichiers objets temporaires (Windows)
├── data/                # Ressources (Assets)
│   ├── cartes/          # Images des cartes
│   └── plateau/         # Images des plateaux
├── doc/                 # Documentation (Doxygen et Diagrammes)
├── project-files/       # Règles, rapport et présentations PDF
├── src/                 # Code Source
│   ├── mainSDL.cpp      # Point d'entrée Version Graphique
│   ├── mainTXT.cpp      # Point d'entrée Version Console
│   ├── mainDEV.cpp      # Point d'entrée Version Développeur
│   ├── mainTEST.cpp     # Point d'entrée Tests unitaires
│   ├── core/            # Logique du jeu (Modèle)
│   └── affichage/       # Gestion des Vues et du Contrôleur
│       ├── Controleur.* # Lien Modèle-Vue
│       ├── sdl/         # Implémentation Graphique (Vue)
│       └── txt/         # Implémentation Console (Vue)
├── SDL2-*/              # Bibliothèques pour compilation Windows (MinGW)
├── CMakeLists.txt       # Configuration CMake
├── Makefile             # Configuration Make
├── LICENSE              # Licence MIT du projet
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
make txt       # Compile et lance la version console
make sdl       # Compile et lance la version graphique SDL2
make dev       # Compile et lance la version Développeur
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
make clean        # Supprime les objets (.o) et les binaires
make cleandoc     # Supprime la documentation générée
```

## 👥 Contributeurs

Ce projet a été réalisé dans le cadre de l'unité d'enseignement LIFAPCD à l'Université Lyon 1.

* **Marius CISERANE**
* **Valentin LAPORTE**

---

## ⚖️ Licence & Propriété Intellectuelle

Le code source de ce projet est distribué sous la licence **MIT**.

> **⚠️ Avertissement :**
> Ce logiciel est une adaptation numérique réalisée à des fins **pédagogiques et non lucratives**.
> Les règles du jeu, le nom "Tac-Tik" et les concepts originaux restent la propriété exclusive de leurs auteurs et éditeurs respectifs.
> Ce projet n'est pas affilié à l'éditeur officiel du jeu.