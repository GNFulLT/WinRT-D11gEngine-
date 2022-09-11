#include <Windows.h>
#include <string>
#include "Window/MainWindow.h"
#include "Game/GameMain.h"
#include <d3d11.h>
using namespace GNF;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevinstance, _In_ LPSTR lpszCmdline, _In_ int nCmdShow)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	std::string appName = "GNF_ENGINE";

	HANDLE prevMutex = OpenMutexA(NULL, FALSE, appName.c_str());
	if (prevMutex)
		return -1;


	HANDLE mutex = CreateMutexA(NULL, FALSE, appName.c_str());
	//Mutex Error
	if (mutex == NULL)
	{
		return -1;
	}

	Game::GameMain* m = Game::GameMain::BuildGame();

	int res = m->Run();
	
	return res;
}