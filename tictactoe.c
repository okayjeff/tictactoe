#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BSIZE 3


// Data structures
enum CellState {Empty, X, O};
enum game_status {Started, Won, Stalemate};

typedef int board_t[BSIZE][BSIZE];

struct Move {
    int row, col;
};

// Game defaults
int ActivePlayer = X;
int game_status = Started;

void PrintGameBoard(board_t board) {
    printf("\n");
    int square_no = 1; // Counter to track number of squares
    for (int row = 0; row < BSIZE; row++) {
        for (int col = 0; col < BSIZE; col++) {
            int square = board[row][col];
            if (square == Empty) {
                printf("[%d]", square_no);
            } else if (square == X) {
                printf("[x]");
            } else {
                printf("[o]");
            }
            square_no++;
        }
        printf("\n");
    }
    printf("\n");
}

struct Move translateSquareToMove(int square) {
    // Translate an integer square number to a 
    // move with valid coordinates.

    // Whole number is the row; modulus the columns.
    int indexSquare = square - 1;  // So we can index on the board array
    double position = indexSquare/BSIZE;

    int row = (int)position;
    int col = indexSquare % BSIZE;
    struct Move move = {row, col};
    return move;
}

int TranslateMoveToSquare(struct Move move) {
    int square = 1 + (BSIZE * move.row) + move.col;
    return square;
}

void ToggleActivePlayer(enum CellState lastMovePlayed) {
    if (lastMovePlayed == X) {
        ActivePlayer = O;
    } else {
        ActivePlayer = X;
    }
}

int checkForWinner(board_t board) {
    // Check for 3 in a row
    return 0;
}

int checkForStalemate(board_t board) {
    // Are there no available squares?
    for (int row = 0; row < BSIZE; row++) {
        for (int col = 0; col < BSIZE; col++) {
            if (board[row][col] == Empty) {
                return 0;
            }
        }
    }
    return 1;
}

void CheckGameStatus(board_t board) {
    //Check for available squares and 3 in a row...
    if (checkForWinner(board) == 1) {
        game_status = Won;
        printf("We have a winner!\n");
    }
    if (checkForStalemate(board) == 1) {
        game_status = Stalemate;
        printf("Bummer. No winner today.\n");
    }
}

int checkForLegalMove(board_t board, struct Move move) {
    int row = move.row;
    int col = move.col;

    if (0 <= row && row < BSIZE) {
        if (0 <= col && col < BSIZE) {
            int cell = board[move.row][move.col];

            if (cell != Empty) {
                printf("Square already taken. Make another move.\n");
                return 0;
            }
            return 1;
        }
    }
    printf("Must choose a valid square. Try again.\n");
    return 0;
}

void commitMove(board_t board, struct Move move, enum CellState state) {
    board[move.row][move.col] = state;
}

struct Move generateMove() {
    int seed = rand() % 1000;

    srand(time(NULL) + seed);
    int row = rand() % BSIZE;

    srand(time(NULL) + seed);
    int col = rand() % BSIZE;

    struct Move move = {row, col};
    return move;
}

int MakeMove(board_t board, struct Move move, enum CellState state) {
    int legalMove = checkForLegalMove(board, move);
    if (legalMove) {
        commitMove(board, move, state);
        ToggleActivePlayer(state);
        PrintGameBoard(board);
        CheckGameStatus(board);
        return 1;
    }
    return 0;
}

struct Move GetPlayerMove() {
    int square;
    printf("Your move. Choose an available square 1-9.\n");
    scanf("%d", &square);
    return translateSquareToMove(square);
}

int main() {
    // Create a board
    board_t board = {0};
    PrintGameBoard(board);

    while (game_status == Started) {
        if (ActivePlayer == X) {
            struct Move playerMove = GetPlayerMove();
            MakeMove(board, playerMove, ActivePlayer);    
        } else {
            struct Move cpuMove = generateMove();
            MakeMove(board, cpuMove, ActivePlayer);
        }
    }
    return 0;
}
