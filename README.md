# Tac-Tik C++

![Language](https://img.shields.io/badge/Language-C++-00599c)
![Build](https://img.shields.io/badge/Build-Make_|_CMake-orange)
![Library](https://img.shields.io/badge/Library-SDL2-red)
![License](https://img.shields.io/badge/License-MIT-lightgrey)

Adaptation en C++ du jeu de société **Tac-Tik**, développée selon une architecture **MVC (Modèle-Vue-Contrôleur)**.
Projet réalisé dans l'UE "Conception et Développement d'Applications" (S4, Polytech Lyon).

## Fonctionnalités

* **Mode Console :** Léger et rapide, jouable dans le terminal.
* **Mode Graphique :** Interface complète avec souris et animations (SDL2).
* **Intelligence Artificielle :** Joueur contre Ordinateur (IA basique).

## Architecture Technique

Le projet suit le pattern MVC pour séparer la logique (Core) de l'affichage (Console/SDL).

### Documentation
* [Présentation et choix techniques](project-files/Presentation.pdf)
* [Règles du jeu](project-files/Règles_du_jeu_Tac-Tik-1.pdf)
* [Diagramme de Gantt](project-files/CC_DiagrammeGantt.pdf)

## Architecture du projet

```text
.
├── bin/                 # Exécutables
├── data/                # Assets
├── doc/                 # Documentation Doxygen et diagrammes
├── src/
│   ├── mainSDL.cpp
│   ├── mainTXT.cpp
│   ├── core/            # Modèle
│   └── affichage/       # Vues & Contrôleur
├── CMakeLists.txt
├── Makefile
├── LICENSE
└── README.md
```

## Installation

#### Prérequis (Linux)
* Compilateur C++ (g++)
* Make ou CMake
* Bibliothèques SDL2 :

```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-gfx-dev
```

### Compilation et Exécution

Utilisez le Makefile fourni :

```bash
make txt       # Compile et lance la version console
make sdl       # Compile et lance la version graphique SDL2
make dev       # Compile et lance la version Développeur
make doc       # Génère la documentation Doxygen
make test      # Vérifie les fuites mémoires avec Valgrind
make clean     # Supprime les objets (.o) et les binaires
```

### Cross-Compilation (Windows)

Le projet permet de générer des exécutables `.exe` pour Windows depuis un environnement Linux (nécessite `MinGW`).

* **Prérequis :**

```bash
sudo apt-get install mingw-w64
```

* **Commandes de compilation :**
```bash
make mainTXTWindows   # Génère bin/mainTXT.exe
make mainSDLWindows   # Génère bin/mainSDL.exe
make mainDEVWindows   # Génère bin/mainDEV.exe (Debug)
```

## Auteurs

* **Marius CISERANE**
* **Valentin LAPORTE**


## Licence

Ce projet est sous licence MIT - voir le fichier [LICENSE](LICENSE) pour plus de détails.

> **Avertissement :** Ce logiciel est une adaptation numérique réalisée à des fins **pédagogiques et non lucratives**. Les règles du jeu et le nom "Tac-Tik" restent la propriété exclusive de leurs ayants droit.