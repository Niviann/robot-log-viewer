#include "framework.h"
#include "Projekt 3.h"
#include <Commdlg.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <objidl.h>
#include <gdiplus.h>
#include <math.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;
using namespace std;

#define MAX_LOADSTRING 100
#define IDC_TEXT 101

HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static HWND START;
static HWND STOP;
static HWND CLEAR;
static HWND LOAD;
static HWND TIME_MINUS;
static HWND TIME_PLUS;
static HWND PLUS1;
static HWND PLUS2;
static HWND PLUS3;
static HWND MINUS1;
static HWND MINUS2;
static HWND MINUS3;

HPEN hPen;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: W tym miejscu umieść kod.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Inicjuj ciągi globalne
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PROJEKT3, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Wykonaj inicjowanie aplikacji:
	if (!InitInstance(hInstance, nCmdShow))
		return FALSE;


	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJEKT3));

	MSG msg;

	// Główna pętla komunikatów:
	while (GetMessage(&msg, nullptr, 0, 0))
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	return (int)msg.wParam;
}

//
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJEKT3));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROJEKT3);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1750, 800, nullptr, nullptr, hInstance, nullptr);



	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

wstring openFile(HWND hWnd)
{
	OPENFILENAME ofn;
	TCHAR file_name[200] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = file_name;
	ofn.nMaxFile = sizeof(file_name);
	ofn.lpstrFile[0] = '\0';
	ofn.lpstrFilter = L"Logs\0*.log\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&ofn);
	return file_name;
}

class filePath
{
private:
	wstring path;
public:
	void filepath(HWND hWnd)
	{
		path = openFile(hWnd);
		InvalidateRect(hWnd, NULL, TRUE);
	}
	wstring getFilePath()
	{
		return path;
	}
};
filePath file;

class sim
{
private:
	bool running = false;
	int position = 1;
	int error = 0;
	double last_roll;
	double last_pitch;
	double last_yaw;
	double arrow1;
public:
	void start() { running = true; }
	void stop() { running = false; }
	bool is_running() { return running; }
	void setPos(int pos) { position = pos; }
	int getPos() { return position; }
	void setErr(int errorNum) { error = 0; error = errorNum; }
	int getErr() { return error; }
	void setLastRoll(double value) { last_roll = value; }
	double getLastRoll() { return last_roll; }
	void setLastPitch(double value) { last_pitch = value; }
	double getLastPitch() { return last_pitch; }
	void setLastYaw(double value) { last_yaw = value; }
	double getLastYaw() { return last_yaw; }
};
sim simulation;

