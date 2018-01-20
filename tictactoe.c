#include <stdio.h>
#include <stdlib.h>

#define BSIZE 3


// Data structures
enum game_status {Started, Won, Stalemate};

typedef int board_t[BSIZE][BSIZE];

typedef struct {
    int row, col;
    enum {Empty, X, O} player;
} move_t;

// Game vars
int active_player;
int game_status;

void PrintGameBoard(board_t board) {
    printf("\n");
    int square_no = 1; // Counter to track number of squares
    for (int row = 0; row < BSIZE; row++) {
        for (int col = 0; col < BSIZE; col++) {
            int square = board[row][col];                
            if (square == X) {
                printf("[x]");
            } else if (square == O) {
                printf("[o]");
            } else {
                printf("[%i]", square_no);
            }
            square_no++;
        }
        printf("\n");
    }
    printf("\n");
}

move_t TranslateSquareToMove(int square_no) {
    // Translate an integer square number to a 
    // move with valid coordinates.

    // Whole number is the row; modulus the columns.
    int index_square = square_no - 1;  // So we can index on the board array
    double position = index_square/BSIZE;

    int row = position;
    int col = index_square % BSIZE;
    move_t move = {row, col, active_player};
    return move;
}

int TranslateMoveToSquare(move_t move) {
    int square = 1 + (BSIZE * move.row) + move.col;
    return square;
}

void ToggleActivePlayer(player) {
    if (player == X) {
        active_player = O;
    } else {
        active_player = X;
    }
}

int CheckForWinner(board_t board) {
    // Get all the square numbers each player occupies,
    // translate them to moves and then determine if
    // any player has 3 in a row.
    int x_squares[BSIZE*BSIZE];
    int o_squares[BSIZE*BSIZE];
    int x_nxt_pos = 0;  // Keep track of next free pos in arr
    int o_nxt_pos = 0;  // Keep track of next free pos in arr

    for (int row = 0; row < BSIZE; row++) {
        for (int col = 0; col < BSIZE; col++) {
            if (board[row][col] == X) {
                move_t move = {row, col, active_player};
                int sq_no = TranslateMoveToSquare(move);
                x_squares[x_nxt_pos] = sq_no;
                x_nxt_pos++;
            } else if (board[row][col] == O) {
                move_t move = {row, col, active_player};
                int sq_no = TranslateMoveToSquare(move);
                o_squares[o_nxt_pos] = sq_no;
                o_nxt_pos++;
            } 
        }
    }
    // Check arrays for any winning combination of squares...
    return 0;
}

int CheckForStalemate(board_t board) {
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
    if (CheckForWinner(board) == 1) {
        game_status = Won;
        printf("We have a winner!\n");
    }
    if (CheckForStalemate(board) == 1) {
        game_status = Stalemate;
        printf("Bummer. No winner today.\n");
    }
}

int CheckForLegalMove(board_t board, move_t move) {
    int row = move.row;
    int col = move.col;

    if (0 <= row && row < BSIZE) {
        if (0 <= col && col < BSIZE) {
            int square = board[move.row][move.col];

            if (square != Empty) {
		if (active_player == X) {    
                    printf("Square already taken. Make another move.\n");
		}
                return 0;
            }
            return 1;
        }
    }
    printf("Must choose a valid square. Try again.\n");
    return 0;
}

void commitMove(board_t board, move_t move) {
    board[move.row][move.col] = move.player;
}

/* Choose the first available square */
move_t GenerateMove(board_t board) {
    move_t move;
    for (int row = 0; row < BSIZE; row++) {
        for (int col = 0; col < BSIZE; col++) {
	    if (board[row][col] == Empty) {
		move.row = row;
		move.col = col;
		move.player = active_player;
		printf("O plays square %d\n", TranslateMoveToSquare(move));
	        row = col = BSIZE; break;  // Break out of nested loops.
	    }
	}
    }

    return move;
}

int MakeMove(board_t board, move_t move) {
    int legalMove = CheckForLegalMove(board, move);
    if (legalMove) {
        commitMove(board, move);
        ToggleActivePlayer(active_player);
        PrintGameBoard(board);
        CheckGameStatus(board);
        return 1;
    }
    return 0;
}

move_t GetPlayerMove() {
    int square;
    printf("Your move. Choose an available square 1-9.\n");
    scanf("%d", &square);
    return TranslateSquareToMove(square);
}

int main() {
    board_t board = {0};
    active_player = X;
    game_status = Started;

    PrintGameBoard(board);

    while (game_status == Started) {
        if (active_player == X) {
            move_t player_move = GetPlayerMove();
            MakeMove(board, player_move);
        } else {
            move_t cpu_move = GenerateMove(board);
            MakeMove(board, cpu_move);
        }
    }
    return 0;
}
