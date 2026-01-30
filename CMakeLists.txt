cmake_minimum_required(VERSION 3.10)
project(TacTik VERSION 1.0 LANGUAGES CXX C)

# --- Configuration Principale (équivalent CXXFLAGS) ---
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -g)

# Sortie des exécutables dans le dossier 'bin' (comme le Makefile)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# --- Dossiers et Chemins d'inclusion ---
set(CORE_DIR ${CMAKE_SOURCE_DIR}/src/core)
set(AFFICHAGE_DIR ${CMAKE_SOURCE_DIR}/src/affichage)
set(TXT_DIR ${AFFICHAGE_DIR}/txt)
set(SDL_DIR ${AFFICHAGE_DIR}/sdl)

# --- Recherche des dépendances (équivalent LDFLAGS) ---
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# SDL2_gfx n'a pas toujours de module CMake standard, on le cherche manuellement
find_library(SDL2_GFX_LIBRARY NAMES SDL2_gfx)
find_path(SDL2_GFX_INCLUDE_DIR NAMES SDL2_gfxPrimitives.h PATH_SUFFIXES SDL2)

# Inclusion des headers
include_directories(
    ${CORE_DIR}
    ${AFFICHAGE_DIR}
    ${TXT_DIR}
    ${SDL_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_GFX_INCLUDE_DIR}
)

# --- Définition des groupes de sources (équivalent CORE_OBJS et VIEW_OBJS) ---

# Objets communs au noyau du jeu
set(CORE_SOURCES
    ${CORE_DIR}/Carte.cpp
    ${CORE_DIR}/Pioche.cpp
    ${CORE_DIR}/Pion.cpp
    ${CORE_DIR}/Joueur.cpp
    ${CORE_DIR}/IA.cpp
    ${CORE_DIR}/Plateau.cpp
    ${CORE_DIR}/Jeu.cpp
)

# Objets d'affichage et contrôle
# Note : Le Makefile lie TOUT (Console ET Graphique) pour tous les exécutables
set(VIEW_SOURCES
    ${TXT_DIR}/AffichageConsole.cpp
    ${SDL_DIR}/AffichageGraphique.cpp
    ${AFFICHAGE_DIR}/Controleur.cpp
)

# --- Création des Exécutables ---

# 1. Main TXT
add_executable(mainTXT src/mainTXT.cpp ${CORE_SOURCES} ${VIEW_SOURCES})
target_link_libraries(mainTXT 
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${SDL2_TTF_LIBRARIES} 
    ${SDL2_GFX_LIBRARY}
)

# 2. Main SDL
add_executable(mainSDL src/mainSDL.cpp ${CORE_SOURCES} ${VIEW_SOURCES})
target_link_libraries(mainSDL 
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${SDL2_TTF_LIBRARIES} 
    ${SDL2_GFX_LIBRARY}
)

# 3. Main DEV
add_executable(mainDEV src/mainDEV.cpp ${CORE_SOURCES} ${VIEW_SOURCES})
target_link_libraries(mainDEV 
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${SDL2_TTF_LIBRARIES} 
    ${SDL2_GFX_LIBRARY}
)

# 4. Test (Le Makefile ne lie pas la SDL pour 'bin/test')
add_executable(test src/mainTEST.cpp ${CORE_SOURCES})
# Pas de linkage SDL ici pour respecter la règle bin/test du Makefile

# --- Cibles Personnalisées (Custom Targets) ---

# Cible : make memcheck (équivalent à 'make test' avec valgrind dans ton Makefile)
find_program(VALGRIND_PATH valgrind)
if(VALGRIND_PATH)
    add_custom_target(memcheck
        COMMAND ${VALGRIND_PATH} --leak-check=full --track-origins=yes $<TARGET_FILE:test>
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Lancement de Valgrind sur l'exécutable de test..."
    )
endif()

# Cible : make doc
find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc/doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Génération de la documentation Doxygen..."
    )
    
    # Cible : make cleandoc
    add_custom_target(cleandoc
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/doc/html
        COMMENT "Suppression de la documentation HTML..."
    )
endif()

# --- Configuration pour Windows (Cross-Compilation MinGW) ---
if(WIN32)
    # Ajout des flags statiques présents dans la partie Windows du Makefile
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    # Note : Pour cross-compiler, il faut invoquer cmake avec un fichier toolchain, 
    # ex: cmake -DCMAKE_TOOLCHAIN_FILE=Toolchain-mingw.cmake ..
endif()