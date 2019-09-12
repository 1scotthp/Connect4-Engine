//
// Created by 1scot on 5/29/2019.
//

#ifndef CONNECT4_GAME_H
#define CONNECT4_GAME_H

#include "Board.h"


class Game {
public:
    Game();

    static int getxMove();

    int getyMove(int x, Board &board);

    void makeMove();

    char getMarker();

    int FindBestMove(Board &gameBoard);

    double miniMax(Board boardCopy, int depth, int alpha, int beta);

    static double evalOf(Board &boardCopy);

    void runGame();

private:
    int const searchOrder[7] = {3,2,4,5,1,6,0}; //order for checking moves

    Board gameBoard;
    //Board boardCopy;
};


#endif //CONNECT4_GAME_H
