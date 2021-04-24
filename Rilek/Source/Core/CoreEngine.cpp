#include "stdafx.h"
#include "CoreEngine.h"


#include "Tools/Delegate.h"
#include "Tools/Logger.h"

#include "Windows/WindowSystem.h"

namespace Rilek::Core
{
	Engine* Engine::mInstance = nullptr;

	void Engine::CreateSystems()
	{
		m_windowsSystem = CREATE_SYSTEM(Rilek::Window::WindowSystem);
	}

	void Engine::RegisterSystems()
	{
		RegisterUpdateSystems<
			Rilek::Window::WindowSystem
		>();
	}

	void Engine::Init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
	{
		mInstance = this;

		// show console terminal
		ShowConsole();

		// initialise logger
		Rilek::Tools::Logger::Init();

		RLK_INFO("Initialising Rilek Engine");

		// Create and register systems
		CreateSystems();
		RegisterSystems();

		//start window
		if (!m_windowsSystem)
		{
			RLK_ERROR("Window system not created!");
		}
		else
			m_windowsSystem->InitWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow);




		for (auto& delegate : m_initDelegates)
			delegate();
	}

	void Engine::Update()
	{
		while (isRunning)
		{
			for (auto& delegate : m_updateDelegates)
				delegate(0.f);
		}
	}

	void Engine::End()
	{
		RLK_INFO("Shutting down Rilek Engine");

		for (auto& delegate : m_endDelegates)
			delegate();
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

