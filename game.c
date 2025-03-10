#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PLAYERS 4
#define grid_MIN_SIZE 15
#define grid_MAX_SIZE 20
#define NUM_TARGETS 18

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position pos;
    char target;
    int movesPredicted;
    int movesTaken;
} Player;

char grid[grid_MAX_SIZE][grid_MAX_SIZE];
char targets[NUM_TARGETS];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initializegrid(int gridSize, int numPlayers, Player *players) {
    // Initialiser la grid avec des espaces vides
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            grid[i][j] = ' ';
        }
    }

    // Placer les murs autour de la grid
    for (int i = 0; i < gridSize; i++) {
        grid[0][i] = '#';  // Mur supérieur
        grid[i][0] = '#';  // Mur gauche
        grid[gridSize-1][i] = '#';  // Mur inférieur
        grid[i][gridSize-1] = '#';  // Mur droit
    }
    

    // Initialiser les cibles
    for (int i = 0; i < NUM_TARGETS; i++) {
        targets[i] = 'A' + i;
    }

    // Placer les cibles (18 cibles représentées par des lettres de A à R)
    for (int i = 0; i < NUM_TARGETS; i++) {
        int x, y;
        int validPosition;
        int random;
        do {
            validPosition = 1;
            x = rand() % (gridSize - 4) + 2;  // Éviter les bords
            y = rand() % (gridSize - 4) + 2;  // Éviter les bords

            // Vérifier que la cible n'est pas à côté d'une autre cible
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (grid[x + dx][y + dy] >= 'A' && grid[x + dx][y + dy] <= 'R') {
                        validPosition = 0;
                        break;
                    }
                }
                if (!validPosition) break;
            }
        } while (!validPosition);

        grid[x][y] = targets[i];  // Marquer la position de la cible avec une lettre
        
        // Placer des murs formant un angle autour de la cible
        
        random = rand () % 4;
        switch (random) {
            case 0:
                grid[x][y-1] = '#';
                grid[x+1][y] = '#';
                break;
            case 1:
                grid[x][y-1] = '#';
                grid[x-1][y] = '#';
                break;
            case 2:
                grid[x][y+1] = '#';
                grid[x+1][y] = '#';
                break;
            case 3:
                grid[x][y+1] = '#';
                grid[x-1][y] = '#';
                break;
            default:
                break;
        
        }
    }
    
    // Placer les murs perpendiculaires aux bordures
    for (int i = 0; i < 2 ; i++){
        int r;
        r = rand() % (gridSize - 2) + 1;
        while (grid[1][r] == '#'){
            r = rand() % (gridSize - 2) + 1;
        }
        grid[1][r] = '#'; // Mur supérieur
        
        
        r = rand() % (gridSize - 2) + 1;
        while (grid[r][1] == '#'){
            r = rand() % (gridSize - 2) + 1;
        }
        grid[r][1] = '#'; // Mur gauche
        
        
        r = rand() % (gridSize - 2) + 1;
        while (grid[gridSize- 2][r] == '#'){
            r = rand() % (gridSize - 2) + 1;
        }
        grid[gridSize- 2][r] = '#';    // Mur inférieur
        
        
        r = rand() % (gridSize - 2) + 1;
        while (grid[r][gridSize- 2] == '#'){
             r = rand() % (gridSize - 2) + 1;
        }
        grid[r][gridSize- 2] = '#';  // Mur droit
    
    }
    
    // Placer les joueurs sur des positions aléatoires distinctes
    for (int i = 0; i < numPlayers; i++) {
        int x, y;
        do {
            x = rand() % (gridSize - 2) + 1;
            y = rand() % (gridSize - 2) + 1;
        } while (grid[x][y] != ' '); // Assurer que la position est vide
        players[i].pos.x = x;
        players[i].pos.y = y;
        grid[x][y] = '1' + i;  // Marquer la position du joueur avec son numéro
    }
        
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void displaygrid(int gridSize) {
    system("clear");  // Effacer le terminal (Linux)
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            if (grid[i][j] == '#') {
                printf( "\033[36m" "%c " "\033[0m", grid[i][j]); 
                
            }
            
            else if(grid[i][j] >='A' && grid[i][j]<='R') {
                printf ("\033[33m" "%c " "\033[0m", grid[i][j]);
            }
            
            else{
                printf("%c ", grid[i][j]);
            }
        }
        printf("\n");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void movePlayer(Player *player, char direction, int gridSize, Player *players, int targetReached) {
    int newX = player->pos.x;
    int newY = player->pos.y;
    int dx = 0, dy = 0;
    static char previousValue = ' '; // Ajout d'une variable statique pour mémoriser la valeur précédente de la case

    switch (direction) {
        case 'h': case 'H': dx = -1; break; // Haut
        case 'b': case 'B': dx = 1; break;  // Bas
        case 'g': case 'G': dy = -1; break; // Gauche
        case 'd': case 'D': dy = 1; break;  // Droite
        default: break;
    }

    while (1) {
        int nextX = newX + dx;
        int nextY = newY + dy;

        // Vérifier si le déplacement est valide
        if (nextX < 0 || nextX >= gridSize || nextY < 0 || nextY >= gridSize || grid[nextX][nextY] == '#' || (grid[nextX][nextY] >= '1' && grid[nextX][nextY] <= '4')) {
            break; // Sortir de la boucle si un obstacle est rencontré
        }

        newX = nextX;
        newY = nextY;
    }
    
    if (grid[newX][newY] == player->target) { // si cible atteinte
        grid[player->pos.x][player->pos.y] = previousValue; // Restaurer la valeur précédente de la case
        previousValue = grid[newX][newY]; // Sauvegarder la nouvelle valeur de la case
        player->pos.x = newX;
        player->pos.y = newY;
        grid[newX][newY] = player->target;
    }
    
    if (direction == 'f'|| direction == 'F'){  // si déclaré forfait, cette condition permet de désafficher le joueur 
        grid[player->pos.x][player->pos.y] = previousValue; // Restaurer la valeur précédente de la case
        grid[newX][newY] = previousValue;
    }
    
    
    else{
        // Mettre à jour la grid
        grid[player->pos.x][player->pos.y] = previousValue; // Restaurer la valeur précédente de la case
        previousValue = grid[newX][newY]; // Sauvegarder la nouvelle valeur de la case
        player->pos.x = newX;
        player->pos.y = newY;
        grid[newX][newY] = '1' + (player - players); // Mettre à jour la position du joueur sur la grid
    }
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void playGame(int gridSize, int numPlayers, int numRounds) {
    Player players[MAX_PLAYERS];
    int roundScores[MAX_PLAYERS] = {0};
    

    // Boucle de jeu pour chaque manche
    for (int round = 1; round <= numRounds; round++) {
        
        //choisir une taille de grid aléatoire entre 15 et 20
        gridSize = rand() % 6 + 15;
        initializegrid(gridSize, numPlayers, players);
        displaygrid(gridSize);
        printf("\nManche %d\n", round);
        printf("Taille de la grid : %d\n\n", gridSize);

        // Assigner une cible à chaque joueur
        for (int i = 0; i < numPlayers; i++) {
            int cible = rand() % NUM_TARGETS;
            players[i].target = targets[cible];
            printf("Joueur %d - Votre objectif est la cible %c\n", i+1, players[i].target);
        }
        printf("\n");


        // Demander les prédictions de mouvements
        for (int i = 0; i < numPlayers; i++) {
            int validInput = 0;
            while (!validInput) {
                printf("Joueur %d - Prédiction de coups pour atteindre la cible %c : ", i+1, players[i].target);
                if (scanf("%d", &players[i].movesPredicted) == 1 && players[i].movesPredicted >= 1 && players[i].movesPredicted <= 99) {
                    validInput = 1;
                } else {
                    printf("Entrée invalide. Veuillez entrer un entier \n");
                    while (getchar() != '\n'); // Vider le buffer d'entrée
                }
            }
        }

        
        // Déplacements des joueurs et calcul des résultats
        for (int i = 0; i < numPlayers; i++) {
            int movesTaken = 0;
            int targetReached = 0;
            int tempoX, tempoY;
            
        
            while (targetReached == 0 ) {
                // Afficher la grid
                displaygrid(gridSize);
                printf("Manche %d\n\nà vous de jouer !\n\n", round);
                
                // Demander le déplacement au joueur
                char direction;
                printf("Joueur %d - Déplacement (haut/h, bas/b, gauche/g, droite/d, forfait/f) : \n", i+1);
                scanf(" %c", &direction);
                
                // Déplacer le joueur en ligne droite
                movePlayer(&players[i], direction, gridSize, players, targetReached); 
                movesTaken++;
                
                if (direction == 'f'|| direction == 'F'){   // si déclaré forfait
                    targetReached = 2;
                    printf("Tu as déclaré forfait, dommage.\n\n");
                break;
                }
                
                if (direction !='h' && direction !='H' && direction !='g' && direction !='G' && direction !='d' && direction !='D' && direction !='f' && direction !='F' && direction !='b' && direction !='B' ) {
                    printf("ATTENTION, coup invalide, rejouez\n\n");
                    struct timespec req;
                    req.tv_sec = 2;
                    req.tv_nsec = 0;
                    nanosleep(&req, NULL);
                    movesTaken--;
                }

                // Vérifier s'il atteint la cible
                if (grid[players[i].pos.x][players[i].pos.y] == players[i].target) {
                    targetReached = 1;
                    printf("BRAVO ! vous avez atteint la cible !\n\n");
                    break;
                }
            }

            // Calcul des résultats
            players[i].movesTaken = movesTaken; // Enregistrer le nombre de coups réels

            if (targetReached == 1 && movesTaken == players[i].movesPredicted) {
                roundScores[i] += 2; // Marquer 2 points pour une prédiction correcte
                
            } else if (targetReached == 1 && movesTaken < players[i].movesPredicted) {
                roundScores[i] -= 1; // Moins 1 point pour moins de coups que prévu 
            } else if (targetReached == 1 && movesTaken > players[i].movesPredicted || targetReached == 2) {
                for (int j = 0; j < numPlayers; j++) {
                    roundScores[j] += 1; // Marquer 1 point pour les autres joueurs
                }
            }
        }
    }

    // Afficher le score final et déterminer le gagnant
    printf("\nScore Final :\n");
    int maxScore = roundScores[0];
    int winner = 0;
    for (int i = 0; i < numPlayers; i++) {
        printf("Joueur %d : %d points\n", i+1, roundScores[i]);
        if (roundScores[i] > maxScore) {
            maxScore = roundScores[i];
            winner = i;
        }
    }
    printf("Le Joueur %d remporte la partie !\n", winner + 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    int gridSize, numPlayers, numRounds, targetReached;
    srand(time(NULL));
    
    printf("\nBienvenue dans le jeu !\n\n");
    
    printf("Entrez le nombre de joueurs (max %d): ", MAX_PLAYERS);
    scanf("%d", &numPlayers);
    printf("Entrez le nombre de manches: ");
    scanf("%d", &numRounds);
    
    
    while (numPlayers < 1 || numPlayers > MAX_PLAYERS || numRounds < 1 || numRounds > 99) {
        printf("\n\nSaisie invalide, veuillez recommencer \n\n\n");
        printf("Entrez le nombre de joueurs (max %d): ", MAX_PLAYERS);
        scanf("%d", &numPlayers);
        printf("Entrez le nombre de manches: ");
        scanf("%d", &numRounds);
    }

    playGame(gridSize, numPlayers, numRounds);

    return 0;
}
