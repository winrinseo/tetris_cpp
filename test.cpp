#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

const int BOARD_WIDTH = 100;
const int BOARD_HEIGHT = 30;

const int MAP_WIDTH = 12;
const int MAP_HEIGHT = 23;

const int X_OFFSET = 5;
const int Y_OFFSET = 3;

const double SPEED = 0.6;

void CursorView(char show){
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

//커서 위치 이동
void gotoxy(int x=0, int y=0)//windows,stdio
{
	COORD coord = { 0, 0 };
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void drawBorder(){
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

void initTerminal(){
    char command[100];
    sprintf(command, "%s%d%s%d", "mode con: cols=", BOARD_WIDTH, " lines=",BOARD_HEIGHT);
    system(command);
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);  // 콘솔 출력 인코딩을 UTF-8로 설정
    SetConsoleCP(CP_UTF8); 
	CursorView(0);
    initTerminal();
    drawBorder();

    while(1){};
	return 0;
}