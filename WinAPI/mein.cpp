#include<Windows.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPravInst, LPSTR lpCmdLine, INT nCmdShow)
{
	MessageBox
	(
		NULL, 
		"Hello Windows!", 
		"MessageBox", 
		MB_YESNO | MB_ICONINFORMATION | MB_HELP |MB_DEFBUTTON3 | MB_SYSTEMMODAL
	);
	return 0;
}