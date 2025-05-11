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

void myGameInstance::initBoard(){
    this->board = new char*[MAP_HEIGHT];
    

    for(int i = 0;i<MAP_HEIGHT;i++){
        this->board[i] = new char[MAP_WIDTH];
        // *board[i] = {0};
        for(int j = 0;j<MAP_WIDTH; j++)
            this->board[i][j] = 0;
    }

    for(int i = 0;i<MAP_HEIGHT;i++){
        board[i][0] = 4;
        board[i][MAP_WIDTH-1] = 4;
    }

    for(int i = 0;i<MAP_WIDTH;i++){
        board[MAP_HEIGHT-1][i] = 4;
    }
}

//콘솔 테두리 그리기
void myGameInstance::drawBorder(){
	gotoxy(0, 0);						//테두리 출력
	printf("┏");
	for (int i = 1; i < BOARD_WIDTH-2; i++){
		gotoxy( i, 0);
		printf("━");
	}

	gotoxy(BOARD_WIDTH-2, 0);
	printf("┓");

	for (int i = 1; i < BOARD_HEIGHT-2; i++){
		gotoxy( BOARD_WIDTH-2, i);
		printf("┃");
	}

	gotoxy(BOARD_WIDTH-2, BOARD_HEIGHT-2);
	printf("┛");

	for (int i = 1; i < BOARD_WIDTH-2; i++){
		gotoxy(i, BOARD_HEIGHT-2);
		printf("━");
	}

	for (int i = 1; i < BOARD_HEIGHT-2; i++){
		gotoxy(0, i);
		printf("┃");
	}

    gotoxy(0, BOARD_HEIGHT-2);
	printf("┗");
}



void myGameInstance::drawMenu(){
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
    while (!_kbhit())
	{
		gotoxy(30, 20);
		printf("- Press any button -");
		Sleep(400);
		gotoxy(30, 20);
		printf("                    ");
		Sleep(400);
	}
	_getch();
}



//메인 렌더링
void myGameInstance::drawBoard(){
    
    //게임판 그리기
    for(int i = 0;i<MAP_HEIGHT;i++){

        for(int j = 0;j<MAP_WIDTH;j++){

            gotoxy(X_OFFSET + j , Y_OFFSET + i);
            
            switch(this->board[i][j]){
                case 0:
                    printf(" ");
                    break;
                case 1:
                    printf("■");
                    break;
                case 2:
                    printf("■");
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









void myGameInstance::gameLoop()  {
    // this->initTerminal();
    this->initBoard();
    this->drawMenu();
    system("cls");
    
    this->drawBorder();

    Block * b = new Block(board , 3 , 1);

    clock_t last_time = clock();



    while(1){
        clock_t now = clock();

        // 게임판 렌더링
        this->drawBoard();
        
        //게임 설정
        // while(false && _kbhit()){
            
        // }

        double diff = double(now - last_time) / CLOCKS_PER_SEC;
        
        b->onBoardBlock();
        
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
            gotoxy(0 , 0);
            // printf("click!! % d" , ch);
            b->rotateBlock();
        }


        
        Sleep(30);
    }
}