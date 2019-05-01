#pragma once

class el
{
public:
	short f = 0;
	short c = 1;
	el(POINT position);
	HBRUSH brush[11];
	
	POINT pos;
	el *next;

	void Shine(bool a);
};

class Snake
{
private:
	short direction;		//	0 - right, 1 - down, 2 - left, 3 - up.

	bool go = true;
	bool add = false;

	el *head = new el({ 32, 32 });
public:				//	init().
	bool stop = false;
	short len = 1;				//	512/8 = 64 blocks are the area of game field. (512x512 pixels) => 64 is max len.

	short move(HWND *hWnd, HDC *hdc);			//	changing position and checking front side (walls or food).
	void rotate(short dir);
};

class Food
{
private:
	POINT pos;

	HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
public:
	void Show(HWND *hWnd, HDC *hdc);
	Food();
	void ChangePos();
};

class Game
{
private:
	RECT R = {0,150,512,512};
	RECT WINDOW = { 0,0,512,512 };

	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));

	char str[75];
public:
	bool start = true;
	
	Snake *snake;

	short speed = 1;		// 1..10.
	short lvl = 0;		// 0 - init, 1 - game, 2 - pause.

	RECT* GetRect();
	void Play(HWND *hWnd, HDC *hdc);			//load lvl.
};