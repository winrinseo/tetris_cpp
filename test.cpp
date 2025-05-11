#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <iostream>

//커서 위치 이동
void gotoxy(int x=0, int y=0)//windows,stdio
{
	COORD coord = { 0, 0 };
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
	
	char ** board = new char*[10];

    for(int i = 0;i<10;i++)
        board[i] = new char[20];

    
    board[0][0] = {0};
    for(int i = 0;i<20;i++)
        std::cout<<board[0][i];
    while(1){};
	return 0;
}