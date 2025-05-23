#include "myGame.h"
#include <windows.h>


//커서 이동
void myGameInstance::gotoxy(int x, int y){
    if (x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
        COORD pos = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    }
}


void myGameInstance::initTerminal(){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // 콘솔 버퍼 먼저 설정
    COORD bufferSize = { (SHORT)BOARD_WIDTH, (SHORT)BOARD_HEIGHT };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // 콘솔 창 크기 설정
    SMALL_RECT windowSize = { 0, 0, (SHORT)(BOARD_WIDTH - 1), (SHORT)(BOARD_HEIGHT - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

//플레이어 수에 따른 게임 판 초기화
void myGameInstance::initBoard(){
    this->board = new char**[PLAYER];
    
    for(int p = 0; p< PLAYER; p++){

        this->board[p] = new char*[MAP_HEIGHT];

        for(int i = 0;i<MAP_HEIGHT;i++){
            this->board[p][i] = new char[MAP_WIDTH];
            for(int j = 0;j<MAP_WIDTH; j++)
                this->board[p][i][j] = 0;
        }
    }

    
    for(int p = 0; p< PLAYER; p++){
        for(int i = 0;i<MAP_HEIGHT;i++){
            board[p][i][0] = 4;
            board[p][i][MAP_WIDTH-1] = 4;
        }

        for(int i = 0;i<MAP_WIDTH;i++){
            board[p][MAP_HEIGHT-1][i] = 4;
        }
    }
}

// 게임판 메모리 해제
void myGameInstance::delBoard(){
    for(int p = 0; p < PLAYER; p++){

        for(int i = 0;i<MAP_HEIGHT;i++){
            delete[] this->board[p][i]; // char *
        }
        delete[] this->board[p]; // char **
    }

    delete[] this->board; // char ***
}

//콘솔 테두리 그리기
void myGameInstance::drawBorder(){
	gotoxy(0, 0);						//테두리 출력
	printf("┏");
	for (int i = 1; i < BOARD_WIDTH-1; i++){
		gotoxy( i , 0);
		printf("━");
	}

	gotoxy(BOARD_WIDTH-1, 0);
	printf("┓");

	for (int i = 1; i < BOARD_HEIGHT-1; i++){
		gotoxy( BOARD_WIDTH-1, i);
		printf("┃");
	}

	gotoxy(BOARD_WIDTH-1, BOARD_HEIGHT-1);
	printf("┛");

	for (int i = 1; i < BOARD_WIDTH-1; i++){
		gotoxy(i, BOARD_HEIGHT-1);
		printf("━");
	}

	for (int i = 1; i < BOARD_HEIGHT-1; i++){
		gotoxy(0, i);
		printf("┃");
	}

    gotoxy(0, BOARD_HEIGHT-1);
	printf("┗");
}


// 입력된 모드 반환

MODE myGameInstance::drawMenu(){
    int ret = 0;

    this->drawBorder();
    
    gotoxy(15, 7);
	printf("■■■  ■■■  ■■■  ■■■  ■■■  ■■■");
	gotoxy(15, 8);
	printf(" ■   ■     ■   ■  ■  ■   ■    ");
	gotoxy(15, 9);
	printf(" ■   ■■■   ■   ■■    ■   ■■■");
	gotoxy(15, 10);
	printf(" ■   ■     ■   ■  ■  ■     ■");
	gotoxy(15, 11);
	printf(" ■   ■■■   ■   ■  ■ ■■■  ■■■");

    //키입력 감지하면 종료
    while (!_kbhit()){
		gotoxy(30, 20);
		printf("- Press any button -");
		Sleep(400);
		gotoxy(30, 20);
		printf("                    ");
		Sleep(400);
    }
    //입력 버퍼 제거
    _getch();

    gotoxy(23 , 15 + ret);
    printf("> ");

    gotoxy(25 , 15);
    printf("1. SINGLE MODE");
    gotoxy(25 , 16);
    printf("2. LOCAL PVP MODE"); 
    gotoxy(25 , 17);
    printf("3. SERVER PVP MODE");

    //키입력 감지하면 종료
    while (true){
        int ch = _getch();
        //엔터 혹은 스페이스 입력시 반환
        if(ch == 13 || ch == 32){
            return static_cast<MODE>(ret);
        }else{
            gotoxy(23 , 15 + ret);
            printf("  ");

            ch = _getch();

            if(ch == 72){
                ret = (ret - 1 + (int)MODE::MODE_SIZE) % (int)MODE::MODE_SIZE;
            }else if(ch == 80){
                ret = (ret + 1) % (int)MODE::MODE_SIZE;
            }
            gotoxy(23 , 15 + ret);
            printf("> ");
        }

        Sleep(40);
	} 
}



//메인 렌더링
void myGameInstance::drawBoard(int x_offset , int y_offset , int player){
    
    //게임판 그리기
    //요청받은 플레이어의 게임판을 그린다.
    for(int i = 0;i<MAP_HEIGHT;i++){

        for(int j = 0;j<MAP_WIDTH;j++){

            gotoxy(x_offset + (j * 2) , y_offset + i);
            
            switch(this->board[player][i][j]){
                case 0:
                    printf(" ");
                    break;
                case 1:
                    printf("■");
                    break;
                case 2:
                    printf("▨");
                    break;
                case 3:
                    printf("■");
                    break;
                case 4:
                    printf("□");
                    break;
                default:
                    printf("♬");
            }

        }
    }

    //상황판 그리기
}


// 싱글 게임

int myGameInstance::singleGame(){
    Block * b = new Block(board[0] , 3);

    clock_t last_time = clock();



    while(1){
        clock_t now = clock();

        // 게임판 렌더링
        this->drawBoard(X_OFFSET , Y_OFFSET , 0);
        
        //게임 설정
        // while(false && _kbhit()){
            
        // }

        double diff = double(now - last_time) / CLOCKS_PER_SEC;
        
        bool is_gen = b->onBoardBlock();
        
        //speed 마다 블록 아래로
        if(diff > SPEED){
            gotoxy(0,0);
            std::cout<<diff;
            b->moveBlock(DIRECTION::DOWN);
            last_time = now;
        }
        
        // //키 입력 들어왔을 때
        if(_kbhit()){
            int ch = _getch();
            if(ch == 224){
                ch = _getch();
                if(ch == 75) // 블록 이동
                    b->moveBlock(DIRECTION::LEFT);
                else if(ch == 77)
                    b->moveBlock(DIRECTION::RIGHT);
                else if(ch == 80)
                    b->moveBlock(DIRECTION::DOWN);

                else if(ch == 72) //블록 회전
                    b->rotateBlock();

            }

            if(ch == 32){
                b->spaceKeyPress();
            }

        }
        
        int score = b->clearLine();
        if(score == -1 || !is_gen) { //반환된 점수가 -1이거나 블록이 생성되지 않았다면 게임 오버
            system("cls");
            break;
        }
        
        //cpu
        Sleep(30);
    }
    //블록 객체 메모리 해제
    delete b;

    return 0;
}









void myGameInstance::gameLoop()  {
    // this->initTerminal();
    this->initBoard();
    MODE mode = this->drawMenu();
    system("cls");
    
    this->drawBorder();

    switch(mode){
        case MODE::NORMAL:
            this->singleGame();
            break;
        default:
            this->singleGame();
            break;
    }

    
}