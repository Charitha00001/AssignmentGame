#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 10    // maximum grid size allowed


char** initializeBoard(int size);  //allocating and initializing game board
void displayBoard(char** board, int size);  //display curerent board state
int getUserMove(int* row, int* col, int size);  //getting the user input moves
int isValidMove(char** board, int row, int col, int size);  // validity of the move checking
int checkWin(char** board, int size, char symbol);  //if  a player won
int isDraw(char** board, int size);  //if game came to a draw
void writeScoreToFile(const char* result);
void getComputerMove(char** board, int size, int* row, int* col);

int main() {
    int size, mode;
    srand(time(NULL));  //random number generating

    printf("Enter grid size (3 to 10): ");
    scanf("%d", &size);
    if (size < 3 || size > MAX_SIZE) {
        printf("Invalid grid size. Exiting.\n");
        return 1;
    }

    
    printf("Select mode:\n");
    printf("1. User vs User\n");
    printf("2. User vs Computer\n");
    printf("3. Three Players\n");
    printf("Enter choice: ");
    scanf("%d", &mode);
    if (mode < 1 || mode > 3) {
        printf("Invalid mode. Exiting.\n");
        return 1;
    }

    char symbols[3] = {'X', 'O', 'Z'};  //symbols which are players getting
    int roles[3];   
    //assigning roles based on the mode player choose
    if (mode == 1) {  
        roles[0] = roles[1] = 0;  //if both are human
        roles[2] = -1;    //third is not used
    } else if (mode == 2) {
        roles[0] = 0;  //user vs computer scenario
        roles[1] = 1;
        roles[2] = -1;  //third is not used
    } else {
        for (int i = 0; i < 3; i++) {
            printf("Select role for Player %c:\n1. Human\n2. Computer\nEnter choice: ", symbols[i]);
            scanf("%d", &roles[i]);
            if (roles[i] != 1 && roles[i] != 2) {
                printf("Invalid role. Exiting.\n");
                return 1;
            }
            roles[i] -= 1;
        }
    }

    char** board = initializeBoard(size);
    int turn = 0;
    int row, col;
    //main loop 
    while (1) {
        displayBoard(board, size);
    //determines move base on the role
        if (roles[turn] == 0) {
            printf("Player %c's turn.\n", symbols[turn]);
            while (!getUserMove(&row, &col, size) || !isValidMove(board, row, col, size)) {
                printf("Invalid move. Try again.\n");
            }
        } else if (roles[turn] == 1) {
            printf("Computer's turn (%c)...\n", symbols[turn]);
            getComputerMove(board, size, &row, &col);
            printf("Computer chose: %d %d\n", row, col);
        }

        board[row][col] = symbols[turn];   //apply move
        //checking for a winnig situation
        if (checkWin(board, size, symbols[turn])) {
            displayBoard(board, size);
            if (roles[turn] == 1)
                printf("Computer (%c) wins!\n", symbols[turn]);
            else
                printf("Player %c wins!\n", symbols[turn]);

            writeScoreToFile(roles[turn] == 1 ? "Computer wins" :
                             symbols[turn] == 'X' ? "Player X wins" :
                             symbols[turn] == 'O' ? "Player O wins" :
                             "Player Z wins");
            break;
        }
        //checking for whether it is a draw
        if (isDraw(board, size)) {
            displayBoard(board, size);
            printf("It's a draw!\n");
            writeScoreToFile("Draw");
            break;
        }
        //advancing the turn
        turn = (mode == 3) ? (turn + 1) % 3 : 1 - turn;
    }

    for (int i = 0; i < size; i++) free(board[i]);  //memory freeing.
    free(board);
    return 0;
}
//allocating and initializing the board with spaces
char** initializeBoard(int size) {
    char** board = (char**)malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        board[i] = (char*)malloc(size * sizeof(char));
        for (int j = 0; j < size; j++) board[i][j] = ' ';
    }
    return board;
}
//display board with showing grid lines
void displayBoard(char** board, int size) {
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf(" %c ", board[i][j]);
            if (j < size - 1) printf("|");
        }
        printf("\n");
        if (i < size - 1) {
            for (int j = 0; j < size; j++) {
                printf("---");
                if (j < size - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}
//prompting user for input
int getUserMove(int* row, int* col, int size) {
    printf("Enter row and column (0 to %d): ", size - 1);
    return scanf("%d %d", row, col) == 2;
}
//checking if the move is within limits and is it for an empty cell
int isValidMove(char** board, int row, int col, int size) {
    return row >= 0 && row < size && col >= 0 && col < size && board[row][col] == ' ';
}
//checking for a winnig situation
int checkWin(char** board, int size, char symbol) {
    int win;

    for (int i = 0; i < size; i++) {    //rows checking
        win = 1;
        for (int j = 0; j < size; j++) {
            if (board[i][j] != symbol) { win = 0; break; }
        }
        if (win) return 1;
    }

    for (int j = 0; j < size; j++) {  //colomn checking
        win = 1;
        for (int i = 0; i < size; i++) {
            if (board[i][j] != symbol) { win = 0; break; }
        }
        if (win) return 1;
    }

    win = 1;
    for (int i = 0; i < size; i++) {   //checking main diagonal
        if (board[i][i] != symbol) { win = 0; break; }
    }
    if (win) return 1;

    win = 1;
    for (int i = 0; i < size; i++) {   //checking anti diagonal
        if (board[i][size - i - 1] != symbol) { win = 0; break; }
    }
    return win;
}
//if all cells filled
int isDraw(char** board, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == ' ') return 0;
    return 1;
}

void writeScoreToFile(const char* result) {
    FILE* file = fopen("tictactoe_log.txt", "a");
    if (file == NULL) {
        printf("Error writing to log file.\n");
        return;
    }
    fprintf(file, "%s\n", result);
    fclose(file);
}
//randomly selecting a move for computer's turn
void getComputerMove(char** board, int size, int* row, int* col) {
    do {
        *row = rand() % size;
        *col = rand() % size;
    } while (!isValidMove(board, *row, *col, size));
}
