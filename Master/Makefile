# Nom de l'exécutable
EXEC = master_handler

# Fichiers sources
SRC = $(wildcard *.cpp) $(wildcard src/*/*.cpp)

# Fichiers objets
OBJ = $(SRC:.cpp=.o)

# Librairies
LIB = -lwiringPi -lpthread -ljsoncpp

# Flags de compilation
CXXFLAGS = -std=c++11 -Wall -Wextra -I/usr/lib/ssl

# Règle par défaut
all: $(EXEC)

# Règle de compilation
$(EXEC): $(OBJ)
	g++ -o $@ $^ $(CXXFLAGS) $(LIB)

# Règle de nettoyage
clean:
	rm -f $(OBJ) $(EXEC)

# Règle d'exécution
run: $(EXEC)
	./$(EXEC)

# Règles de dépendances
%.o: %.cpp
	g++ -c -o $@ $< $(CXXFLAGS) $(LIB)

###################################################
