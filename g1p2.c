#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  


#define MAX_SIZE 10


char** initializeBoard(int size);
void displayBoard(char** board, int size);
int getUserMove(int* row, int* col, int size);
int isValidMove(char** board, int row, int col, int size);
int checkWin(char** board, int size, char symbol);
int isDraw(char** board, int size);
void writeScoreToFile(const char* result);


void getComputerMove(char** board, int size, int* row, int* col);

int main() {
    int size, mode;

    
    srand(time(NULL));  

    
    printf("Enter grid size (3 to 10): ");
    scanf("%d", &size);
    if (size < 3 || size > MAX_SIZE) {
        printf("Invalid grid size. Exiting.\n");
        return 1;
    }

    
    printf("Select mode:\n1. User vs User\n2. User vs Computer\nEnter choice: ");
    scanf("%d", &mode);
    if (mode != 1 && mode != 2) {
        printf("Invalid mode. Exiting.\n");
        return 1;
    }

    char** board = initializeBoard(size);
    int turn = 0; 
    int row, col;

    while (1) {
        displayBoard(board, size);
        if (mode == 1 || (mode == 2 && turn == 0)) {
            
            printf("Player %c's turn.\n", turn == 0 ? 'X' : 'O');
            while (!getUserMove(&row, &col, size) || !isValidMove(board, row, col, size)) {
                printf("Invalid move. Try again.\n");
            }
        } else {
           
            printf("Computer's turn (O)...\n");
            getComputerMove(board, size, &row, &col);
            printf("Computer chose: %d %d\n", row, col);
        }

        
        board[row][col] = turn == 0 ? 'X' : 'O';

        
        if (checkWin(board, size, board[row][col])) {
            displayBoard(board, size);
            if (mode == 2 && turn == 1)
                printf("Computer wins!\n");
            else
                printf("Player %c wins!\n", board[row][col]);

           
            writeScoreToFile(mode == 2 && turn == 1 ? "Computer wins" :
                             turn == 0 ? "Player X wins" : "Player O wins");
            break;
        }

        
        if (isDraw(board, size)) {
            displayBoard(board, size);
            printf("It's a draw!\n");
            writeScoreToFile("Draw");
            break;
        }

        turn = 1 - turn; 
    }

   
    for (int i = 0; i < size; i++) free(board[i]);
    free(board);

    return 0;
}

char** initializeBoard(int size) {
    char** board = (char**)malloc(size * sizeof(char*));
    for (int i = 0; i < size; i++) {
        board[i] = (char*)malloc(size * sizeof(char));
        for (int j = 0; j < size; j++) board[i][j] = ' ';
    }
    return board;
}


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


int getUserMove(int* row, int* col, int size) {
    printf("Enter row and column (0 to %d): ", size - 1);
    return scanf("%d %d", row, col) == 2;
}


int isValidMove(char** board, int row, int col, int size) {
    return row >= 0 && row < size && col >= 0 && col < size && board[row][col] == ' ';
}


int checkWin(char** board, int size, char symbol) {
    int win;

    
    for (int i = 0; i < size; i++) {
        win = 1;
        for (int j = 0; j < size; j++) {
            if (board[i][j] != symbol) { win = 0; break; }
        }
        if (win) return 1;
    }

    
    for (int j = 0; j < size; j++) {
        win = 1;
        for (int i = 0; i < size; i++) {
            if (board[i][j] != symbol) { win = 0; break; }
        }
        if (win) return 1;
    }

  
    win = 1;
    for (int i = 0; i < size; i++) {
        if (board[i][i] != symbol) { win = 0; break; }
    }
    if (win) return 1;

    
    win = 1;
    for (int i = 0; i < size; i++) {
        if (board[i][size - i - 1] != symbol) { win = 0; break; }
    }
    return win;
}


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



void getComputerMove(char** board, int size, int* row, int* col) {
    do {
        *row = rand() % size;
        *col = rand() % size;
    } while (!isValidMove(board, *row, *col, size));
}
