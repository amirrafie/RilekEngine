#include "stdafx.h"
#include "core_engine.h"


#include "Tools/Delegate/delegate.h"
#include "Tools/Logger/logger.h"

#include "Windows/window_system.h"

namespace Rilek::Core
{
	engine* engine::m_instance = nullptr;

	void engine::create_systems()
	{
		m_windowsSystem = CREATE_SYSTEM(Rilek::Window::window_system);

	}

	void engine::register_systems()
	{
		register_update_systems<
			Rilek::Window::window_system
		>();

		register_fixed_update_systems<
		>();
	}

	void engine::init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
	{
		m_instance = this;

		// show console terminal
		show_console();

		// initialise logger
		Rilek::Tools::logger::init();

		RLK_INFO("Initialising Rilek Engine");

		// Create and register systems
		create_systems();
		register_systems();

		//start window
		if (!m_windowsSystem)
		{
			RLK_ERROR("Window system not created!");
		}
		else
			m_windowsSystem->init_windows(hInstance, hPrevInstance, lpCmdLine, nCmdShow);




		for (auto& delegate : m_initDelegates)
			delegate();
	}

	void engine::update()
	{
		while (m_is_running)
		{
			for (auto& delegate : m_updateDelegates)
				delegate(0.f);
		}
	}

	void engine::end()
	{
		RLK_INFO("Shutting down Rilek Engine");

		for (auto& delegate : m_endDelegates)
			delegate();
	}

	// Stop the engine
	void engine::stop()
	{
		m_is_running = false;
	}

	// Show console terminal
	void engine::show_console()
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

