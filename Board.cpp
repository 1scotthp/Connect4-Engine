//
// Created by 1scot on 5/29/2019.
//

#include "Board.h"


Board::Board() {

    for (auto & y : board) {
        for (char & x : y) {
            x = E;
        }
    }
}

void Board::setBoard(int x, int y, char mark) {
    board[ROWS - y - 1][x] = mark;
    humanTurn = !humanTurn;
    emptySquares -= 1;
}

void Board::printBoard() {
    for (auto & y : board) {
        for (int x = 0; x < COLUMNS - 1; x++) {
            cout << y[x] << "  |  ";
        }
        cout << y[COLUMNS - 1];
        cout << "\n" << "-------------------------------------" << "\n";
    }
    cout << " 0 ";
    for (int z = 1; z < COLUMNS - 1; z++) {
        cout << "   " << z << "  ";
    }
    cout << "  6\n\n";
}

/**
 * lookForWinner -
 * This method checks to see if there are 4 in a row
 * @param mark - the character (X or O) to be checked
 * @return bool that's true if there's 4 in a row
 */
bool Board::lookForWinner(char mark) {
    //Check horizontals
    for (auto & y : board) {
        for (int x = 0; x < COLUMNS - 3; x++) {
            if (y[x] == y[x + 1] && y[x + 1] == y[x + 2]
                && y[x + 2] == y[x + 3] && y[x + 3] == mark) {
                return true;
            }
        }
    }

    //Check verticals
    for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS - 3; y++) {
            // Check verticals
            if (board[y][x] == board[y + 1][x] && board[y + 1][x] == board[y + 2][x]
                && board[y + 2][x] == board[y + 3][x] && board[y + 3][x] == mark) {
                return true;
            }
        }
    }

    //Check down diagonals
    for (int y = 0; y < ROWS - 3; y++) {
        for (int x = 0; x < COLUMNS - 3; x++) {
            if (board[y][x] == board[y + 1][x + 1] && board[y + 1][x + 1] == board[y + 2][x + 2]
                && board[y + 2][x + 2] == board[y + 3][x + 3] && board[y + 3][x + 3] == mark) {
                return true;
            }
        }
    }

    //Check up diagonals
    for (int y = ROWS - 1; y > 2; y--) {
        for (int x = 0; x < COLUMNS - 3; x++) {
            if (board[y][x] == board[y - 1][x + 1] && board[y - 1][x + 1] == board[y - 2][x + 2]
                && board[y - 2][x + 2] == board[y - 3][x + 3] && board[y - 3][x + 3] == mark) {
                return true;
            }
        }
    }
    return false;
}

/*int Board::check3s(char mark){
    int num3s = 0;
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLUMNS - 2; x++) {
            // Check horizontals
            if (board[y][x] == board[y][x + 1] && board[y][x + 1] == board[y][x+2]
                && board[y][x+2]  == mark){
                num3s++;
            }
        }
    }

    for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS - 2; y++) {
            // Check horizontals
            if (board[y][x] == board[y + 1][x] && board[y +1 ][x] == board[y +2][x]
                && board[y +2][x] ==mark) {
                num3s++;
            }
        }
    }

    for (int y = 0; y < ROWS - 2; y++) {
        for (int x = 0; x < COLUMNS - 2; x++) {
            // Check horizontals
            if (board[y][x] == board[y + 1][x + 1] && board[y +1 ][x + 1] == board[y +2][x+2]
                && board[y +2][x+2] ==mark){
                num3s++;
            }
        }
    }

    for (int y = ROWS; y > 2; y--) {
        for (int x = 0; x < COLUMNS - 3; x++) {
            if (board[y][x] == board[y - 1][x + 1] && board[y -1][x + 1] == board[y -2][x+2]
                && board[y-2][x+2] ==mark){
                num3s++;
            }
        }
    }
    return num3s;
}*/

bool Board::spaceIsEmpty(int x, int y) {
    return board[ROWS - y - 1][x] == E;
}

/**
 * checkCol()-
 * This method checks if a column can be played in
 * @param x - column number to be checked
 * @return true if column has a space available
 */

bool Board::checkCol(int numCol) {
    return board[0][numCol] == E;
}

bool Board::lookForTie() {
    bool tie = true;
    for (int x = 0; x < COLUMNS; x++) {
        if (board[0][x] == E) {
            tie = false;
        }
    }
    return tie;
}

/**
 * positionPoints() -
 * This method attempts to evaluate a static position by caculating how many possible 4 in a row
 * combinations each player's pieces are a part of. It checks every possible set of 4 in a row
 * and adds a point for every computer piece in each set, subtracts for every human piece in each set
 *
 * (so it turns into just that pieces in the middle are more valuable than pieces on the edges)
 * @return int for position evaluation
 */
double Board::positionPoints() {
    double count = 0; //this counts up for every comp piece and down for human piece
    //char setOf4[4];

    for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS; y++) {
            if(board[y][x] == X){
                count = count - rowVal[y] - colVal[x];
            } else if (board[y][x] == O){
                count = count + rowVal[y] + colVal[x];
            }
        }
    }


    for(int y = 1; y < ROWS; y++){
        for(int x = 0; x < COLUMNS - 3; x++){
            if(board[y][x] == board[y][x + 1] && board[y][x+2] == board[y][x+1]
            && board[y][x + 3] == E){
                if(board[y][x] == X){
                    if(humanTurn == (emptySquares % 2 == 1)){
                        return 1000;
                    } //human turn odd wins
                } else if(board[y][x] == O){
                    if(!humanTurn == (emptySquares % 2 == 1)){
                        return -1000;
                    }
                }
            }
        }
    }
    //threes
    /*
    for (auto & y : board) {
        for (int x = 0; x < COLUMNS - 3; x++) {
            setOf4[0] = y[x];
            setOf4[1] = y[x + 1];
            setOf4[2] = y[x + 2];
            setOf4[3] = y[x + 3];
            for (char i : setOf4) {
                if (i == X) {
                    count--;
                } else if (i == O) {
                    count++;
                }
            }
        }
    }

    for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS - 3; y++) {
            setOf4[0] = board[y][x];
            setOf4[1] = board[y + 1][x];
            setOf4[2] = board[y + 2][x];
            setOf4[3] = board[y + 3][x];
            for (char i : setOf4) {
                if (i == X) {
                    count--;
                } else if (i == O) {
                    count++;
                }
            }
        }
    }

    for (int y = 0; y < ROWS - 3; y++) {
        for (int x = 0; x < COLUMNS - 3; x++) {
            setOf4[0] = board[y][x];
            setOf4[1] = board[y + 1][x + 1];
            setOf4[2] = board[y + 2][x + 2];
            setOf4[3] = board[y + 3][x + 3];
            for (char i : setOf4) {
                if (i == X) {
                    count--;
                } else if (i == O) {
                    count++;
                }
            }
        }
    }

    for (int y = ROWS; y > 3; y--) {
        for (int x = 0; x < COLUMNS - 3; x++) {
            setOf4[0] = board[y][x];
            setOf4[1] = board[y - 1][x + 1];
            setOf4[2] = board[y - 2][x + 2];
            setOf4[3] = board[y - 3][x + 3];
            for (char i : setOf4) {
                if (i == X) {
                    count--;
                } else if (i == O) {
                    count++;
                }
            }
        }
    } */
    return count;
}


