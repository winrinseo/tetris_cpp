/*
 전체적인 게임 로직은 이곳에서 정의
 
*/
#pragma once
#include <iostream>
#include <conio.h>
#include <cmath>
#include "gameInstance.h"
#include "block.h"
#include "gameWithThread.h"

// 게임 설정
const int PLAYER = 2;
const int BOARD_WIDTH = 100;
const int BOARD_HEIGHT = 30;

const int MAP_WIDTH = 12;
const int MAP_HEIGHT = 23;

const int X_OFFSET = 5;
const int Y_OFFSET = 2;

const double SPEED = 0.6;



//게임 모드 (modesize는 모드의 갯수를 나타냄)
enum MODE {NORMAL , PVP_LOCAL , PVP_SERVER , MODE_SIZE};

class myGameInstance : public GameInstance{

private:

    //singleton 패턴
    myGameInstance();
    static myGameInstance * instance;

    //게임판 
    //0 : 빈칸 , 1 : 현재 블록 , 2 : 그림자 , 3 : 블록 , 4 : 벽 , 100 ~ : 색상 블록
    char *** board;

    // 유저별 게임 진행 객체
    Block ** tetris;

    void gameLoop() override;

    void gotoxy(int x,int y);
    void initTerminal();

    //게임판을 생성하는 함수
    void initBoard();
    //게임판을 제거하는 함수
    void delBoard();

    //테두리 그리는 함수
    void drawBorder();
    //싱글 게임에서 필요한 화면을 그리는 함수
    void drawSingle();
    //pvp 게임에서 필요한 화면을 그리는 함수
    void drawDouble();

    //메뉴 선택
    MODE drawMenu();
    //화면에 게임판 그리는 함수 player에 따라 따로 그림
    void drawBoard(int x_offset , int y_offset , int player);
    //다음 블록을 그리는 함수
    void drawNext(int x_offset , int y_offset , int player , int & prev);
    //싱글 게임
    int singleGame();
    //멀티 게임
    int localGame();
    //서버 멀티 게임
    int serverGame();


public:
    
    ~myGameInstance();

    static myGameInstance * get();
};