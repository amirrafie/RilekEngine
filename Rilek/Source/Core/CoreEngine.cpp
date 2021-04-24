#include "stdafx.h"
#include "CoreEngine.h"


#include "Tools/Delegate.h"
#include "Tools/Logger.h"

namespace Rilek::Core
{
	Engine* Engine::mInstance = nullptr;

	void Engine::Init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
	{
		mInstance = this;

		// show console terminal
		ShowConsole();

		// initialise logger
		Rilek::Tools::Logger::Init();

		RLK_INFO("Initialising Rilek Engine");

		//start window
		m_windowsSystem.Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	}

	void Engine::Update()
	{
		while (isRunning)
		{
			m_windowsSystem.Update();
		}
	}

	void Engine::End()
	{
		RLK_INFO("Shutting down Rilek Engine");
		m_windowsSystem.End();
	}

	// Stop the engine
	void Engine::Stop()
	{
		isRunning = false;
	}

	// Show console terminal
	void Engine::ShowConsole()
	{    
		if (AllocConsole())
		{
			FILE* file;

			freopen_s(&file, "CONOUT$", "wt", stdout);
			freopen_s(&file, "CONOUT$", "wt", stderr);
			freopen_s(&file, "CONOUT$", "wt", stdin);
			SetConsoleTitleW(L"console");
		}
	}
}