class scaleClass
{
private:
	double scale_roll = 1;
	double scale_pitch = 1;
	double scale_yaw = 1;
	double scale_time = 1;
public:
	double getRoll() { return scale_roll; }
	double getPitch() { return scale_pitch; }
	double getYaw() { return scale_yaw; }
	double getTime() { return scale_time; }
	void setRoll(double set, HWND hWnd)
	{
		scale_roll = set;
		InvalidateRect(hWnd, NULL, TRUE);
	}
	void setPitch(double set, HWND hWnd)
	{
		scale_pitch = set;
		InvalidateRect(hWnd, NULL, TRUE);
	}
	void setYaw(double set, HWND hWnd)
	{
		scale_yaw = set;
		InvalidateRect(hWnd, NULL, TRUE);
	}
	void setTime(double set, HWND hWnd)
	{
		scale_time = set;
		InvalidateRect(hWnd, NULL, TRUE);
	}
};
scaleClass scale;

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		PLUS1 = CreateWindow(TEXT("Button"), TEXT("+"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			90, 50, 50, 50, hWnd, NULL, NULL, NULL);
		MINUS1 = CreateWindow(TEXT("Button"), TEXT("-"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			90, 150, 50, 50, hWnd, NULL, NULL, NULL);

		PLUS2 = CreateWindow(TEXT("Button"), TEXT("+"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			90, 265, 50, 50, hWnd, NULL, NULL, NULL);
		MINUS2 = CreateWindow(TEXT("Button"), TEXT("-"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			90, 360, 50, 50, hWnd, NULL, NULL, NULL);

		PLUS3 = CreateWindow(TEXT("Button"), TEXT("+"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			90, 470, 50, 50, hWnd, NULL, NULL, NULL);
		MINUS3 = CreateWindow(TEXT("Button"), TEXT("-"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			90, 570, 50, 50, hWnd, NULL, NULL, NULL);

		TIME_PLUS = CreateWindow(TEXT("Button"), TEXT("+"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			675, 665, 50, 50, hWnd, NULL, NULL, NULL);
		TIME_MINUS = CreateWindow(TEXT("Button"), TEXT("-"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			575, 665, 50, 50, hWnd, NULL, NULL, NULL);

		START = CreateWindow(TEXT("Button"), TEXT("START"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			1350, 545, 50, 50, hWnd, NULL, NULL, NULL);
		//STOP = CreateWindow(TEXT("Button"), TEXT("STOP"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			//1350, 545, 50, 50, hWnd, NULL, NULL, NULL);
		CLEAR = CreateWindow(TEXT("Button"), TEXT("CLEAR"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			1425, 545, 50, 50, hWnd, NULL, NULL, NULL);
		LOAD = CreateWindow(TEXT("Button"), TEXT("LOAD"), WS_VISIBLE | WS_CHILD | BS_FLAT,
			1500, 545, 50, 50, hWnd, NULL, NULL, NULL);

		HWND hText = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER, 1575, 570, 100, 20, hWnd, (HMENU)IDC_TEXT, NULL, NULL);
	}
	break;

	case WM_COMMAND:
	{
		//obsługa przycisków
		if ((HWND)lParam == START)
		{
			simulation.start();
			InvalidateRect(hWnd, NULL, TRUE);
		}
			

		//if ((HWND)lParam == STOP)
			//simulation.stop();

		if ((HWND)lParam == LOAD)
			file.filepath(hWnd);

		if ((HWND)lParam == CLEAR)
			InvalidateRect(hWnd, NULL, TRUE);

		if ((HWND)lParam == TIME_PLUS)
			scale.setTime(scale.getTime() / 0.75, hWnd);

		if ((HWND)lParam == TIME_MINUS && (25 * scale.getTime()) > 2)
			scale.setTime(scale.getTime() * 0.75, hWnd);

		if ((HWND)lParam == PLUS1)
			scale.setRoll(scale.getRoll() / 0.75, hWnd);

		if ((HWND)lParam == MINUS1 && (50 * scale.getRoll()) > 2)
			scale.setRoll(scale.getRoll() * 0.75, hWnd);

		if ((HWND)lParam == PLUS2)
			scale.setPitch(scale.getPitch() / 0.75, hWnd);

		if ((HWND)lParam == MINUS2 && (50 * scale.getPitch()) > 2)
			scale.setPitch(scale.getPitch() * 0.75, hWnd);

		if ((HWND)lParam == PLUS3)
			scale.setYaw(scale.getYaw() / 0.75, hWnd);

		if ((HWND)lParam == MINUS3 && (50 * scale.getYaw()) > 2)
			scale.setYaw(scale.getYaw() * 0.75, hWnd);



		int wmId = LOWORD(wParam);
		// Analizuj zaznaczenia menu:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_PAINT:
	{

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		Graphics graphics(hdc);
		graphics.Clear(Color(255, 255, 255, 255));

		int errorNumber = 0;
		int len = GetWindowTextLength(GetDlgItem(hWnd, IDC_TEXT)) + 1;
		char* text = new char[len];
		GetWindowTextA(GetDlgItem(hWnd, IDC_TEXT), &text[0], len);
		sscanf_s(text, "%d", &errorNumber);
		simulation.setErr(errorNumber);

		Pen black2px(Color(255, 0, 0, 0), 2);
		Pen black1px(Color(255, 0, 0, 0), 1);
		Pen red(Color(255, 255, 0, 0), 3);
		Pen green(Color(255, 0, 200, 0), 2);
		Pen blue(Color(255, 0, 200, 200), 3);
		Pen yellow(Color(225, 200, 200, 0), 3);
		Pen pink(Color(225, 200, 0, 200), 3);
		Pen white(Color(255, 255, 255, 255), 3);

		TextOut(hdc, 25, 125, L"ROLL:", 5);
		TextOut(hdc, 25, 335, L"PITCH:", 6);
		TextOut(hdc, 25, 545, L"YAW:", 4);

		TextOut(hdc, 1430, 25, L"NORTH", 5);
		TextOut(hdc, 1430, 375, L"SOUTH", 5);
		TextOut(hdc, 1255, 200, L"WEST", 4);
		TextOut(hdc, 1615, 200, L"EAST", 4);

		TextOut(hdc, 1160, 125, L"10°/1s", 6);
		TextOut(hdc, 1160, 335, L"20°/1s", 6);
		TextOut(hdc, 1160, 545, L"50°/1s", 6);

		TextOut(hdc, 1575, 545, L"ERROR NUM:", 10);

		graphics.DrawRectangle(&black2px, 150, 25, 1000, 200);
		graphics.DrawRectangle(&black2px, 150, 235, 1000, 200);
		graphics.DrawRectangle(&black2px, 150, 445, 1000, 200);
		graphics.DrawEllipse(&black2px, 1300, 50, 300, 300);

		// rysowanie kratki
		double x_pos = 150 + (40 * scale.getTime());
		double y_pos_roll_up = 125 - (50 * scale.getRoll());
		double y_pos_pitch_up = 335 - (50 * scale.getPitch());
		double y_pos_yaw_up = 545 - (50 * scale.getYaw());
		double y_pos_roll_down = 125 + (50 * scale.getRoll());
		double y_pos_pitch_down = 335 + (50 * scale.getPitch());
		double y_pos_yaw_down = 545 + (50 * scale.getYaw());

		graphics.DrawLine(&green, 150, 125, 1150, 125);
		graphics.DrawLine(&green, 150, 335, 1150, 335);
		graphics.DrawLine(&green, 150, 545, 1150, 545);

		while (x_pos < 1150)
		{
			graphics.DrawLine(&black1px, x_pos, 25, x_pos, 225);
			graphics.DrawLine(&black1px, x_pos, 235, x_pos, 435);
			graphics.DrawLine(&black1px, x_pos, 445, x_pos, 645);
			x_pos = x_pos + (40 * scale.getTime());
		}

		while (y_pos_roll_up > 25)
		{
			graphics.DrawLine(&black1px, 150, y_pos_roll_up, 1150, y_pos_roll_up);
			y_pos_roll_up = y_pos_roll_up - (50 * scale.getRoll());
		}

		while (y_pos_pitch_up > 235)
		{
			graphics.DrawLine(&black1px, 150, y_pos_pitch_up, 1150, y_pos_pitch_up);
			y_pos_pitch_up = y_pos_pitch_up - (50 * scale.getPitch());
		}

		while (y_pos_yaw_up > 445)
		{
			graphics.DrawLine(&black1px, 150, y_pos_yaw_up, 1150, y_pos_yaw_up);
			y_pos_yaw_up = y_pos_yaw_up - (50 * scale.getYaw());
		}

		while (y_pos_roll_down < 225)
		{
			graphics.DrawLine(&black1px, 150, y_pos_roll_down, 1150, y_pos_roll_down);
			y_pos_roll_down = y_pos_roll_down + (50 * scale.getRoll());
		}

		while (y_pos_pitch_down < 435)
		{
			graphics.DrawLine(&black1px, 150, y_pos_pitch_down, 1150, y_pos_pitch_down);
			y_pos_pitch_down = y_pos_pitch_down + (50 * scale.getPitch());
		}

		while (y_pos_yaw_down < 645)
		{
			graphics.DrawLine(&black1px, 150, y_pos_yaw_down, 1150, y_pos_yaw_down);
			y_pos_yaw_down = y_pos_yaw_down + (50 * scale.getYaw());
		}

		//rysowanie grafu
		ifstream log;
		log.open(file.getFilePath());
		if (log.is_open())
		{
			int numLines = 0;
			string unused;

			for (int i = 0; i < simulation.getErr() + simulation.getPos(); i++)
				getline(log, unused);
			while (getline(log, unused))
				numLines = numLines + 1;
			numLines = numLines - simulation.getPos();
			int stop_num = (int)((1000 / (40 * scale.getTime())) * 25);

			if (numLines < stop_num)
				stop_num = numLines;
			log.close();

			log.open(file.getFilePath());

			for (int i = 0; i < simulation.getErr() + simulation.getPos(); i++)
				getline(log, unused);

			double pos_x = 150;
			double pos_x_older = 150;
			double pos_y_roll = 125;
			double pos_y_pitch = 335;
			double pos_y_yaw = 545;
			double rotation = 0;

			for (int i = 0; i < stop_num; i++)
			{
				string line;
				getline(log, line);
				std::istringstream iss(line);
				double roll;
				double pitch;
				double yaw;

				if (!(iss >> roll >> pitch >> yaw)) { break; }

				double pos_x_old = pos_x;
				pos_x = pos_x + 40 * scale.getTime() / 25;

				double pos_y_roll_old = pos_y_roll;
				pos_y_roll = 125 + 50 * scale.getRoll() / 10 * roll; //10 stopni

				double pos_y_pitch_old = pos_y_pitch;
				pos_y_pitch = 335 + 50 * scale.getPitch() / 20 * pitch; //20stopni

				double pos_y_yaw_old = pos_y_yaw;
				pos_y_yaw = 545 + 50 * scale.getYaw() / 50 * yaw; //50 stopni

				if (pos_y_roll <= 225 && pos_y_roll >= 25 && pos_y_roll_old <= 225 && pos_y_roll_old >= 25)
					graphics.DrawLine(&blue, (int)pos_x_old, (int)pos_y_roll_old, (int)pos_x, (int)pos_y_roll);

				if (pos_y_pitch <= 435 && pos_y_pitch >= 235 && pos_y_pitch_old <= 435 && pos_y_pitch_old >= 235)
					graphics.DrawLine(&yellow, (int)pos_x_old, (int)pos_y_pitch_old, (int)pos_x, (int)pos_y_pitch);

				if (pos_y_yaw <= 645 && pos_y_yaw >= 445 && pos_y_yaw_old <= 645 && pos_y_yaw_old >= 445)
					graphics.DrawLine(&pink, (int)pos_x_old, (int)pos_y_yaw_old, (int)pos_x, (int)pos_y_yaw);
			}
			log.close();

			if (simulation.is_running())
			{

				log.open(file.getFilePath());

				for (int i = 0; i < simulation.getErr() + simulation.getPos(); i++)
					getline(log, unused);

				pos_x = 150;
				pos_x_older = 150;
				pos_y_roll = 125;
				pos_y_pitch = 335;
				pos_y_yaw = 545;
				
				double arrow1_x = 1450;
				double arrow1_y = 200;
				double arrow2_x = 1450;
				double arrow2_y = 200;
				double arrow3_x = 1450;
				double arrow3_y = 200;

				for (int i = 0; i < stop_num; i++)
				{
					string line;
					getline(log, line);
					std::istringstream iss(line);
					double roll;
					double pitch;
					double yaw;
					graphics.DrawLine(&white, 1450, 200, arrow1_x, arrow1_y);
					graphics.DrawLine(&white, (int)arrow1_x, (int)arrow1_y, arrow2_x, arrow2_y);
					graphics.DrawLine(&white, (int)arrow1_x, (int)arrow1_y, arrow3_x, arrow3_y);
					if (!(iss >> roll >> pitch >> yaw)) { break; }

					double pos_x_old = pos_x;
					pos_x = pos_x + 40 * scale.getTime() / 25;

					double pos_y_roll_old = pos_y_roll;
					pos_y_roll = 125 +  50 * scale.getRoll() / 10 * roll;

					double pos_y_pitch_old = pos_y_pitch;
					pos_y_pitch = 335 + 50 * scale.getPitch() / 20 * pitch;

					double pos_y_yaw_old = pos_y_yaw;
					pos_y_yaw = 545 + 50 * scale.getYaw() / 50 * yaw;


					if (pos_y_roll <= 225 && pos_y_roll >= 25 && pos_y_roll_old <= 225 && pos_y_roll_old >= 25)
						graphics.DrawLine(&red, (int)pos_x_old, (int)pos_y_roll_old, (int)pos_x, (int)pos_y_roll);

					if (pos_y_pitch <= 435 && pos_y_pitch >= 235 && pos_y_pitch_old <= 435 && pos_y_pitch_old >= 235)
						graphics.DrawLine(&red, (int)pos_x_old, (int)pos_y_pitch_old, (int)pos_x, (int)pos_y_pitch);

					if (pos_y_yaw <= 645 && pos_y_yaw >= 445 && pos_y_yaw_old <= 645 && pos_y_yaw_old >= 445)
						graphics.DrawLine(&red, (int)pos_x_old, (int)pos_y_yaw_old, (int)pos_x, (int)pos_y_yaw);

					

					arrow1_x =1450 + sin(yaw * 0.0174533) * 125 * -1;
					arrow1_y =200 - cos(yaw * 0.0174533) * 125;

					arrow2_x = 1450 + sin((yaw + 10) * 0.0174533) * 100 * -1;
					arrow2_y = 200 - cos((yaw + 10) * 0.0174533) * 100;

					arrow3_x = 1450 + sin((yaw - 10) * 0.0174533) * 100 * -1;
					arrow3_y = 200 - cos((yaw - 10) * 0.0174533) * 100;

					graphics.DrawLine(&red, 1450, 200, arrow1_x, arrow1_y);
					graphics.DrawLine(&red, (int)arrow1_x, (int)arrow1_y, arrow2_x, arrow2_y);
					graphics.DrawLine(&red, (int)arrow1_x, (int)arrow1_y, arrow3_x, arrow3_y);
					simulation.setPos(simulation.getPos() +1 );

					Sleep(40);
				}

				log.close();

				if (stop_num < numLines)
				{
					
					EndPaint(hWnd, &ps);
					InvalidateRect(hWnd, NULL, TRUE);

				}
				else
				{
					simulation.stop();
					simulation.setPos(0);
				}
			}

		}
		EndPaint(hWnd, &ps);	
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Procedura obsługi komunikatów dla okna informacji o programie.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
