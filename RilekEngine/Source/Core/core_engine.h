#pragma once
#include <memory>
#include <vector>
#include <type_traits>

#include "Tools/Delegate/delegate.h"
#include "ECS/world.h"

#define CREATE_SYSTEM(s) create_system<s>(#s)

namespace Rilek::Window
{
	class window_system;
}

namespace Rilek::Core
{
	struct system_data
	{
		const char* m_name;
		float m_prev_dt;
	};

	//wrapper functions
	template<typename System>
	void system_init_wrapper(System* t_system_ptr, Rilek::ECS::world&)
	{
		(*t_system_ptr).System::init();
	}

	class engine
	{
		using system_type = std::unique_ptr<void, void(*)(void*)>;
		using system_ID = unsigned int;

		system_ID generate_system_ID()
		{
			static system_ID systemID = 0;
			return systemID++;
		}

		template<typename System>
		system_ID get_system_ID()
		{
			static system_ID ID = generate_system_ID();
			return ID;
		}


		template<typename System>
		System* get_system()
		{
			return static_cast<System*>(m_systemContainer[get_system_ID<System>()]);
		}

		// Create instance of a system and store the init and end delegate
		template<typename System>
		System* create_system(const char* systemName)
		{
			// get the system id, and check if the id corresponds to the latest index that will be assigned
			system_ID id = get_system_ID<System>();
			assert((id == m_systemContainer.size()) && "Error creating system");

			//  Create the system
			system_type system(new System{}, [](void* ptr) {delete static_cast<System*>(ptr); });
			auto& system_ref = m_systemContainer.emplace_back(std::move(system));
			System* system_ptr = static_cast<System*>(system_ref.get());

			// store init delegate
			if constexpr (std::is_invocable_v<decltype(&System::init), System&, Rilek::ECS::world&>)
			{
				auto& init_delegate_ref = m_initDelegates.emplace_back();
				init_delegate_ref.attach<&System::init>(system_ptr);
			}
			else if constexpr (std::is_invocable_v<decltype(&System::init), System&>)
			{
				auto& init_delegate_ref = m_initDelegates.emplace_back();
				init_delegate_ref.attach<&system_init_wrapper<System>>(system_ptr);
			}


			// store end delegate
			auto& end_delegate_ref = m_endDelegates.emplace_back();
			end_delegate_ref.attach<&System::end>(system_ptr);

			// create system data
			auto& data = m_systemDataContainer.emplace_back();
			data.m_name = systemName;

			return system_ptr;
		}


		// To register multiple systems
		template<typename... Systems>
		void register_update_systems()
		{
			(register_update_system<Systems>(), ...);
		}

		// Register a system to run once per engine loop
		template<typename System>
		void register_update_system()
		{
			system_ID id = get_system_ID<System>();
			assert((id < m_systemContainer.size()) && "System was not created!");

			auto& system_ref = m_systemContainer[id];
			System* system_ptr = static_cast<System*>(system_ref.get());

			// store update delegate
			auto& update_delegates = m_updateDelegates.emplace_back();
			update_delegates.attach<&System::update>(system_ptr);
		}

		// To register multiple fixed update systems
		template<typename... Systems>
		void register_fixed_update_systems()
		{
			(register_fixed_update_system<Systems>(), ...);
		}

		// Register a system to run once per dt
		template<typename System>
		void register_fixed_update_system()
		{
			system_ID id = get_system_ID<System>();
			assert((id < m_systemContainer.size()) && "System was not created!");

			auto& system_ref = m_systemContainer[id];
			System* system_ptr = static_cast<System*>(system_ref.get());

			// store update delegate
			auto& fixed_update_delegates = m_fixedUpdateDelegates.emplace_back();
			fixed_update_delegates.attach<&System::fixed_update>(system_ptr);
		}


	public:
		engine() {}
		engine(const engine&) = delete;
		engine& operator= (const engine&) = delete;
		engine& operator= (engine&&) = delete;

		inline static engine* get_engine() { return m_instance; }
		inline bool engine_is_running() { return m_is_running; };

		void create_systems();
		void register_systems();

		void init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
		void update();
		void end();


		// Stop the engine
		void stop();

		// Show console terminal
		void show_console();

	private:
		static engine* m_instance;


		bool m_is_running = true;

		// Pointers to important systems
		Window::window_system* m_windowsSystem = nullptr;

		std::vector<system_type> m_systemContainer;
		std::vector<system_data> m_systemDataContainer;

		ECS::world m_current_world;

		// for frametime calculation
		std::chrono::high_resolution_clock::time_point m_frame_start_time;
		std::chrono::high_resolution_clock::time_point m_frame_end_time;
		float m_prev_frame_dt;
		float m_fixed_frame_dt;
		float m_accumulated_dt;

		std::vector<Rilek::delegate<void(ECS::world&)>> m_initDelegates;
		std::vector<Rilek::delegate<void(ECS::world&, float)>> m_updateDelegates;
		std::vector<Rilek::delegate<void(ECS::world&, float)>> m_fixedUpdateDelegates;
		std::vector<Rilek::delegate<void(ECS::world&)>> m_endDelegates;

	};
}