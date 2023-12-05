
// piskvorky1.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


        if (*(p + y * size + x) == SPACE ){ //*(p + y * size + x) ukazatel na aktualne zadane policko
            *(p + y * size + x) = player;                           // p - ukazatel na hraciu plochu
            break;                                                  
        }
        else {
            printf("Cell has already been set.\n");
        }
    }

    // Set cell
    //*(p + y * size + x) = player;

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


    char player1[10];
    char player2[10];

    // Intro
    printf("Welcome to Tic Tac Toe in C!\n");

    // Get settings

    // Players
    printf("Zadaj meno prveho hraca: ");
    scanf("%10s", player1);
    printf("Zadaj meno druheho hraca: ");
    scanf("%10s", player2);

    printf("hrac 1: %s \n hrac 2: %s \n", player1, player2);
    
    

    // Board size
    printf("Choose board size (1-9): ");
    scanf("%ls", &size);
    printf("velkost je %cx%c\n", size, size);
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

    // Generate the board
    pboard = (char*)malloc(size * size * sizeof(char));  // Change the type of pboard to char*

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            *(pboard + i * size + j) = SPACE;
        }
    }

    
    printBoard(pboard, size);

    char xo;
    int u = 0;
    
    while(1){


        if(checkWin(pboard, size) != 0){
            winner = xo;
            break;
        }

        if(u % 2 == 0){
            xo = X;
        } else {
            xo = O;
        }

        turn(pboard, xo, size);
        printBoard(pboard, size);
        checkWin(pboard, size);

        

        u++;

    }
    

    printBoard(pboard, size);
    free(pboard);  // Change the function to free the memory

    // Declare winner!
    if (winner == 0) {
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
