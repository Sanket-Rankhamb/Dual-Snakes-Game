#include"SystemFunctions.h"
#include<windows.h>

void setCursorPosition(int x, int y){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = {x, y};
	SetConsoleCursorPosition(hConsole, pos);
}

void setTextColor(int color){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole,BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE );
	SetConsoleTextAttribute(hConsole,color);
}

void sleep( int milliseconds ){
	Sleep(milliseconds);
}
