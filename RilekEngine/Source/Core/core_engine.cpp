#include "stdafx.h"
#include "core_engine.h"

#include "Tools/Delegate/delegate.h"
#include "Tools/Logger/logger.h"
#include "Tools/Containers/ranges.h"

#include "Glfw/glfw_system.h"
#include "Windows/window_system.h"
#include "Tests/test_system.h"

namespace Rilek::Core
{
	std::unordered_map<std::string, std::function<void(ECS::world&, const std::string&)>> engine::s_component_registration_fns;

	engine* engine::s_instance = nullptr;
	winmain_entry_params engine::s_initial_entry_params;

	engine::engine() :
		m_prev_frame_dt(0),
		m_fixed_frame_dt(0),
		m_accumulated_dt(0)
	{
	}

	void engine::create_systems()
	{
		CREATE_SYSTEM(Rilek::Glfw::glfw_system);
		CREATE_SYSTEM(Rilek::Window::window_system); // dependant on Rilek::Glfw::glfw_system
		CREATE_SYSTEM(Rilek::test_system);

	}

	void engine::register_systems()
	{
		register_update_systems<
			Rilek::Glfw::glfw_system,
			Rilek::Window::window_system,
			test_system
		>();

		register_fixed_update_systems<
			test_system
		>();
	}

	void engine::register_components()
	{
		for (const auto& component_reg_pair : s_component_registration_fns)
		{
			component_reg_pair.second(m_current_world, component_reg_pair.first);
		}
	}

	void engine::init(winmain_entry_params entry_params)
	{
		s_instance = this;
		s_initial_entry_params = entry_params;

#if SHOW_CONSOLE
		// show console terminal
		show_console();
#endif

		// initialise logger
		Rilek::Tools::logger::init();

		RLK_INFO("Initialising Rilek Engine");

		// Create and register systems
		create_systems();
		register_systems();

		// register components
		register_components();

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

		for (auto& delegate : reverse(m_endDelegates))
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
			FILE* cout,* cin;

			freopen_s(&cout, "CONOUT$", "w", stdout);
			if (freopen_s(&cin, "CONIN$", "w+", stdin) != 0)
			{
				assert(false);
			};
			SetConsoleTitleW(L"console");
		}
	}
}

