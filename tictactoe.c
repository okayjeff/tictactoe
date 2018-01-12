#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 3


// Data structures
enum CellState {Empty, X, O};
enum GameStatus {Started, Won, Stalemate};

struct Move {
    int row, col;
};

// Game defaults
int ActivePlayer = X;
int gameStatus = Started;

void PrintGameBoard(int game_board[BOARD_SIZE][BOARD_SIZE]) {
    int square_no = 1;  // Counter to track number of squares
    int board_size = sizeof(game_board[0]) / sizeof(game_board[0][0]);

    for (int row = 0; row < board_size; row++) {
        for (int col = 0; col < board_size; col++) {
            int cell = game_board[row][col];
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

const char* getPlayerName(enum CellState activePlayer) {
    switch (activePlayer) {
        case X: return "X";
        case O: return "O";
        case Empty: return "X";
    }
}

void toggleActivePlayer(enum CellState lastMovePlayed) {
    if (lastMovePlayed == X) {
        ActivePlayer = O;
    } else {
        ActivePlayer = X;
    }
}

int checkForWinner(int game_board[BOARD_SIZE][BOARD_SIZE]) {
    // Check for 3 in a row
    return 0;
}

int checkForStalemate(int game_board[BOARD_SIZE][BOARD_SIZE]) {
    // Are there no available squares?
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (game_board[row][col] == Empty) {
                return 0;
            }
        }
    }
    return 1;
}

void checkGameStatus(int game_board[BOARD_SIZE][BOARD_SIZE]) {
    //Check for available squares and 3 in a row...
    if (checkForWinner(game_board) == 1) {
        gameStatus = Won;
        printf("We have a winner!\n");
    }
    if (checkForStalemate(game_board) == 1) {
        gameStatus = Stalemate;
        printf("Bummer. No winner today.\n");
    }
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
        PrintGameBoard(gameBoard);
        checkGameStatus(gameBoard);
        return 1;
    }
    return 0;
}

struct Move translateSquareToMove(int square) {
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
    printf("Enter your move (%s): \n", getPlayerName(ActivePlayer));
    scanf("%d", &square);
    return translateSquareToMove(square);
}



int main() {
    // Create a board
    int game_board[BOARD_SIZE][BOARD_SIZE];
    PrintGameBoard(game_board);

    while (gameStatus == Started) {
        

        if (ActivePlayer == X) {
            struct Move playerMove = getPlayerMove();
            makeMove(game_board, playerMove, ActivePlayer);    
        } else {
            struct Move cpuMove = generateMove();
            makeMove(game_board, cpuMove, ActivePlayer);
        }
    }
    return 0;
}
