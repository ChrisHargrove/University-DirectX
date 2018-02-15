#include "Game.h"

int main() {

	wWinMain(GetModuleHandle(NULL), NULL, NULL, 1);

#if DEBUG_MODE == 1
	system("pause");
#endif

	return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//---------------------------------------------------------------- Game title, fullscreen, vSync
	if (!MainGame::Instance()->Initialize(hInstance, "DirectX Game", false, true)) { 
		MainGame::Instance()->Shutdown(); return 0; 
	}
	
	MainGame::Instance()->Run();
	MainGame::Instance()->Shutdown();

	return 0;
}