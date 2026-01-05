#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<stdio.h>

CONST CHAR g_sz_CLASS_NAME[] = "Main Windoe PV_522";

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));
	wClass.style = NULL;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = g_sz_CLASS_NAME;
	wClass.lpfnWndProc = WndProc;
	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	INT screen_width = GetSystemMetrics(SM_CXSCREEN);
	INT screen_height = GetSystemMetrics(SM_CYSCREEN);
	INT window_width = screen_width * 3 / 4;
	INT window_height = screen_height * 3 / 4;
	INT window_start_x = screen_width / 8;
	INT window_start_y = screen_height / 8;

	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_CLASS_NAME,
		g_sz_CLASS_NAME,
		WS_OVERLAPPEDWINDOW,
		window_start_x, window_start_y,
		window_width, window_height,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hStatic = CreateWindowEx
		(
			NULL,
			"Static",
			"Этот текст создан функцией CreateWindowEx()",
			WS_CHILD | WS_VISIBLE,
			10, 10,
			500, 25,
			hwnd,
			(HMENU)1000,
			GetModuleHandle(NULL),
			NULL
		);
		HWND hEdit = CreateWindowEx
		(
			NULL,
			"Edit",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 40,
			500, 22,
			hwnd,
			(HMENU)1001,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL,
			"Button",
			"Применить",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			410, 70,
			100, 25,
			hwnd,
			(HMENU)1002,
			GetModuleHandle(NULL),
			NULL
		);

	}
		break;
	case WM_SIZE:

	case WM_MOVE:
	{
		RECT rect;
		GetWindowRect(hwnd,&rect);
		CHAR sz_title[MAX_PATH] = {};
		sprintf(
			sz_title, "%s - Position: %ix%i, Size: %ix%", 
			g_sz_CLASS_NAME, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top
		);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
	}
	case WM_COMMAND:
	
		switch (LOWORD(wParam))
		{
		case 1002:
		{
			CHAR  sz_buffer[MAX_PATH] = {};
			HWND hStatic = GetDlgItem(hwnd, 1000);
			HWND hEdit = GetDlgItem(hwnd, 1001);
			SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);
			SendMessage(hStatic, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;
		}
			break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}