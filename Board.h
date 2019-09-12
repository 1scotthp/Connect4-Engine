//
// Created by 1scot on 5/29/2019.
//

#ifndef CONNECT4_BOARD_H
#define CONNECT4_BOARD_H
#pragma once

#include <iostream>
#include <vector>

using namespace std;


class Board {

public:
    Board();

    static const int ROWS = 6;
    static const int COLUMNS = 7;

    static const char X = 'X';
    static const char O = 'O';
    static const char E = ' ';//Empty square
    bool humanTurn = true;

    void setBoard(int x, int y, char mark);

    void printBoard();

    bool lookForWinner(char mark);

    bool lookForTie();

    bool spaceIsEmpty(int x, int y);

    bool checkCol(int x);

    double positionPoints();

private:
    char board[ROWS][COLUMNS];//x is used for col# and y for row# in th{}is program
    int emptySquares = 42;//bad style

    int rowVal[6] = {1,2,2,1,0,0};
    int colVal[7] = {0,1,2,3,2,1,0};



};

/*int check3s(char mark);
    int positionPoints(char mark);*/


#endif //CONNECT4_BOARD_H
