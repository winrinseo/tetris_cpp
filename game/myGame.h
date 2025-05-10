#include <iostream>
#include <conio.h>
#include "gameInstance.h"


const int BOARD_WIDTH = 100;
const int BOARD_HEIGHT = 80;

const int MAP_WIDTH = 12;
const int MAP_HEIGHT = 22;

const int X_OFFSET = 6;
const int Y_OFFSET = 5;


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