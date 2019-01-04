//x0A - bomb
//x18 - flag
//xB0 - unrevealed tile
#include "stdafx.h"
#include "MineField.h"
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <string>
#include <windows.h>
using namespace std;

void Pic()
{
	cout<<"\t\t\xC9\xd1"<<setw(25)<<setfill('\xCD')<<"\xd1\xBB"<<endl
		<<"\t\t\xC7\xc5"<<setw(25)<<setfill('\xc4')<<"\xc5\xb6"<<endl
		<<"\t\t\xba\xb3"<<"      Minesweeper      "<<"\xb3\xba"<<endl
		<<"\t\t\xC7\xc5"<<setw(25)<<setfill('\xc4')<<"\xc5\xb6"<<endl
		<<"\t\t\xC8\xcf"<<setw(25)<<setfill('\xCD')<<"\xcf\xbc"<<endl;
}
void CursorVisibility()
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO c;
	c.bVisible=1;
	c.dwSize=100;
	SetConsoleCursorInfo(h,&c);
}
void SetWindow(int w,int h) 
{
	HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD crd = {w,h};
    SMALL_RECT src = {0, 0,w-1,h-1};
    SetConsoleWindowInfo (out_handle, true, &src);
    SetConsoleScreenBufferSize (out_handle, crd);
}
int main()
{
	CursorVisibility();//размер курсора
	system("color F0");//цвет экрана
	SetWindow(62,50);//устанавливаем размер экрана, который будет игру с самым большим размером поля
	char key;
	int w=9,h=9,m=10;//standart settings
	do{
		system("cls");
		Pic();
		cout<<" 1. Play\n 2. Settings\n 3. Rules\n 4. Exit\n";
		cout<<" Your choice: ";
		string str;
		getline(cin,str);//проверка на введение всего одного символа
		if(str.length()==1)//если был введен только один символ - присваем его
			key=str[0];
		else//в противном случае присваиваем заведомо неправельное значение
			key='g';
		switch(key)
		{
			case '1'://New game
			{
				system("cls");
				MineField game(w,h,m);
				game.StartGame();
				break;
			}
			case '2'://Settings
			{
				char key2;
				do{
					int exit=0;
					system("cls");
					cout<<"Choose level:\n1. Beginner (9x9 - 10 mines)\n2. Intermediate (16x16 - 40 mines)\n3. Expert (16x30 - 99 mines)\n4. Custom\n5. Back to menu\n";
					cout<<"Your choice: ";
					string str2;
					getline(cin,str2);//слегка костыльная проверка на введение всего одного символа
					if(str2.length()==1)//если был введен только один символ - присваем его
						key2=str2[0];
					else//в противном случае присваеваем заведомо неправильное значение
						key2='g';
					switch(key2)
					{
						case '1':
						{
							w=9,h=9,m=10;
							break;
						}
						case '2':
						{
							w=16,h=16,m=40;
							break;
						}
						case '3':
						{
							w=30,h=16,m=99;
							break;
						}
						case '4':
						{
							do{
								cout<<"Height (9-24): ";
								cin>>h;
							}while(h<9||h>24);
							do{
								cout<<"Width (9-30): ";
								cin>>w;
							}while(w<9||w>30);
							do{
								cout<<"Mines (10-"<<h*w/2<<"): ";
								cin>>m;
							}while(m<10||m>h*w/2);
							break;
						}
						case '5':
						{
							exit=1;
							break;
						}
					}
				}while(key2<'1'||key2>'5');
				break;
			}
			case '3':
			{
				system("cls");
				cout<<"\t\tObjective and basic concepts\n\n"
					<<"The objective in Minesweeper is to find and mark all the mines hidden under the squares. Each square will have one of the following:\n"
					<<"\tA mine, and if you open it you'll lose the game.\n\t"
					<<"A number, which tells you how many of its adjacent squares have mines in them. If the number is zero that means that none of the adjacent squares have mines, and they will be automatically opened as well.\n\n\t\t\t"
					<<"Gameplay\n\n"
					<<"Press arrow keys to move on the board.\n"
					<<"Press enter to open a square.\n"
					<<"Press ']' to mark/unmark a square.\n"
					<<"Press exit to get back to main menu.\n\n\t\t\t"
					<<"Winning\n\n"
					<<"You've won the game when you've opened all squares that don't contain a mine. If you've opened all the empty squares but haven't flagged the mine squares remaining, they will be auto-flagged and you have won. So, essentially flagging is not required, it's only there to help you keep track of where you think the mines are. The real way to win is open all the non-mine squares.";
				getch();
				break;
			}
			case '4':
			{
				return 0;
			}
			default:{break;}
		}
	}while(1);
}