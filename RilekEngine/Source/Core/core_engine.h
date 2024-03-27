#pragma once
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "Tools/Delegate/delegate.h"
#include "ECS/world.h"

#define CREATE_SYSTEM(s) create_system<s>(#s)
#define REGISTER_COMPONENT(s) Rilek::Core::engine_component_registrator<s> component_registrator_##s(#s);

#define SHOW_CONSOLE 1

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

	struct winmain_entry_params
	{
		HINSTANCE m_hInstance;
		HINSTANCE m_hPrevInstance;
		LPWSTR    m_lpCmdLine;
		int       m_nCmdShow;
	};

	//wrapper functions for delegates
	template<typename System>
	void system_init_wrapper(System* t_system_ptr, Rilek::ECS::world&) { (*t_system_ptr).System::init(); }

	template<typename System>
	void system_end_wrapper(System* t_system_ptr, Rilek::ECS::world&) { (*t_system_ptr).System::end(); }

	template<typename System, std::enable_if_t<std::is_invocable_v<decltype(&System::update), System&>, bool> = true>
	void system_update_wrapper(System* t_system_ptr, Rilek::ECS::world&, float) { (*t_system_ptr).System::update(); }
	template<typename System, std::enable_if_t<std::is_invocable_v<decltype(&System::update), System&, Rilek::ECS::world&>, bool> = true>
	void system_update_wrapper(System* t_system_ptr, Rilek::ECS::world& t_world, float) { (*t_system_ptr).System::update(t_world); }
	template<typename System, std::enable_if_t<std::is_invocable_v<decltype(&System::update), System&, float>, bool> = true>
	void system_update_wrapper(System* t_system_ptr, Rilek::ECS::world&, float m_prev_frame_dt) { (*t_system_ptr).System::update(m_prev_frame_dt); }
	template<typename System, std::enable_if_t<std::is_invocable_v<decltype(&System::update), System&, float, Rilek::ECS::world&>, bool> = true>
	void system_update_wrapper(System* t_system_ptr, Rilek::ECS::world& t_world, float m_fixed_dt) { (*t_system_ptr).System::update(m_prev_frame_dt, t_world); }

	template<typename System, std::enable_if_t<std::is_invocable_v<decltype(&System::fixed_update), System&>, bool> = true>
	void system_fixed_update_wrapper(System* t_system_ptr, Rilek::ECS::world&, float) { (*t_system_ptr).System::fixed_update(); }
	template<typename System, std::enable_if_t<std::is_invocable_v<decltype(&System::fixed_update), System&, Rilek::ECS::world&>, bool> = true>
	void system_fixed_update_wrapper(System* t_system_ptr, Rilek::ECS::world& t_world, float) { (*t_system_ptr).System::fixed_update(t_world); }
	template<typename System, std::enable_if_t<std::is_invocable_v<decltype(&System::fixed_update), System&, float>, bool> = true>
	void system_fixed_update_wrapper(System* t_system_ptr, Rilek::ECS::world&, float m_fixed_dt) { (*t_system_ptr).System::fixed_update(m_fixed_dt); }
	template<typename System, std::enable_if_t<std::is_invocable_v<decltype(&System::fixed_update), System&, float, Rilek::ECS::world&>, bool> = true>
	void system_fixed_update_wrapper(System* t_system_ptr, Rilek::ECS::world& t_world, float m_fixed_dt) { (*t_system_ptr).System::fixed_update(m_fixed_dt, t_world); }


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
			system_ID id = get_system_ID<System>();
			RLK_ASSERT((id < m_systemContainer.size()),"System was not created!");
			return static_cast<System*>(m_systemContainer[id]);
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
			// void init(Rilek::ECS::world&)
			if constexpr (std::is_invocable_v<decltype(&System::init), System&, Rilek::ECS::world&>)
			{
				auto& init_delegate_ref = m_initDelegates.emplace_back();
				init_delegate_ref.attach<&System::init>(system_ptr);
			}
			// void init()
			else if constexpr (std::is_invocable_v<decltype(&System::init), System&>)
			{
				auto& init_delegate_ref = m_initDelegates.emplace_back();
				init_delegate_ref.attach<&system_init_wrapper<System>>(system_ptr);
			}
			else
			{
				static_assert(
					std::is_invocable_v<decltype(&System::init), System&, Rilek::ECS::world&> || 
					std::is_invocable_v<decltype(&System::init), System&>, 
					"System has no valid init function!");
			}

			// store end delegate
			// void end(Rilek::ECS::world&)
			if constexpr (std::is_invocable_v<decltype(&System::end), System&, Rilek::ECS::world&>)
			{
				auto& end_delegate_ref = m_endDelegates.emplace_back();
				end_delegate_ref.attach<&System::end>(system_ptr);
			}
			// void end()
			else if constexpr (std::is_invocable_v<decltype(&System::end), System&>)
			{
				auto& end_delegate_ref = m_endDelegates.emplace_back();
				end_delegate_ref.attach<&system_end_wrapper<System>>(system_ptr);
			}
			else
			{
				static_assert(
					std::is_invocable_v<decltype(&System::end), System&, Rilek::ECS::world&> ||
					std::is_invocable_v<decltype(&System::end), System&>, 
					"System has no valid end function!");
			}

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
			// void update(Rilek::ECS::world&, float)
			if constexpr (std::is_invocable_v<decltype(&System::update), System&, Rilek::ECS::world&, float>)
			{
				auto& update_delegate_ref = m_updateDelegates.emplace_back();
				update_delegate_ref.attach<&System::update>(system_ptr);
			}
			else if constexpr (
				std::is_invocable_v<decltype(&System::update), System&> ||
				std::is_invocable_v<decltype(&System::update), System&, Rilek::ECS::world&> ||
				std::is_invocable_v<decltype(&System::update), System&, float> ||
				std::is_invocable_v<decltype(&System::update), System&, float, Rilek::ECS::world&>
				)
			{
				auto& update_delegate_ref = m_updateDelegates.emplace_back();
				update_delegate_ref.attach<&system_update_wrapper<System>>(system_ptr);
			}
			else
			{
				static_assert(
					std::is_invocable_v<decltype(&System::update), System&, Rilek::ECS::world&, float> ||
					std::is_invocable_v<decltype(&System::update), System&> ||
					std::is_invocable_v<decltype(&System::update), System&, Rilek::ECS::world&> ||
					std::is_invocable_v<decltype(&System::update), System&, float> ||
					std::is_invocable_v<decltype(&System::update), System&, float, Rilek::ECS::world&>,
					"System has no valid update function!");
			}
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

			// store fixed_update delegate
			// void fixed_update(Rilek::ECS::world&, float)
			if constexpr (std::is_invocable_v<decltype(&System::fixed_update), System&, Rilek::ECS::world&, float>)
			{
				auto& fixed_update_delegates = m_fixedUpdateDelegates.emplace_back();
				fixed_update_delegates.attach<&System::fixed_update>(system_ptr);
			}
			else if constexpr (
				std::is_invocable_v<decltype(&System::fixed_update), System&> ||
				std::is_invocable_v<decltype(&System::fixed_update), System&, Rilek::ECS::world&> ||
				std::is_invocable_v<decltype(&System::fixed_update), System&, float> ||
				std::is_invocable_v<decltype(&System::fixed_update), System&, float, Rilek::ECS::world&>
				)
			{
				auto& fixed_update_delegates = m_fixedUpdateDelegates.emplace_back();
				fixed_update_delegates.attach<&system_fixed_update_wrapper<System>>(system_ptr);
			}
			else
			{
				static_assert(
					std::is_invocable_v<decltype(&System::fixed_update), System&, Rilek::ECS::world&, float> ||
					std::is_invocable_v<decltype(&System::fixed_update), System&> ||
					std::is_invocable_v<decltype(&System::fixed_update), System&, Rilek::ECS::world&> ||
					std::is_invocable_v<decltype(&System::fixed_update), System&, float> ||
					std::is_invocable_v<decltype(&System::fixed_update), System&, float, Rilek::ECS::world&>,
					"System has no valid fixed_update function!");
			}

		}


	public:
		engine();
		engine(const engine&) = delete;
		engine& operator= (const engine&) = delete;
		engine& operator= (engine&&) = delete;

		inline static engine* get_engine() { return s_instance; }
		inline static const winmain_entry_params& get_windows_entry_params() { return s_initial_entry_params; }
		inline bool engine_is_running() { return m_is_running; };

		void create_systems();
		void register_systems();

		void register_components();

		void init(winmain_entry_params entry_params);
		void update();
		void end();


		// Stop the engine
		void stop();

		// Show console terminal
		void show_console();

		// Global component registration
		static std::unordered_map<std::string, std::function<void(ECS::world &, const std::string &)>> s_component_registration_fns;
	private:
		static engine* s_instance;

		// initial wWinMain entry params
		static winmain_entry_params s_initial_entry_params;

		bool m_is_running = true;

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

	template<typename Component>
	class engine_component_registrator
	{
	public:
		engine_component_registrator(const char *component_name)
		{
			engine::s_component_registration_fns[component_name] = [](ECS::world& world, const std::string& component_name)
				{
					world.register_component<Component>(component_name.c_str());
				};
		}
	};
}