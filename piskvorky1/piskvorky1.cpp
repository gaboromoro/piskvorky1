
// piskvorky1.cpp : Defines the entry point for the application.
//

#include "piskvorky1.h"
#include <stdio.h>
#include <stdlib.h>

#define X 88
#define O 79
#define SPACE 32

void printBoard(char* p, int s) {
    // Input the pointer to the board array and the size
    // Function prints out the current board status
    int i, j;
    for (i = 0; i < s; i++) {
        printf("\n");
        if (i > 0) {
            // Print horizontal dividers
            for (j = 0; j < s; j++) {
                if (j > 0) {
                    printf("----");
                }
                else {
                    printf("\t---");
                }
            }
        }
        else {
            // Print headers
            printf("\t");
            for (j = 0; j < s; j++) {
                if (j > 0) {
                    printf("  %c ", j + 65);
                }
                else {
                    printf(" %c ", j + 65);
                }
            }
            printf("\n");
        }
        printf("\n %i", i + 1);
        // Print row values
        for (j = 0; j < s; j++) {
            if (j > 0) {
                printf("|");
            }
            else {
                printf("\t");
            }
            printf(" %c ", *(p + i * s + j));
        }
    }
    printf("\n\n");
}

int turn(char* p, char player, int size) {
    // Get action command
    char action[3];
    int x, y;

    while (1) {
        printf("Player %c, place your mark:", player);
        scanf("%s", action);
        x = action[0];
        y = action[1];

        // Interrupt game by pressing q
        if (action[0] == 113 || action[0] == 81) {
            printf("Exiting the game...");
            exit(0);
        }
        // Validate input
        if (x >= 97) {
            x -= 32;
        }
        if (x > (64 + size) || x < 65) {
            printf("%c is an invalid value for x.\n", x);
            continue;
        }
        else if (y > 48 + size || y < 48) {
            printf("%c is an invalid value for y.\n", y);
            continue;
        }
        else {
            // Set to index
            x -= 65;
            y -= 49;
            // printf("x:%d, y:%d\n",x, y);
        }
        // Check if cell has already been set
        if (*(p + y * size + x) == SPACE) {
            break;
        }
        else {
            printf("Cell has already been set.\n");
        }
    }

    // Set cell
    *(p + y * size + x) = player;

    return 0;
}

int botTurn(char* p, char player, int size) {
    int i, j;       // Loop iterators
    int c, d;       // Secondary loop iterator
    int cv, l;      // Secondary loop counter
    int mult;       // Score multiplier
    int value;      // Cell's current value in loop
    int score;      // Cell's priority
    int highscore = 0;// Cell with the highest score.
    // Byte 1 is blank, 2 is x value, 3 is y value, 4 is score
    int selfScore = 0;// Count of the bot's own filled cells

    // Loop through rows
    for (i = 0; i < size; i++) {
        // Loop through cells
        for (j = 0; j < size; j++) {
            value = *(p + i * size + j);
            if (value == SPACE) {
                // If cell has not been chosen yet

                // Get coordinate score
                score = 1 + 1;                           // Add one for each row/column
                if (i == j) { score++; } // Add one for diagonal 1
                if (size - 1 - i == j) { score++; } // Add one for diagonal 2

                mult = 1 + (1 << 8) + (1 << 16) + (1 << 24);
                selfScore = 0;
                for (c = 0; c < size; c++) {
                    cv = (*(p + i * size + c) << 24) +        // Horizontal
                        (*(p + c * size + j) << 16);        // Vertical
                    if (i == j) {
                        cv += *(p + c * size + c) << 8;            // Diagonal 1
                    }
                    if (size - 1 - i == j) {
                        cv += *(p + (size - 1 - c) * size + c);    // Diagonal 2
                    }

                    // Loop through the 4 parameters
                    for (l = 0; l < 4; l++) {
                        // Only add score if it isn't blank or belonging to a the bot
                        if ((((cv >> (l * 8)) & 0xFF) != 0x20) && (((cv >> (l * 8)) & 0xFF) != player) && (((cv >> (l * 8)) & 0xFF) != 0)) {
                            score += (mult >> (l * 8)) & 0xFF;
                            mult += 2 << (l * 8);
                        }
                        // Check if there are many of our own cells filled
                        if (((cv >> (l * 8)) & 0xFF) == player) {
                            selfScore += 1 << (l * 8);
                        }
                        // If row/col/diag is missing just one cell, fill it.
                        if (selfScore >> (l * 8) == (size - 1)) {
                            score += 5;
                        }
                    }
                }

                // Check for a high score
                if (score > (highscore & 0xFF)) {
                    highscore = (score & 0xFF) | (i << 16 & 0xFF0000) | (j << 8 & 0xFF00);
                    // printf("%08x!", highscore);
                }
                else {
                    // printf("%08x ", score & 0xFF);
                }
            }
            else {
                // If cell has been set, just print who filled it
                //printf("%c        ", value);
            }
        }
    }
    // Set cell
    *(p + (highscore >> 16 & 0xFF) * size + (highscore >> 8 & 0xFF)) = player;
    return 0;
}

