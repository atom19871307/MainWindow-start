#undef UNICODE
#include<Windows.h>
#include"resource.h"

CONST CHAR* g_sz_VALUES[] = { "This","is","my","first","List","Box" };
int g_selectedIndex = -1;  // хранит индекс редактируемого элемента

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProcEdit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPravInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DlgProc, 0);
	return 0;
}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
	HWND hList = GetDlgItem(hwnd, IDC_LIST);
	for (int i = 0; i < sizeof(g_sz_VALUES) / sizeof(g_sz_VALUES[0]); i++)
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)g_sz_VALUES[i]);
	}
			break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_LIST:
			if (HIWORD(wParam) == LBN_DBLCLK)
				DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, DlgProcEdit, 0);
			break;
		case IDC_BUTTON_ADD:
			DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ADD), hwnd, DlgProcAdd, 0);
			break;
			
			// Код для удаления элемента
		case IDC_BUTTON_DELETE:
		{
			HWND hList = GetDlgItem(hwnd, IDC_LIST);
			// Получаем индекс выбранного элемента
			int selectedIndex = SendMessage(hList, LB_GETCURSEL, 0, 0);
			// Если есть выбранный элемент
			if (selectedIndex != LB_ERR)
			{
				// Удаляем элемент по индексу
				SendMessage(hList, LB_DELETESTRING, selectedIndex, 0);
			}
			else
			{
				MessageBox(hwnd, "Выберите элемент для удаления", "Info", MB_OK | MB_ICONINFORMATION);
			}
			break;
		}

		case IDCANCEL:
			EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}

BOOL CALLBACK DlgProcAdd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hwnd, IDC_EDIT_ADD));
		break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{	
		case IDOK:
		{
			CHAR sz_buffer[MAX_PATH] = {};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ADD);
			SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);

			HWND hParent = GetParent(hwnd);
			HWND hList = GetDlgItem(hParent, IDC_LIST);

					// Проверяем, существует ли элемент
			
			if (SendMessage(hList, LB_FINDSTRINGEXACT, 0, (LPARAM)sz_buffer) == LB_ERR)
			{
				// Если элемент не найден, добавляем его в список
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)sz_buffer);
				EndDialog(hwnd, 0);
			
			}
			else
			{
				MessageBox(hwnd, "Такой элемент уже добавлен", "Info", MB_OK | MB_ICONINFORMATION);
			
			}
			break;
		}
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
	}
	break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}

BOOL CALLBACK DlgProcEdit(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)"Изменить элемент");
		HWND hParent = GetParent(hwnd);
		HWND hList = GetDlgItem(hParent, IDC_LIST);
		// Получаем индекс выбранного элемента
		g_selectedIndex = SendMessage(hList, LB_GETCURSEL, 0, 0);


		if (g_selectedIndex != LB_ERR)
		{ 	
		CHAR sz_buffer[MAX_PATH] = {};
		SendMessage(hList, LB_GETTEXT, g_selectedIndex, (LPARAM)sz_buffer);
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ADD);
		SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		SendMessage(hEdit, EM_SETSEL, 0, -1);
		SendMessage(hEdit, EM_SETSEL, -1, -1);
		}
		SetFocus(GetDlgItem(hwnd, IDC_EDIT_ADD));

	}
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CHAR sz_buffer[MAX_PATH] = {};
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_ADD);
			SendMessage(hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)sz_buffer);

			HWND hParent = GetParent(hwnd);
			HWND hList = GetDlgItem(hParent, IDC_LIST);   // Переопределяем hList здесь

			// Обновляем элемент в списке
			SendMessage(hList, LB_DELETESTRING, g_selectedIndex, 0);// Удаляем старый элемент
			SendMessage(hList, LB_INSERTSTRING, g_selectedIndex, (LPARAM)sz_buffer);   // Вставляем новый

			EndDialog(hwnd, 0);
			break;
		}
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
	}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}
