//
// Created by Scott Phillips 5/29/2019.
//

#include "Board.h"


Board::Board() {
    for (auto &y : board) {
        for (char &x : y) {
            x = E;
        }
    }
}

void Board::setBoard(int x, int y, char mark) {
    board[ROWS - y - 1][x] = mark;
    humanTurn = !humanTurn;
    if (mark == E) {
        emptySquares++;
    } else {
        emptySquares--;
    }
}

void Board::printBoard() {
    for (auto &y : board) {
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
    for (auto &y : board) {
        for (int x = 0; x < COLUMNS - 3; x++) {
            if (y[x] == y[x + 1] && y[x + 1] == y[x + 2]
                && y[x + 2] == y[x + 3] && y[x + 3] == mark) {
                return true;
            }
        }
    }

    //Check verticals
    for (auto x = 0; x < COLUMNS; x++) {
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
            if (board[y][x] == X) {
                count = count - rowVal[y] - colVal[x];
            } else if (board[y][x] == O) {
                count = count + rowVal[y] + colVal[x];
            }
        }
    }

    int pieceCounter = 0;

    int x = 0;
    for (int y = 0; y < ROWS; y++) {
        while (x < COLUMNS) {
            if (board[y][x] == E && board[y][x+1] == E) { //computer token
                pieceCounter = 0; //no credit when multiple spaces in a row between pieces
            } else if (board[y][x] == X) {
                if (pieceCounter <= 0) {
                    pieceCounter--;
                } else { //previous token/s were comp tokens so reset
                    pieceCounter = -1;
                }
            } else { // piece=O
                if (pieceCounter >= 0) {
                    pieceCounter++;
                } else { //previous token/s were human tokens so reset
                    pieceCounter = 1;
                }
            }
            if (pieceCounter == 3) {
                count += BONUS3;
                break;
            } else if (pieceCounter == -3) {
                count -= BONUS3;
                break;
            }
            x++;
        }
        x = 0;//reset column
        pieceCounter = 0;
    }

    int y = 0;
    for (x = 0; x < COLUMNS; x++) {
        while (y < ROWS) {
            if (board[y][x] == E) {
                break;
            } else if (board[y][x] == X) {
                if (pieceCounter <= 0) {
                    pieceCounter--;
                } else { //previous token/s were comp tokens so reset
                    pieceCounter = -1;
                }
            } else { // = O
                if (pieceCounter >= 0) {
                    pieceCounter++;
                } else { //previous token/s were human tokens so reset
                    pieceCounter = 1;
                }
            }
            if (pieceCounter == 3) {
                count += BONUS3;
                break;
            } else if (pieceCounter == -3) {
                count -= BONUS3;
                break;
            }
            y++;
        }
        y = 0;//reset column
        pieceCounter = 0;
    }

    int startRow = ROWS - 1;
    int startCol = 0;
    while (startCol < 4) {
        count += checkDownDiag(startRow, startCol);
        startCol++;
    }
    startCol = 0;
    while (startRow > 2) {
        count += checkDownDiag(startRow, startCol);
        startRow--;
    }

    startRow = ROWS - 1;
    startCol = COLUMNS - 1;
    while (startCol > 2) {
        count += checkUpDiag(startRow, startCol);
        startCol--;
    }
    startCol = COLUMNS - 1;
    while (startRow > 2) {
        count += checkUpDiag(startRow, startCol);
        startRow--;
    }

    return count;
}


int Board::getEmptySquares() {
    return emptySquares;
}


int Board::checkDownDiag(int y, int x) {
    int pieceCounter = 0;
    int count = 0;

    while (x < COLUMNS && y >= 0) {
        if(y < 2 && pieceCounter==0){
            break;
        }
        if (board[y][x] == O) { //computer token
            if (pieceCounter >= 0) {
                pieceCounter++;
            } else { //previous token/s were human tokens so reset
                pieceCounter = 1;
            }
        } else if (board[y][x] == X) {
            if (pieceCounter <= 0) {
                pieceCounter--;
            } else { //previous token/s were comp tokens so reset
                pieceCounter = -1;
            }
        } else if (board[y - 1][x + 1] == E) {
            pieceCounter = 0;
        }
        if (pieceCounter == 3) {
            count += BONUS3;
            break;
        } else if (pieceCounter == -3) {
            count -= BONUS3;
            break;
        }
        x++;
        y--;
    }
    return count;
}

int Board::checkUpDiag(int y, int x) {
    int pieceCounter = 0;
    int count = 0;
    while (x >= 0 && y >= 0) {
        if(y<2 && pieceCounter==0){//can't get3 in a row
            break;
        }
        if (board[y][x] == O) { //computer token
            if (pieceCounter >= 0) {
                pieceCounter++;
            } else { //previous token/s were human tokens so reset
                pieceCounter = 1;
            }
        } else if (board[y][x] == X) {
            if (pieceCounter <= 0) {
                pieceCounter--;
            } else { //previous token/s were comp tokens so reset
                pieceCounter = -1;
            }
        } else if (board[y - 1][x - 1] == E) {
            pieceCounter = 0;
        }
        if (pieceCounter == 3) {
            count += BONUS3;
            break;
        } else if (pieceCounter == -3) {
            count -= BONUS3;
            break;
        }
        x--;
        y--;
    }
    return count;
}

