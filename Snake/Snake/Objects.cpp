#include "stdafx.h"
#include "Objects.h"
#include <stdio.h>
#include <ctime>
#include <thread>

short map[64][64];

short i, j;

el *obj;

POINT p, tp;

Food *food;

el::el(POINT position) {
	pos = position;

	brush[0] = CreateSolidBrush(RGB(255, 255, 255));

	for (i = 0; i < 10; i++)
	{
		brush[i+1] = CreateSolidBrush(RGB(i * 20, 100 / (i + 1), i * 10));
	}
}

void el::Shine(bool a) {
	if (a) f = 1;

	if (c >= 9) f = -1;
	else if (c < 2 && !a) {
		f = 0;
		c = 1;
	}

	c = c + f;
}

//------------------------------------------------------------

POINT RandomSpace() {
	for (i = rand() % 63; i < 128; i++) {
		for (j = rand() % 63; j < 128; j++) {
			if (map[i % 63][j % 63] == 0) {
				map[i % 63][j % 63] = 2;
				return { i % 63 + 1, j % 63 + 1 };
			}
		}
	}

	return { 0,0 };
}

RECT BLOCK(POINT p) {
	return { 8 * (p.x - 1) + 1 , 8 * (64 - p.y) + 1, 8 * p.x - 1,  8 * (65 - p.y) - 1 };
}

void Game::Play(HWND *hWnd, HDC *hdc) {
	if (lvl == 0) {
		FillRect(*hdc, &WINDOW, brush);
		sprintf_s(str, "Snake\r\r\r\rspeed = %d\r\r[press SPACE to play]\r\r(use 'a','w','d','s')", speed);
		DrawTextA(*hdc, str, -1, &R, DT_CENTER);

		//ReleaseDC(*hWnd, *hdc);
	}
	else if (lvl == 1) {
		FillRect(*hdc, &WINDOW, brush);

		srand(time(NULL));

		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				map[i][j] = 0;
			}
		}

		if (start) {
			start = false;

			snake = new Snake();
			food = new Food();
		}

		food->Show(hWnd, hdc);
	}
	else if (lvl == 9) {
		FillRect(*hdc, &WINDOW, brush);

		DrawTextA(*hdc, "\r\r\r\r\rpause", -1, &R, DT_CENTER);
	}
	else if (lvl == 10) {
		FillRect(*hdc, &WINDOW, brush);

		sprintf_s(str, "\r\r\r\r\rYou lose the game!\r\rYou get %d points!", (snake->len - 1) * speed);

		DrawTextA(*hdc, str, -1, &R, DT_CENTER);
	}
	else if (lvl == 11) {
		FillRect(*hdc, &WINDOW, brush);

		DrawTextA(*hdc, "\r\r\r\r\rYou won!!!", -1, &R, DT_CENTER);
	}
}

RECT* Game::GetRect() {
	return &R;
}

//-----------------------------------------------------------

void Snake::rotate(short dir) {
	if (direction % 2 != dir % 2)
		if (go) {
			go = false;
			direction = dir;
		}
}

short Snake::move(HWND *hWnd, HDC *hdc) {
	if (stop) return 1;
	
	obj = head;
	
	p = obj->pos;

	go = true;
	
	if (len == 1) FillRect(*hdc, &BLOCK(obj->pos), obj->brush[0]);

	map[obj->pos.x - 1][obj->pos.y - 1] = 0;

	if (direction == 0) map[++obj->pos.x - 1][obj->pos.y - 1];
	else if (direction == 1) map[obj->pos.x - 1][--obj->pos.y - 1];
	else if (direction == 2) map[--obj->pos.x - 1][obj->pos.y - 1];
	else if (direction == 3) map[obj->pos.x - 1][++obj->pos.y - 1];

	if (head->pos.x > 64 || head->pos.x < 1 || head->pos.y > 64 || head->pos.y < 1) return 2;

	if (map[obj->pos.x - 1][obj->pos.y - 1] == 2){
		map[obj->pos.x - 1][obj->pos.y - 1] = 1;

		food->ChangePos();
		food->Show(hWnd, hdc);

		add = true;
	}
	else if (map[obj->pos.x - 1][obj->pos.y - 1] == 1) return 2;

	obj->Shine(add);
	FillRect(*hdc, &BLOCK(obj->pos), obj->brush[obj->c]);
	
	for (i = 1; i < len; i++)
	{
		if (i == len - 1) FillRect(*hdc, &BLOCK(obj->next->pos), obj->next->brush[0]);
		
		obj = obj->next;

		map[obj->pos.x - 1][obj->pos.y - 1] = 0;

		tp = obj->pos;
		obj->pos = p;
		p = tp;

		map[obj->pos.x - 1][obj->pos.y - 1] = 1;

		obj->Shine(add);
		FillRect(*hdc, &BLOCK(obj->pos), obj->brush[obj->c]);
	}

	if (add) {
		if (++len == 4096) return 3;

		obj->next = new el(p);
		obj = obj->next;

		obj->Shine(add);
		FillRect(*hdc, &BLOCK(obj->pos), obj->brush[obj->c]);

		add = false;
	}

	//ReleaseDC(*hWnd, *hdc);

	ValidateRect(*hWnd, NULL);

	return 0;
}

//------------------------------------------------------------

void Food::Show(HWND *hWnd, HDC *hdc) {
	FillRect(*hdc, &BLOCK(pos), brush);

	ValidateRect(*hWnd, NULL);
	//ReleaseDC(*hWnd, *hdc);
}

void Food::ChangePos() {
	pos = RandomSpace();
}

Food::Food() {
	pos = RandomSpace();
}