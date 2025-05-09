#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <ctime>

#include "myGame.h"

//커서 지우기
void CursorView(char show){
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}


int main(){

    SetConsoleOutputCP(CP_UTF8);  // 콘솔 출력 인코딩을 UTF-8로 설정
    SetConsoleCP(CP_UTF8); 

    CursorView(0);

    myGameInstance * my = new myGameInstance;
    
    my->start();
    return 0;
}