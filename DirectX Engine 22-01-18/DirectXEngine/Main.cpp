#include "Game.h"

int main() {

	wWinMain(GetModuleHandle(NULL), NULL, NULL, 1);
	system("pause");
	return 0;
}
/*********************************************************
Entry point for Win32 Application
HINSTANCE - Handle(instance) to application
In opt - optional, not used anymore primarily used in 16bit
LPWSTR lpCmdLine - string that contains the command line arguments
int nCmdShow (optional) - determines how window will be shown for the first time
MessageBox(NULL, "Hello World", "Test", NULL); - Displays a popup box
*********************************************************/
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Game MainGame(hInstance, "Hello World", 800, 600);

	if (FAILED(MainGame.Initialize())) { MainGame.Shutdown(); return 0; }

	return MainGame.Run();
}