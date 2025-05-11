#include <stdio.h>
#include <Windows.h>
#include <string.h>

//커서 위치 이동
void gotoxy(int x=0, int y=0)//windows,stdio
{
	COORD coord = { 0, 0 };
	coord.X = x; coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
	bool TipPlay=true;
	int x1,y1;
	char abc[100] = "mode con: cols=100 lines=50";
	system(abc); // 이런식으로 같은 글자가 써있는 문자열 변수를 넣어도 됩니다.
	while(TipPlay)
	{
		gotoxy(0,0);
		printf("x 좌표입력 :    \n");
		printf("y 좌표입력 :    \n");
		gotoxy(13,0);
		scanf("%d",&x1);
		gotoxy(13,1);
		scanf("%d",&y1);

		sprintf(abc, "%s%d%s%d", "mode con: cols=", x1, " lines=",y1); //결과:"mode con: cols=[x1값] lines=[y1값]";
		// 배열 abc 에 여러 문자열 집어넣기. printf는 콘솔창에 띄워주는것이지만 sprintf는 앞의 변수에 입력을해주는 함수입니다.
		printf("%s",abc);
		system(abc);

		gotoxy(x1-2,y1-1);//구석지에 별 생성 이해가 안될경우 이전강의 ㄱㄱ
		printf("★");
	}
	return 0;
}