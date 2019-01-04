#include "StdAfx.h"
#include "MineField.h"
#include <windows.h>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
using namespace std;

void MineField::SetColor(int text,int background)//���� ������
{
   HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(h,(WORD)((background<<4)|text));
}
MineField::MineField(int width,int height,int bombNumber)
{
	this->width=width;
	this->height=height;
	this->bombNumber=bombNumber;
	curProgress=0;
	field=new Tile*[height];
	for(int i=0;i<height;++i)
		field[i]=new Tile[width]();
	player.posX=player.posY=0;
}
void MineField::StartGame()//������ ����
{
	GenerateBombs();
	DisplayField();
	SetPlayer();
}
void MineField::GenerateBombs()//���������� �������� ����� ���� �� �������� ������ ���-��
{
	int i=0;
	srand(time(NULL));
	while(i<bombNumber)
	{
		int x=rand()%(height),
			y=rand()%(width);
		if(!field[x][y].HasBomb())
		{
			field[x][y].SetBomb(true);
			++i;
		}
	}
}
void MineField::DisplayField()//����� ����
{
	cout<<"\xC9";
	for(int i=0;i<width-1;++i)
		cout<<"\xCD\xD1";
	cout<<"\xCD\xBB"<<endl;
	for(int i=0;i<height;++i)
	{
		cout<<"\xBA";
		for(int j=0;j<width;++j)
		{
			cout<<"\xB0";
			if(j!=width-1)
				cout<<"\xB3";
			else
				cout<<"\xBA";
		}
		if(i!=height-1)
		{
			cout<<endl<<"\xC7";
			for(int i=0;i<width-1;++i)
				cout<<"\xC4\xC5";
			cout<<"\xC4\xB6"<<endl;
		}
		else
		{
			cout<<endl<<"\xC8";
			for(int i=0;i<width-1;++i)
				cout<<"\xCD\xCF";
			cout<<"\xCD\xBC"<<endl;
		}
	}
}
void MineField::SetPlayer()//��������� ����, ������� ��������� ������� ������
{
	ChangePlayerPos();//������������� ��������� ��������� �������
	int key,exit=0;
	while(1)
	{
		key=getch();
		switch(key)
		{
			case 80://down
			{
				DownRight(player.posY,height);
				break;
			}
			case 72://up
			{
				UpLeft(player.posY,height);
				break;
			}
			case 77://right
			{
				DownRight(player.posX,width);
				break;
			}
			case 75://left
			{
				UpLeft(player.posX,width);
				break;
			}
			case 13://enter
			{
				if(field[player.posY][player.posX].GetCurState()==0)//���� ������� ����� - ���������, � ��������� ������ - ������ �� ������
				{
					if(field[player.posY][player.posX].HasBomb())//����� ����
					{
						EndGameFailure();
						exit=1;
					}
					else
						RevealRegion(player.posX,player.posY);//��������� �������
					if(curProgress==width*height-bombNumber)//���� �������� ������ ���-�� ��������� ��� ���� - ���� ��������
					{
						EndGameSuccess();
						exit=1;
					}

				}
				break;
			}
			case 93://"]"
			{
				if(field[player.posY][player.posX].GetCurState()==0)//������� ����� - ��������
				{
					SetColor(11,15);
					field[player.posY][player.posX].SetCurState(1);
					cout<<"\x18";
					SetColor(0,15);
					ChangePlayerPos();
				}
				else if(field[player.posY][player.posX].GetCurState()==1)//������� ������� - ��������
				{
					field[player.posY][player.posX].SetCurState(0);
					cout<<"\xB0";
					ChangePlayerPos();
				}
				break;
			}
			case 27://escape
			{
				exit=1;
				goto a;
			}
		}
		if(exit==1)//���� =1 ������� �� ������������ �����
a:		break;
	}
}
MineField::Tile::Tile(void)
{
	curState=State::Hidden;
	bomb=false;
}
void MineField::Tile::SetBomb(bool has)
{
	bomb=has;
}
void MineField::Tile::SetCurState(int state)
{
	curState=state;
}
int MineField::Tile::GetCurState()
{
	return curState;
}
bool MineField::Tile::HasBomb()
{
	return bomb;
}
void MineField::DownRight(int&playerPos,int edge)//������ ������� ������
{
	if(playerPos!=edge-1)
		++playerPos;
	else
		playerPos=0;
	ChangePlayerPos();
}
void  MineField::UpLeft(int&playerPos,int edge)//������ ������� ������
{
	if(playerPos!=0)
		--playerPos;
	else
		playerPos=edge-1;
	ChangePlayerPos();
}
void MineField::ChangePlayerPos()//���������� ������ �� ������� ������� ������
{
	COORD C;
	C.X=(short)player.posX*2+1;
	C.Y=(short)player.posY*2+1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),C);
}
void MineField::ChangePlayerPos(short x,short y)//���������� ������ �� ������� �,�
{
	COORD C;
	C.X=x;
	C.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),C);
}
int MineField::CheckNeighboringMines(int x,int y)//��������� ���������� ���� ������ ������
{
	int num=0;
	for(int i=-1;i<=1;++i) // ����� ������� �������� �������� �����
		for(int j=-1;j<=1;++j)
			if(OnField(x+i,y+j)&&field[y+j][x+i].HasBomb())
				++num;
	return num;
}
void MineField::RevealRegion(int curx,int cury)
{
	int num=CheckNeighboringMines(curx,cury);//��������� ���� �� ����� �����
	ChangePlayerPos(curx*2+1,cury*2+1);//������ ������
	if(field[cury][curx].GetCurState()!=1)//���� �� �������, ������� �����
	{
		cout<<num;//������� ���-�� �������� ����
		field[cury][curx].SetCurState(2);//������ ��������� �� ��������
		++curProgress;//���������� ��������
	}
	if(num==0)//���� ����� �� ���� ����, ���������� ���������
		for(int i=-1;i<=1;++i)
			for(int j=-1;j<=1;++j)
				if(OnField(curx+j,cury+i)&&field[cury+i][curx+j].GetCurState()!=2)//���� �� ���� � ��������� �� ��������, ��������� ������
					RevealRegion(curx+j,cury+i);
	ChangePlayerPos();//������� ������ ������� ������� �� ��������� ������
}
bool MineField::OnField(int x,int y)//��������� ��������� �� ����� � �������� ����
{
	if(x>=0&&x<width&&y>=0&&y<height)
		return true;
	else
		return false;
}
void MineField::EndGameFailure()//���������� ��������� ����
{
	for(int i=0;i<width;++i)
		for(int j=0;j<height;++j)
			if(field[j][i].HasBomb())//���� ���� �����
			{
				ChangePlayerPos(i*2+1,j*2+1);
				if(field[j][i].GetCurState()==0) // ���� ����������
				{
					SetColor(4,15);
					cout<<"\x06";
				}
				else if(field[j][i].GetCurState()==1) // ���� ��������
				{
					SetColor(15,2);
					cout<<"\x18";
				}
			}
			else if(field[j][i].GetCurState()==1) // ���� ��� �����, �� ��������
			{
				ChangePlayerPos(i*2+1,j*2+1);
				SetColor(15,4);
				cout<<"\x18";
			}
	SetColor(0,15);
	ChangePlayerPos(width-4,height*2+1);
	cout<<"You lose";
	getch();
}
void MineField::EndGameSuccess() // �������� ��������� ����
{
	for(int i=0;i<width;++i)
		for(int j=0;j<height;++j)
			if(field[j][i].HasBomb()) // ���� ���� �����
			{
				ChangePlayerPos(i*2+1,j*2+1);
				if(field[j][i].GetCurState()==0) // ���� ����������
				{
					SetColor(2,15);
					cout<<"\x06";
				}
			}
	SetColor(0,15);
	ChangePlayerPos(width-3,height*2+1);
	cout<<"You win";
	getch();
}
MineField::~MineField()
{
	for(int i=0;i<height;++i)
		delete[]field[i];
	delete[]field;
}