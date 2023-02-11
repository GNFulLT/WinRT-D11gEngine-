#include "pch.h"
#include "Core/Game.h"
#ifndef _DEBUG

#pragma comment( linker, "/subsystem:windows" )

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevinstance, _In_ LPSTR lpszCmdline, _In_ int nCmdShow)
{
	spdlog::set_level(spdlog::level::critical);

	auto game = GNF::Core::Game::Build();
	game->Init();
	game->Run();
	game->Destroy();
	system("pause");

	return 0;
}

#else if
#pragma comment( linker, "/subsystem:console" )

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	

#ifdef _DEBUG
	spdlog::set_level(spdlog::level::trace);
#endif

	char* pcDynamicHeapStart = new char[17u];
	strcpy_s(pcDynamicHeapStart, 17u, "DynamicHeapStart");
	auto game = GNF::Core::Game::Build();

	try
	{
		game->Init();
		game->Run();
	}
	catch (std::exception ex)
	{
		spdlog::error(std::format("Exception throwed to main msg : \n\n{}",ex.what()).c_str());
	}
	game->Destroy();

	char* pcHeapLeakStart = new char[17u];
	strcpy_s(pcHeapLeakStart, 17u, "Heap Memory leak");

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return 0;
}

#endif