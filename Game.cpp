//
// Created by Scott Phillips on 5/29/2019.
//

#include "Game.h"
#include <iostream>

/**
 * Game()
 * This constructor initializes a board and fills the searchOrder array
 */
Game::Game():gameBoard(Board()) {
}

/**
 * getxMove()
 * This method prompts the user for their move
 * @return xMove - int column of the user's move
 **/
int Game::getxMove() {
    std::cout << "Enter the x coordinate of your move (leftmost column is 0)\n";
    int xMove;
    std::cin >> xMove;
    std::cout << "\n";
    return xMove;
}

/**
 * getyMove()
 * This method returns the y coordinate that the disc would fall to in a given column
 * @param x - column of player's move
 * @param board - board that move is to be made on
 * @return - int y location of move or -1 if no move is possible in the given column
 */
int Game::getyMove(int x, Board &board) {
    for (int y = 0; y < Board::ROWS; y++) {
        if (board.spaceIsEmpty(x, y)) {
            return y;
        }
    }
    return -1;
}

char Game::getMarker() {
    if (gameBoard.humanTurn) {
        return Board::X;
    } else {
        return Board::O;
    }
}

/**
 * makeMove()
 * This method gets the move by either prompting the user or calling FindBestMove(), then
 * makes that move on gameboard
 */
void Game::makeMove() {
    if (gameBoard.humanTurn) {
        int x = getxMove();
        int y = getyMove(x, gameBoard);
        while (y == -1) {
            std::cout << "Invalid move, try again\n";
            x = getxMove();
            y = getyMove(x, gameBoard);
        }
        gameBoard.setBoard(x, y, getMarker());
    } else {
        int x = FindBestMove(gameBoard);
        int y = getyMove(x, gameBoard);
        gameBoard.setBoard(x, y, getMarker());
    }

}

/**
 * FindBestMove()
 * This method finds the best move by calling minimax
 * @return int representing the column the computer should play in
 */
int Game::FindBestMove(Board &gameBoard) {
    //Board boardCopy = gameBoard;
    int alpha = -10000;
    int beta = 10000;
    int depth;

    if(gameBoard.getEmptySquares() > 39){
        return 3;
    }

    if(gameBoard.getEmptySquares() > 33){
        depth = 9;
    } else if(gameBoard.getEmptySquares() > 27){
        depth = 11;
    } else {
        depth = 13;
    }

    //moves ahead to calculate
    int bestMove = -1;
    int maxEval = -15000;

    for (int x = 0; x < Board::COLUMNS; x++) {

        //while the column is full and there is another column, increment column #
        while (x < Board::COLUMNS && !gameBoard.checkCol(searchOrder[x])) {
            x++;
        }
        if (x == Board::COLUMNS) {//break bc this column doesn't exist
            break;
        }
        int currentRow = getyMove(searchOrder[x], gameBoard);
        gameBoard.setBoard(searchOrder[x], currentRow, Board::O);
        int eval = miniMax(gameBoard, depth - 1, alpha, beta);
            maxEval = max(maxEval, eval);
        if (eval == maxEval) {
            bestMove = searchOrder[x];
        }
        gameBoard.setBoard(searchOrder[x], currentRow, Board::E);
    }

    cout << maxEval << "\n";
    if(maxEval > 1000){
        cout<<"You're gonna lose"<<"\n";
    }

    return bestMove;
}

/**
 * This method generates a static evaluation of a position
 * @param boardCopy - board for analyzing future moves
 * @return int - the static eval of the position
 */
double Game::evalOf(Board &boardCopy) {
    if (boardCopy.lookForWinner(Board::O)) {
        return 10000;
    } else if (boardCopy.lookForWinner(Board::X)) {
        return -10000;
    } else {
        return boardCopy.positionPoints();
    }
}

/**
 * minimax()
 * This method is a minimax algorithm
 * @param boardCopy -  board for analyzing future moves
 * @param depth - int depth of search tree
 * @param humanTurn - bool thats true if it is player's turn to play
 * @param alpha - for pruning
 * @param beta - for pruning
 * @return
 */
int Game::miniMax(Board &gameBoard, int depth, int alpha, int beta) {
    //if depth has been reached or there is a winner, stop and return evaluation
    if (depth == 0 || gameBoard.lookForWinner(Board::O) || gameBoard.lookForWinner(Board::X)) {
        return evalOf(gameBoard);
    }

    if (!gameBoard.humanTurn) {
        int maxEval = -15000;
        for (int x = 0; x < Board::COLUMNS; x++) {
            //while the column is full and there is another column, increment column #
            while (x < Board::COLUMNS && !gameBoard.checkCol(searchOrder[x])) {
                x++;
            }
            if (x == Board::COLUMNS) {
                break;
            }
            int currentRow = getyMove(searchOrder[x], gameBoard);
            gameBoard.setBoard(searchOrder[x], currentRow, Board::O);
            int eval = miniMax(gameBoard, depth - 1, alpha, beta);
            maxEval = max(maxEval, eval);

            //pruning
            alpha = max(alpha, eval);
            if (beta <= alpha) {
                gameBoard.setBoard(searchOrder[x], currentRow, Board::E);
                break;
            }
            gameBoard.setBoard(searchOrder[x], currentRow, Board::E);
        }

        return maxEval;

    } else {
        int minEval = 15000;
        for (int x = 0; x < Board::COLUMNS; x++) {
            while (x < Board::COLUMNS && !gameBoard.checkCol(searchOrder[x])) {
                x++;
            }
            if (x == Board::COLUMNS) {
                break;
            }
            int currentRow = getyMove(searchOrder[x], gameBoard);
            gameBoard.setBoard(searchOrder[x], currentRow, Board::X);
            int eval = miniMax(gameBoard, depth - 1, alpha, beta);
            minEval = min(minEval, eval);

            //pruning
            beta = min(beta, eval);
            if (beta <= alpha) {
                gameBoard.setBoard(searchOrder[x], currentRow, Board::E);
                break;
            }
            gameBoard.setBoard(searchOrder[x], currentRow, Board::E);//reset board after testing each move
        }
        return minEval;
    }
}

void Game::runGame() {
    while (!gameBoard.lookForWinner(Board::O) && !gameBoard.lookForWinner(Board::X)
           && !gameBoard.lookForTie()) {
        if (gameBoard.humanTurn) {
            gameBoard.printBoard();
        }
        makeMove();
    }
    gameBoard.printBoard();
    if (gameBoard.lookForWinner(Board::O)) {
        cout << "Game Over! You lost";
    } else {
        cout << "Game Over! You won";
    }
    //void callMiniMax();
}


