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

    char symbols[3] = {'X', 'O', 'Z'};
    int roles[3]; 

    if (mode == 1) {
        roles[0] = roles[1] = 0;
        roles[2] = -1;
    } else if (mode == 2) {
        roles[0] = 0;
        roles[1] = 1;
        roles[2] = -1;
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

    while (1) {
        displayBoard(board, size);

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

        board[row][col] = symbols[turn];

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

        if (isDraw(board, size)) {
            displayBoard(board, size);
            printf("It's a draw!\n");
            writeScoreToFile("Draw");
            break;
        }

        turn = (mode == 3) ? (turn + 1) % 3 : 1 - turn;
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
