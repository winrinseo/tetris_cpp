#include <iostream>
#include <conio.h>
#include "gameInstance.h"
#include "block.h"


const int BOARD_WIDTH = 90;
const int BOARD_HEIGHT = 30;

const int MAP_WIDTH = 12;
const int MAP_HEIGHT = 23;

const int X_OFFSET = 5;
const int Y_OFFSET = 3;

const double SPEED = 0.6;


class myGameInstance : public GameInstance{

private:
    //게임판 
    //0 : 빈칸 , 1 : 현재 블록 , 2 : 블록 , 3 : 그림자 , 4 : 벽
    char ** board;

    void gameLoop() override;

    void gotoxy(int x,int y);
    void initTerminal();

    void initBoard();

    //테두리 그리는 함수
    void drawBorder();

    void drawMenu();
    void drawBoard();

};