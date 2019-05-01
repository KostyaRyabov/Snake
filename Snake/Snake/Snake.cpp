// Snake.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Snake.h"
#include "Objects.h"
#include <thread>

using namespace std;

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


//-----------------------------------------------
thread *t;

HWND hWnd;
HDC hdc;
Game game;

bool start = 1;
//-----------------------------------------------

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SNAKE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKE));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(0);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_SYSMENU,
	   CW_USEDEFAULT, CW_USEDEFAULT, 512, 552, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//

void Run() {
	short result;

	while ((result = game.snake->move(&hWnd, &hdc)) == 0) {
		Sleep(300 / game.speed);
	};

	if (result == 2) {
		game.lvl = 10;
	}
	else if (result == 3) {
		game.lvl = 11;
	}

	game.Play(&hWnd, &hdc);

	ValidateRect(hWnd, NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CHAR:
	{
		hdc = GetDC(hWnd);

		switch (wParam)
		{
		case 'a':
		case 'A':
		{
			if (game.lvl == 0) {
				if (--game.speed < 1) game.speed = 1;

				game.Play(&hWnd, &hdc);
			}
			else {
				game.snake->rotate(2);
			}
		}
			break;
		case 'w':
		case 'W':
			{
				game.snake->rotate(3);
			}
			break;
		case 'd':
		case 'D':
		{
			if (game.lvl == 0) {
				if (++game.speed > 10) game.speed = 10;

				game.Play(&hWnd, &hdc);
			}
			else {
				game.snake->rotate(0);
			}
		}
			break;
		case 's':
		case 'S':
		{
			if (game.lvl == 1) {
				game.snake->rotate(1);
			}
		}
			break;
		case ' ':
		{
			if (game.lvl == 0) {
				game.lvl = 1;
				game.start = true;

				game.Play(&hWnd, &hdc);

				if (t != NULL) if (t->joinable()) t->join();

				t = new thread(Run);
			}
			else if (game.lvl == 9) {
				game.lvl = 1;

				game.snake->stop = false;

				game.Play(&hWnd, &hdc);

				t = new thread(Run);
			}
			else if (game.lvl == 1) {
				game.lvl = 9;
				game.snake->stop = true;

				game.Play(&hWnd, &hdc);
			}
			else if (game.lvl == 10 || game.lvl == 11) {
				game.lvl = 0;

				game.Play(&hWnd, &hdc);
			}
		}
		}

		//ValidateRect(hWnd, NULL);
	}
	break;
	case WM_PAINT:
	{
		hdc = GetDC(hWnd);

		if (start) {
			game.Play(&hWnd, &hdc);
			start = 0;
		}
	}
        break;
	case WM_DESTROY: {
		PostQuitMessage(0);

		t->~thread();
	}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}