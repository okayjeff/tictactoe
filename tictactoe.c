#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 3


enum CellState {Empty, X, O};

struct Move {
    int row, col;
};

int ActivePlayer = X;

void toggleActivePlayer(enum CellState lastPlayer) {
    if (lastPlayer == X) {
        ActivePlayer = O;
    } else {
        ActivePlayer = X;
    }
}

void printGameBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    int square_no = 1;  // Counter to track number of squares
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            int cell = board[row][col];
            if (cell == Empty) {
                printf("[%d]", square_no);
            } else if (cell == X) {
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

int checkForLegalMove(int gameBoard[BOARD_SIZE][BOARD_SIZE], struct Move move) {
    int row = move.row;
    int col = move.col;

    if (0 <= row && row < BOARD_SIZE) {
        if (0 <= col && col < BOARD_SIZE) {
            int cell = gameBoard[move.row][move.col];

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

void commitMove(int gameBoard[BOARD_SIZE][BOARD_SIZE], struct Move move, enum CellState state) {
    gameBoard[move.row][move.col] = state;
}

struct Move generateMove() {
    int seed = rand() % 1000;

    srand(time(NULL) + seed);
    int row = rand() % BOARD_SIZE;

    srand(time(NULL) + seed);
    int col = rand() % BOARD_SIZE;

    struct Move move = {row, col};
    return move;
}

int makeMove(int gameBoard[BOARD_SIZE][BOARD_SIZE], struct Move move, enum CellState state) {
    int row = move.row;
    int col = move.col;

    int legalMove = checkForLegalMove(gameBoard, move);
    if (legalMove == 1) {
        commitMove(gameBoard, move, state);
        toggleActivePlayer(state);
        printGameBoard(gameBoard);
        return 1;
    }
    return 0;
}

struct Move translateMove(int square) {
    // Translate an integer square number to a 
    // move with valid coordinates.

    // Whole number is the row; modulus the columns.
    int indexSquare = square - 1;  // So we can index on the board array
    double position = indexSquare/BOARD_SIZE;

    int row = (int)position;
    int col = indexSquare % BOARD_SIZE;
    struct Move move = {row, col};
    return move;
}

struct Move getPlayerMove() {
    int square;
    printf("Enter your move: \n");
    scanf("%d", &square);
    return translateMove(square);
}

int main() {
    int gameBoard[BOARD_SIZE][BOARD_SIZE] = {0};
    printGameBoard(gameBoard);

    struct Move FakeMove2 = getPlayerMove();
    makeMove(gameBoard, FakeMove2, ActivePlayer);

    return 0;
}
