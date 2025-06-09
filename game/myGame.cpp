#include "myGame.h"
#include <windows.h>


// 콘솔 색상 변경
void setColor(int color , int bgcolor){
    color &= 0xf;
    bgcolor &= 0xf;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                           (bgcolor << 4) | color);
}


//생성자
myGameInstance::myGameInstance(){
    this->tetris = new Block*[8];
}
//소멸자

myGameInstance::~myGameInstance(){
    delete[] this->tetris;
}

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
    
    for(int p = 0; p < PLAYER; p++){

        this->board[p] = new char*[MAP_HEIGHT];

        for(int i = 0;i<MAP_HEIGHT;i++){
            this->board[p][i] = new char[MAP_WIDTH];
            for(int j = 0;j<MAP_WIDTH; j++)
                this->board[p][i][j] = _BOARD::EMPTY;
        }
    }

    
    for(int p = 0; p< PLAYER; p++){
        for(int i = 0;i<MAP_HEIGHT;i++){
            board[p][i][0] = _BOARD::WALL;
            board[p][i][MAP_WIDTH-1] = _BOARD::WALL;
        }

        for(int i = 0;i<MAP_WIDTH;i++){
            board[p][MAP_HEIGHT-1][i] = _BOARD::WALL;
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

//싱글 게임의 화면을 그린다. (테두리 등 변하지 않을 화면)
void myGameInstance::drawSingle(){
    int x_offset = X_OFFSET + 25;
    int y_offset = Y_OFFSET;
    int width = 8 , height = 3;

    //NEXT 및 테두리 그리기
    gotoxy(x_offset + 1, y_offset);
    printf(" NEXT");

    gotoxy(x_offset , y_offset);
    printf("┏");

    for(int i = y_offset + 1;i < y_offset + height; i++){
        gotoxy(x_offset, i);
        printf("┃");
    }
    gotoxy(x_offset, y_offset + height);
    printf("┗");

    for(int i = x_offset + 1;i<x_offset + width; i++){
        gotoxy(i , y_offset + height);
        printf("━");
    }
    gotoxy(x_offset + width , y_offset + height);
    printf("┛");

    for(int i = y_offset + 2 ; i >= y_offset + 1; i--){
        gotoxy(x_offset + width , i);
        printf("┃");
    }
    gotoxy(x_offset + width , y_offset);
    printf("┓");

    //점수판 테두리 그리기

    x_offset = x_offset;
    y_offset = y_offset + 7;
    width = 50 , height = 15;

    gotoxy(x_offset , y_offset);
    printf("┏");

    for(int i = y_offset + 1;i < y_offset + height; i++){
        gotoxy(x_offset , i);
        printf("┃");
    }
    gotoxy(x_offset, y_offset + height);
    printf("┗");

    for(int i = x_offset + 1;i<x_offset + width; i++){
        gotoxy(i , y_offset + height);
        printf("━");
    }
    gotoxy(x_offset + width , y_offset + height);
    printf("┛");

    for(int i = y_offset + height - 1 ; i >= y_offset + 1; i--){
        gotoxy(x_offset + width , i);
        printf("┃");
    }
    gotoxy(x_offset + width , y_offset);
    printf("┓");

    for(int i = x_offset + 1;i<x_offset + width; i++){
        gotoxy(i , y_offset);
        printf("━");
    }

    gotoxy(x_offset + 2 , y_offset + 2);
    printf("SCORE : ");
    gotoxy(x_offset + 2 , y_offset + 4);
    printf("속도 : ");
    gotoxy(x_offset + 2 , y_offset + 5);
    printf("시작 : space");

    gotoxy(x_offset + 2 , y_offset + 7);
    printf("이동 : ← →      회전 : ↑      확정 : space");
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
void myGameInstance::drawBoard(int x_offset , int y_offset , int player ){
    
    //게임판 그리기
    //요청받은 플레이어의 게임판을 그린다.
    for(int i = 0;i<MAP_HEIGHT;i++){

        for(int j = 0;j<MAP_WIDTH;j++){

            gotoxy(x_offset + (j * 2) , y_offset + i);
            _BOARD piece = static_cast<_BOARD>(this->board[player][i][j]);
                if( piece == EMPTY ){
                    setColor(WHITE , 0);
                    printf(" ");
                }else if( piece ==  CUR_BLOCK){
                    setColor(BLOCK_COLOR[tetris[player]->getCurrentBlock()] , 0);
                    printf("■");
                }else if ( piece == SHADOW){
                    setColor(WHITE , 0);
                    printf("▨");
                }else if( piece == PREV_BLOCK){
                    setColor(WHITE , 0);
                    printf("■");
                }else if(piece == WALL){
                    setColor(WHITE , 0);
                    printf("▣");
                }else if(piece < 100){ // 색상 블록일 경우
                    setColor(piece , 0);
                    printf("■");
                }else{
                    printf("♬");
                }
            

        }
    }
}

void myGameInstance::drawNext(int x_offset , int y_offset , int player , int & prev){
    int next = this->tetris[player]->peekQueueBlock();
    if(next == prev) return;

    int sx = x_offset + 2;
    int sy = y_offset + 2;

    // 이전 흔적 지우기
    for(int i = 0;i < 4; i++){
        if(prev == -1) break;
        gotoxy(sx + BLOCK[prev][i][1] , sy + BLOCK[prev][i][0]);
        printf(" ");
    }
    
    for(int i = 0;i < 4; i++){
        gotoxy(sx + BLOCK[next][i][1] , sy + BLOCK[next][i][0]);
        setColor(BLOCK_COLOR[next] , 0);
        printf("■");
    }
    prev = next;
}


// 싱글 게임
int myGameInstance::singleGame(){
    this->tetris[0] = new Block(board[0] , 3);

    clock_t last_time = clock();
    //이전 블록
    int prev = -1;
    //점수
    int score = 0;
    while(1){
        clock_t now = clock();

        // 게임판 렌더링
        this->drawBoard(X_OFFSET , Y_OFFSET , 0);
        
        //게임 설정
        // while(false && _kbhit()){
            
        // }

        double diff = double(now - last_time) / CLOCKS_PER_SEC;
        
        bool is_gen = this->tetris[0]->onBoardBlock();
        
        //다음 블록 그리기
        drawNext(X_OFFSET + 26 , Y_OFFSET , 0 , prev);
        //speed 마다 블록 아래로
        if(diff > SPEED){
            gotoxy(0,0);
            std::cout<<diff;
            this->tetris[0]->moveBlock(DIRECTION::DOWN);
            last_time = now;
        }
        
        // //키 입력 들어왔을 때
        if(_kbhit()){
            int ch = _getch();
            if(ch == 224){
                ch = _getch();
                if(ch == 75) // 블록 이동
                    this->tetris[0]->moveBlock(DIRECTION::LEFT);
                else if(ch == 77)
                    this->tetris[0]->moveBlock(DIRECTION::RIGHT);
                else if(ch == 80)
                    this->tetris[0]->moveBlock(DIRECTION::DOWN);

                else if(ch == 72) //블록 회전
                    this->tetris[0]->rotateBlock();

            }

            if(ch == 32){
                this->tetris[0]->spaceKeyPress();
            }

        }
        
        int s = this->tetris[0]->clearLine();
        if(s == -1 || !is_gen) { //반환된 점수가 -1이거나 블록이 생성되지 않았다면 게임 오버
            system("cls");
            break;
        }

        score += (100 * s);
        //추가 점수
        if(s >= 2) score += (100 * pow(2 , s - 1));
        
        gotoxy(40 , 11);
        setColor(WHITE , 0);
        printf("%d" , score);
        //cpu
        Sleep(30);
    }
    //블록 객체 메모리 해제
    delete this->tetris[0];

    return 0;
}


//로컬 게임, 스레드 활용
int myGameInstance::localGame(){

    int status = -1;
    GameWithThread game(this->tetris , board , 1 , PLAYER , &status);

    //스레드 시작
    game.run();

    clock_t last_time = clock();

    //이전 블록
    int prev[PLAYER] = {-1 , -1};
    
    while(1){
        clock_t now = clock();

        // 게임판 렌더링
        this->drawBoard(X_OFFSET , Y_OFFSET , 0);
        this->drawBoard(X_OFFSET + 50 , Y_OFFSET , 1);
        

        double diff = double(now - last_time) / CLOCKS_PER_SEC;
        game.tick();
        //다음 블록 그리기
        // drawNext(X_OFFSET + 26 , Y_OFFSET , 0 , prev[0]);
        // drawNext(X_OFFSET + 26 , Y_OFFSET , 0 , prev[1]);

        //speed 마다 블록 아래로
        if(diff > SPEED){
            gotoxy(0,0);
            // std::cout<<diff;
            game.moveAll();
            last_time = now;
        }
        
        //키 입력 들어왔을 때
        if(_kbhit()){
            int ch = _getch();
            
            if(ch == 119){
                game.pushKey(UP , 0);
            }else if(ch == 97){
                game.pushKey(LEFT , 0);
            }else if(ch == 115){
                game.pushKey(DOWN , 0);
            }else if(ch == 100){
                game.pushKey(RIGHT , 0);
            }else if(ch == 32){
                game.pushKey(SPACE , 0);
            }else if(ch == 56){
                game.pushKey(UP , 1);
            }else if(ch == 52){
                game.pushKey(LEFT , 1);
            }else if(ch == 53){
                game.pushKey(DOWN , 1);
            }else if(ch == 54){
                game.pushKey(RIGHT , 1);
            }else if(ch == 48){
                game.pushKey(SPACE , 1);
            }

        }
        

        if(status >= 0){ //게임이 종료되었을 때
            //status : 패배한 플레이어
            break;
        }
        
        //cpu
        Sleep(30);
    }
    
    return 0;
}









void myGameInstance::gameLoop()  {
    // this->initTerminal();
    this->initBoard();
    MODE mode = this->drawMenu();
    system("cls");
    
    //변하지 않을 화면을 미리 그려둔다.
    this->drawBorder();

    switch(mode){
        case MODE::NORMAL:
            this->drawSingle();
            this->singleGame();
            break;
        case MODE::PVP_LOCAL:
            this->localGame();
            break;
        
        case MODE::PVP_SERVER:
            break;

        default:
            this->singleGame();
            break;
    }

    
}