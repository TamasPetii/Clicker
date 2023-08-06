#include "App.h"

int main()
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	App::Init();
	App::Run();
	App::Delete();

	return 0;
}