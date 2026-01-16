#define _CRT_SECURE_NO_WARNINGS
#undef UNICODE
#include<Windows.h>
#include<cstdio>
#include<iostream>
#include"resource.h"


#define g_i_BUTTON_SIZE					50
#define g_i_INTERVAL					1
#define g_i_DUBBLE_BUTTON_SIZE			g_i_BUTTON_SIZE*2 + g_i_INTERVAL
#define g_i_START_X						10
#define g_i_START_Y						10
#define g_i_DISPLAY_WIDTH				g_i_BUTTON_SIZE*5 + g_i_INTERVAL*4
#define g_i_DISPLAY_HEIGHT				22
#define g_i_BUTTON_START_X				g_i_START_X
#define g_i_BUTTON_START_Y				g_i_START_Y + g_i_DISPLAY_HEIGHT + g_i_INTERVAL

#define g_i_WINDOW_WIDTH			g_i_DISPLAY_WIDTH + g_i_START_X*2 + 16
#define g_i_WINDOW_HEIGTH			g_i_DISPLAY_HEIGHT + g_i_START_Y*2 + (g_i_BUTTON_SIZE+g_i_INTERVAL)*4 + 38

#define BUTTON_X_POSITION(SHIFT)		g_i_BUTTON_START_X+ (g_i_BUTTON_SIZE + g_i_INTERVAL)* (SHIFT)
#define BUTTON_Y_POSITION(SHIFT)		g_i_BUTTON_START_Y+ (g_i_BUTTON_SIZE + g_i_INTERVAL)* (SHIFT)
 
CONST CHAR g_OPERATIONS[] = "+-*/";

CONST CHAR g_sz_WINDOW_CLASS[] = "Calc PV_522";
LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevinst, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = NULL;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpszMenuName = NULL;
	wClass.lpfnWndProc = (WNDPROC)WndProc;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registratin failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindowEx
	(
		NULL,	//ExStyle
		g_sz_WINDOW_CLASS,	//Window class
		g_sz_WINDOW_CLASS,	//Window title
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME^ WS_MAXIMIZEBOX,//Window style
		CW_USEDEFAULT, CW_USEDEFAULT,//Position
		g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGTH,//Size
		NULL,	//Parent Window
		NULL,	//hMenu
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG  msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)

