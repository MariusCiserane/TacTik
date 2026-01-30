# --- Configuration Principale ---
CXX = g++
CXXFLAGS = -Wall -g -c
LDFLAGS = -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_gfx

# --- Dossiers ---
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj
WIN_OBJ_DIR = obj_win
DOC_DIR = doc

CORE = $(SRC_DIR)/core
AFFICHAGE = $(SRC_DIR)/affichage
TXT = $(AFFICHAGE)/txt
SDL = $(AFFICHAGE)/sdl

# --- Fichiers Objets ---
# Objets communs au noyau du jeu
CORE_OBJS = $(OBJ_DIR)/Carte.o $(OBJ_DIR)/Pioche.o $(OBJ_DIR)/Pion.o \
            $(OBJ_DIR)/Joueur.o $(OBJ_DIR)/IA.o $(OBJ_DIR)/Plateau.o \
            $(OBJ_DIR)/Jeu.o

# Objets d'affichage et contrôle
VIEW_OBJS = $(OBJ_DIR)/AffichageConsole.o $(OBJ_DIR)/AffichageGraphique.o \
            $(OBJ_DIR)/Controleur.o

# --- Cibles Principales ---

all: clean bin_dir $(BIN_DIR)/mainTXT $(BIN_DIR)/mainSDL

# Exécution rapide
txt: $(BIN_DIR)/mainTXT
	./$(BIN_DIR)/mainTXT

sdl: $(BIN_DIR)/mainSDL
	./$(BIN_DIR)/mainSDL

dev: $(BIN_DIR)/mainDEV
	./$(BIN_DIR)/mainDEV

test: $(BIN_DIR)/test
	valgrind --leak-check=full --track-origins=yes ./$(BIN_DIR)/test

# Documentation
doc:
	doxygen $(DOC_DIR)/doxyfile

cleandoc:
	rm -rf $(DOC_DIR)/html

# Nettoyage
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/* $(WIN_OBJ_DIR)/*.o

# --- Compilation des Exécutables ---

$(BIN_DIR)/mainTXT: $(CORE_OBJS) $(VIEW_OBJS) $(OBJ_DIR)/mainTXT.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/mainSDL: $(CORE_OBJS) $(VIEW_OBJS) $(OBJ_DIR)/mainSDL.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/mainDEV: $(CORE_OBJS) $(VIEW_OBJS) $(OBJ_DIR)/mainDEV.o
	$(CXX) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/test: $(CORE_OBJS) $(OBJ_DIR)/mainTEST.o
	$(CXX) $^ -o $@

# --- Compilation des Objets (Règles génériques simplifiées) ---

$(OBJ_DIR)/%.o: $(CORE)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(TXT)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SDL)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(AFFICHAGE)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Création du dossier bin si nécessaire
bin_dir:
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

# --- Cross-Compilation Windows (MinGW) ---

CROSS = x86_64-w64-mingw32-g++
CROSS_FLAGS = -Wall -g -c -municode -DUNICODE -D_UNICODE $(shell x86_64-w64-mingw32-pkg-config sdl2 SDL2_image SDL2_ttf SDL2_gfx --cflags)
CROSS_LDFLAGS = $(shell x86_64-w64-mingw32-pkg-config sdl2 SDL2_image SDL2_ttf SDL2_gfx --libs) -static -static-libgcc -static-libstdc++ -lmingw32

# Cibles Windows
mainTXTWindows: $(BIN_DIR)/mainTXT.exe
mainSDLWindows: $(BIN_DIR)/mainSDL.exe
mainDEVWindows: $(BIN_DIR)/mainDEV.exe

# Règles Windows génériques
$(WIN_OBJ_DIR):
	mkdir -p $(WIN_OBJ_DIR)

$(WIN_OBJ_DIR)/%.o: $(CORE)/%.cpp | $(WIN_OBJ_DIR)
	$(CROSS) $(CROSS_FLAGS) $< -o $@

$(WIN_OBJ_DIR)/%.o: $(TXT)/%.cpp | $(WIN_OBJ_DIR)
	$(CROSS) $(CROSS_FLAGS) $< -o $@

$(WIN_OBJ_DIR)/%.o: $(SDL)/%.cpp | $(WIN_OBJ_DIR)
	$(CROSS) $(CROSS_FLAGS) $< -o $@

$(WIN_OBJ_DIR)/%.o: $(AFFICHAGE)/%.cpp | $(WIN_OBJ_DIR)
	$(CROSS) $(CROSS_FLAGS) $< -o $@

$(WIN_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(WIN_OBJ_DIR)
	$(CROSS) $(CROSS_FLAGS) $< -o $@

# Linkage Windows
WIN_CORE_OBJS = $(addprefix $(WIN_OBJ_DIR)/, $(notdir $(CORE_OBJS)))
WIN_VIEW_OBJS = $(addprefix $(WIN_OBJ_DIR)/, $(notdir $(VIEW_OBJS)))

$(BIN_DIR)/mainTXT.exe: $(WIN_CORE_OBJS) $(WIN_VIEW_OBJS) $(WIN_OBJ_DIR)/mainTXT.o | bin_dir
	$(CROSS) $^ -o $@ $(CROSS_LDFLAGS)

$(BIN_DIR)/mainSDL.exe: $(WIN_CORE_OBJS) $(WIN_VIEW_OBJS) $(WIN_OBJ_DIR)/mainSDL.o | bin_dir
	$(CROSS) $^ -o $@ $(CROSS_LDFLAGS)

$(BIN_DIR)/mainDEV.exe: $(WIN_CORE_OBJS) $(WIN_VIEW_OBJS) $(WIN_OBJ_DIR)/mainDEV.o | bin_dir
	$(CROSS) $^ -o $@ $(CROSS_LDFLAGS)