int checkWin(char* p, int size) {
    int i, j;        // Loop iterators
    int hx, ho;      // Horizontal counters
    int vx, vo;      // Vertical counters
    int hval, vval;  // Temp storage for stuff
    int dx1, dx2, do1, do2;
    int total = 0;
    dx1 = dx2 = do1 = do2 = 0;

    // Loop through rows and columns
    for (i = 0; i < size; i++) {
        // Reset
        hx = ho = vx = vo = 0;
        // Loop through cells
        for (j = 0; j < size; j++) {
            // Get values for rows and columns
            hval = *(p + i * size + j);
            vval = *(p + j * size + i);
            // Check for matches
            if (hval == X) {
                hx++;
            }
            if (hval == O) {
                ho++;
            }
            if (vval == X) {
                vx++;
            }
            if (vval == O) {
                vo++;
            }
            // Check diagonals
            // Downwards
            if (i == j) {
                if (hval == X) {
                    dx1++;
                }
                if (hval == O) {
                    do1++;
                }
            }
            // Upwards
            if (size - 1 - i == j) {
                if (hval == X) {// It's an X
                    dx2++;
                }
                if (hval == O) {// It's an O
                    do2++;
                }
            }
        }
        // printf("Straights; Row: %i, hx: %i, ho: %i, vx: %i, vo: %i.\n", i, hx, ho, vx, vo);
        // printf("Diagonals; Row: %i, dx1: %i, do1: %i, dx2: %i, do2: %i.\n", i, dx1, do1, dx2, do2);
        if (hx == size || vx == size || dx1 == size || dx2 == size) {
            return X;
        }
        else if (ho == size || vo == size || do1 == size || do2 == size) {
            return O;
        }
        total += hx + ho;
    }
    // Check if board is full
    // printf("Total: %i",total);
    if (total == size * size) {
        return 1;
    }
    return 0;
}

int main() {
    // Declare vars
    int players;
    int i, j;
    char* pboard;  // Change the type of pboard to char*
    int size;
    int winner = 0;
    char playerArray[] = { X, O };  // Change the type of playerArray to char
    int p;

    // Intro
    printf("Welcome to Tic Tac Toe in C!\n");

    // Get settings

    // Players
    printf("Choose players (1/2): ");
    players = getchar();
    if (players == 49 || players == 50) {
        players -= 48;
        getchar();
        printf("%i-player game.\n", players);
    }
    else if (players == 10) {
        players = 1;
        printf("Defaulted to %i player.\n", players);
    }
    else {
        printf("Please select only 1 or 2 players (%c).\n", players);
        return 0;
    }

    // Board size
    printf("Choose board size (1-9): ");
    size = getchar();
    if (size - 48 >= 1 && size - 48 <= 9) {
        // Good!
        size -= 48;
    }
    else if (size == 10) {
        size = 3;
        printf("Defaulted to a %ix%i board.\n", size, size);
    }
    else {
        printf("Please select a number between 1 and 9 (%c).\n", size);
        return 0;
    }
    printf("Playing a %i-player game on a %ix%i board.\n", players, size, size);

    // Generate the board
    pboard = (char*)malloc(size * size * sizeof(char));  // Change the type of pboard to char*
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            *(pboard + i * size + j) = SPACE;
        }
    }

    // Play the game!
    while (winner == 0) {
        for (p = 0; p < sizeof(playerArray) / sizeof(playerArray[0]); p++) {
            if (winner) {
                break;
            }
            if (players == 2 || (players == 1 && playerArray[p] == X)) {
                // If it is a human turn
                printBoard(pboard, size);
                turn(pboard, playerArray[p], size);
            }
            else {
                // If it's a bot turn
                botTurn(pboard, playerArray[p], size);
            }
            winner = checkWin(pboard, size);
        }
    }

    // Print final board
    printBoard(pboard, size);
    free(pboard);  // Change the function to free the memory

    // Declare winner!
    if (winner == 1) {
        printf("Tie!");
    }
    else {
        printf("%c wins the game!", winner);
    }
    printf(" Press any key to finish.");
    getchar();
    getchar();
    return 0;
}