{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		//AllocConsole();
		freopen("CONOUT$", "w", stdout);
		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			g_i_START_X, g_i_START_Y,
			g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd,
			(HMENU)IDC_DISPLAY, 
			GetModuleHandle(NULL),
			NULL
		);
		/////////////////////////////////////////////////////////////////////////////////
		CHAR sz_digit[2] = {};
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = i + j + '1';
				CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILD |WS_VISIBLE |BS_PUSHBUTTON,
					BUTTON_X_POSITION(j), BUTTON_Y_POSITION(2 - i / 3),
					//g_i_BUTTON_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL)*j,
					//g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL)*(2-i/3),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					HMENU(IDC_BUTTON_1 +i+j),
					GetModuleHandle(NULL),
					NULL
				);
			}
		}
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_X_POSITION(0), BUTTON_Y_POSITION(3),
			g_i_DUBBLE_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_0),
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			BUTTON_X_POSITION(2), BUTTON_Y_POSITION(3),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_POINT),
			GetModuleHandle(NULL),
			NULL
		);
		/////////////////////////////////////////////////////////////////////////////////
		CHAR sz_operation[2] = {};
		for (int i = 0; i < 4; i++)
		{
			sz_operation[0] = g_OPERATIONS[3 - i];
			CreateWindowEx
			(
				NULL, "Button", sz_operation,
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_ICON,	 // Ավելացրինք BS_ICON
				BUTTON_X_POSITION(3), BUTTON_Y_POSITION(i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				HMENU(IDC_BUTTON_SLASH-i),
				GetModuleHandle(NULL),
				NULL
				);
			//////////////////////////////////////////////////
			// Տեղադրում ենք Icon-ները օպերատորների համար
			HICON hIconSlash = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON4));
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconSlash);

			HICON hIconMult = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON7)); // Ստուգեք ID-ն
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconMult);

			HICON hIconMinus = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON6)); // Ստուգեք ID-ն
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconMinus);

			HICON hIconPlus = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON5)); // Ստուգեք ID-ն
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconPlus);
			///////////////////////////////////////////////////
		}
		////////////////////////////////////////////////////////////////////////////////////////
		CreateWindowEx
		(
			NULL, "Button", "",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_ICON,  // Ավելացրինք BS_ICON
			BUTTON_X_POSITION(4), BUTTON_Y_POSITION(0),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_BSP),
			GetModuleHandle(NULL),
			NULL
		);
		/////////////////////////////////////////////////////////////////////////////
		HICON hIconBsp = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconBsp);
		////////////////////////////////////////////////////////////////////////////////

		CreateWindowEx
		(
			NULL, "Button", "",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON |BS_ICON,		// Ավելացրինք BS_ICON
			BUTTON_X_POSITION(4), BUTTON_Y_POSITION(1),
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_CLR),
			GetModuleHandle(NULL),
			NULL
		);
		//////////////////////////////////////////////////////////////////////////////
		HICON hIconClr = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON2));
		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconClr);
		/////////////////////////////////////////////////////////////////////////////////
		CreateWindowEx
		(
			NULL, "Button", "",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_ICON,		// Ավելացրինք BS_ICON
			BUTTON_X_POSITION(4), BUTTON_Y_POSITION(2),
			g_i_BUTTON_SIZE, g_i_DUBBLE_BUTTON_SIZE,
			hwnd,
			HMENU(IDC_BUTTON_EQUAL),
			GetModuleHandle(NULL),
			NULL
		);
		//////////////////////////////////////////////////////////////////////////////
		HICON hIconEqual = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON3));
		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIconEqual);
		/////////////////////////////////////////////////////////////////////////////
	}
		break;
		//////////////////////////////////////////////////////////////////////////
	case WM_COMMAND:
	{
		static DOUBLE	a = DBL_MIN, b = DBL_MIN;	
		static INT		operation = 0;
		static BOOL		input = FALSE;
		static BOOL		input_operation = FALSE;

		CHAR sz_digit[2] = {};
		CHAR sz_display[MAX_PATH] = {};
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_DISPLAY);
		SendMessage(hEditDisplay, WM_GETTEXT, MAX_PATH, (LPARAM)sz_display);
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			input_operation = FALSE;
			if (input == FALSE)ZeroMemory(sz_display, sizeof(sz_display));
			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
			if (sz_display[0] == '0' && sz_display[1] != '.')
				strcpy(sz_display, sz_digit);
			else
				strcat(sz_display, sz_digit);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;
			break;
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			input_operation = FALSE;
			if (strchr(sz_display, '.'))break;	//https://legacy.cplusplus.com/reference/cstring/strchr/
			strcat(sz_display, ".");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			input = TRUE;
			break;
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			sz_display[strlen(sz_display) - 1] = 0;	//NULL-Terminated Lines
			if (sz_display[0] == 0)sz_display[0] = '0';
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			a = DBL_MIN, b = DBL_MIN;
			operation = 0;
			input = FALSE;
			input_operation = FALSE;
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0");
		}
		
		///////////////////////////////////////////////////////////////////////////////
		// Проверяем, была ли нажата одна из кнопок арифметических операций (+, -, *, /)
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			// Если пользователь в данный момент вводит число (есть активный ввод)
			if (input)
			{
				// Если это самое первое число в цепочке вычислений (переменная 'a' еще пуста)
				if (a == DBL_MIN)
				{
					// Преобразуем текст из дисплея в число и сохраняем как первое значение (a)
					a = atof(sz_display);
				}
				else
				{
					// Если в 'a' уже есть значение, значит это следующее число (конвейер)
					// Сохраняем текущее число из дисплея в переменную 'b'
					b = atof(sz_display);
					// Выполняем ПРЕДЫДУЩУЮ операцию, которую пользователь выбрал до этого
					switch (operation)
					{
					case IDC_BUTTON_PLUS:  a += b; break;	// Сложение
					case IDC_BUTTON_MINUS: a -= b; break;	// Вычитание
					case IDC_BUTTON_ASTER: a *= b; break;	// Умножение
					case IDC_BUTTON_SLASH: a /= b; break;	// Деление
					}	
					// Сразу форматируем результат вычисления обратно в строку
					sprintf(sz_display, "%g", a);
					// И отображаем промежуточный результат на экране калькулятора
					SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
				}
				// Сбрасываем флаг ввода, так как число обработано
				input = FALSE;
			}
			// Сохраняем НОВУЮ операцию, которую нажал пользователь, чтобы выполнить её при следующем вводе
			operation = LOWORD(wParam);
			// Указываем системе, что была нажата кнопка операции
			input_operation = TRUE;
		}
		/*//////////////////////////////////////////////////////////////
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			if (input)
			{
				(a == DBL_MIN ? a : b) = atof(sz_display);//https://legacy.cplusplus.com/reference/cstdlib/atof/?kw=atof
				input = false;
			}
			operation = LOWORD(wParam);
			input_operation = TRUE;
		}*/
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			if (input)
			{
				(a == DBL_MIN ? a : b) = atof(sz_display);//https://legacy.cplusplus.com/reference/cstdlib/atof/?kw=atof
				input = FALSE;
			}
			switch (operation)
			{
			case IDC_BUTTON_PLUS:	a += b;		break;
			case IDC_BUTTON_MINUS:	a -= b;		break;
			case IDC_BUTTON_ASTER:	a *= b;		break;
			case IDC_BUTTON_SLASH:	a /= b;		break;
			}
			input_operation = FALSE;
			if (a != DBL_MIN)
			{
				sprintf(sz_display, "%g", a);
				SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			}
		}

		SetFocus(hwnd);
	}
	break;
	////////////////////////////////////////////////////////////////////////
	case WM_KEYDOWN:
	{
		CHAR sz_key[8] = {};
		sprintf(sz_key, "%i", wParam);
		//std::cout << sz_key << std::endl;
		//MessageBox(hwnd, sz_key, "Info", MB_OK);
		if (GetKeyState(VK_SHIFT) < 0 && wParam == '8')
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0);
		}
		else if (GetKeyState(VK_SHIFT) < 0 && wParam == VK_OEM_PLUS)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, TRUE, 0);
		}
		else if (wParam == VK_OEM_PLUS || wParam == VK_RETURN)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, TRUE, 0); break;
		}
		else if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, TRUE, NULL);
		}
		else if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
		{
			SendMessage(GetDlgItem(hwnd, wParam - VK_NUMPAD0 + IDC_BUTTON_0), BM_SETSTATE, TRUE, NULL);
		}
		switch (wParam)
		{
		case VK_ADD:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS),	BM_SETSTATE, TRUE, 0); break;
		case VK_OEM_MINUS:	
		case VK_SUBTRACT:	SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, TRUE, 0); break;
		case VK_MULTIPLY:	SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0); break;
		case VK_OEM_2:		
		case VK_DIVIDE:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, TRUE, 0); break;
		case VK_OEM_PERIOD:
		case VK_DECIMAL:	SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, TRUE, 0); break;\

		case VK_BACK:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP),	BM_SETSTATE, TRUE, 0); break;
		case VK_ESCAPE:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR),	BM_SETSTATE, TRUE, 0); break;
		//case VK_OEM_PLUS:
		//case VK_RETURN:		SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, TRUE, 0); break;
		}
	}
	break;
	case WM_KEYUP:
	{
		//https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
		if (GetKeyState(VK_SHIFT) < 0 && wParam == '8')
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
		}
		else if (GetKeyState(VK_SHIFT) < 0 && wParam == VK_OEM_PLUS)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0);
		}
		else if (wParam == VK_OEM_PLUS || wParam == VK_RETURN)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
		}
		else if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, FALSE, NULL);
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - '0' + IDC_BUTTON_0), 0);
		}
		else if (wParam >= VK_NUMPAD0 && wParam <= VK_NUMPAD9)
		{
			SendMessage(GetDlgItem(hwnd, wParam - VK_NUMPAD0 + IDC_BUTTON_0), BM_SETSTATE, FALSE, NULL);
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - VK_NUMPAD0 + IDC_BUTTON_0), 0);
		}
	LABEL:
		switch (wParam)
		{
			//case VK_OEM_PLUS:
		case VK_ADD:
			//int a = 2;
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, FALSE, 0);	break;
		case VK_OEM_MINUS:
		case VK_SUBTRACT:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_MINUS), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, FALSE, 0); break;
		case VK_MULTIPLY:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0); break;
		case VK_OEM_2:
		case VK_DIVIDE:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_SLASH), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLASH), BM_SETSTATE, FALSE, 0); break;
		case VK_OEM_PERIOD:
		case VK_DECIMAL:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_POINT), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, FALSE, 0); break;

		case VK_BACK:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_BSP), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_BSP), BM_SETSTATE, FALSE, 0); break;
		case VK_ESCAPE:
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLR), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_CLR), BM_SETSTATE, FALSE, 0); break;
		case VK_RETURN: 
			// Клавиша Enter на клавиатуре
			// Выполняем само действие (вычисление)
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);

			// Убираем визуальный эффект нажатия с кнопки "="
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_EQUAL), BM_SETSTATE, FALSE, 0);
			break;
		}
	}
	break;
	case WM_DESTROY:
		FreeConsole();
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
	default:	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}