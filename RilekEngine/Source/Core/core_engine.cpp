#include "stdafx.h"
#include "core_engine.h"


#include "Tools/Delegate/delegate.h"
#include "Tools/Logger/logger.h"

#include "Windows/window_system.h"
#include "Tests/test_system.h"

#include "ECS/Components/test_component.h"

namespace Rilek::Core
{
	engine* engine::m_instance = nullptr;

	engine::engine() :
		m_prev_frame_dt(0),
		m_fixed_frame_dt(0),
		m_accumulated_dt(0)
	{
	}

	void engine::create_systems()
	{
		m_windowsSystem = CREATE_SYSTEM(Rilek::Window::window_system);
		CREATE_SYSTEM(Rilek::test_system);

	}

	void engine::register_systems()
	{
		register_update_systems<
			Rilek::Window::window_system,
			test_system
		>();

		register_fixed_update_systems<
			test_system
		>();
	}

	void engine::register_components()
	{
		REGISTER_COMPONENT(m_current_world, test_component);
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

		// register components
		register_components();


		//start window
		if (!m_windowsSystem)
		{
			RLK_ERROR("Window system not created!");
		}
		else
			m_windowsSystem->init_windows(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

		// delta time stuffs
		m_fixed_frame_dt = 1.f / 60.f;
		m_accumulated_dt = 0;

		for (auto& delegate : m_initDelegates)
			delegate(m_current_world);
	}

	void engine::update()
	{
		m_frame_start_time = std::chrono::high_resolution_clock::now();

		while (m_is_running)
		{
			m_frame_end_time = std::chrono::high_resolution_clock::now();
			auto chrono_frame_duration = m_frame_end_time - m_frame_start_time;
			m_frame_start_time = std::chrono::high_resolution_clock::now();
			m_prev_frame_dt = std::chrono::duration_cast<std::chrono::duration<float>>(chrono_frame_duration).count();

			// If dt is more than fixed time, fixed update would require more steps
			int m_steps = 0;
			m_accumulated_dt += m_prev_frame_dt;
			while (m_accumulated_dt >= m_fixed_frame_dt)
			{
				++m_steps;
				m_accumulated_dt -= m_fixed_frame_dt;
			}

			m_current_world.update();

			// fixed update
			for (int i = 0; i < m_steps; ++i)
			{
				for (auto& delegate : m_fixedUpdateDelegates)
					delegate(m_current_world, m_fixed_frame_dt);
			}

			// update
			for (auto& delegate : m_updateDelegates)
				delegate(m_current_world, m_prev_frame_dt);
		}
	}

	void engine::end()
	{
		RLK_INFO("Shutting down Rilek Engine");

		for (auto& delegate : m_endDelegates)
			delegate(m_current_world);
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
			FILE* cout,* cin,* err;

			freopen_s(&cout, "CONOUT$", "w", stdout);
			if (freopen_s(&cin, "CONIN$", "w+", stdin) != 0)
			{
				assert(false);
			};
			SetConsoleTitleW(L"console");
		}
	}
}

