#pragma once
#include <memory>
#include <vector>

#include "Tools/Delegate/delegate.h"

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
			auto& systemRef = m_systemContainer.emplace_back(std::move(system));
			System* systemPtr = static_cast<System*>(systemRef.get());

			// store init delegate
			auto& initDelegateRef = m_initDelegates.emplace_back();
			initDelegateRef.attach<&System::init>(systemPtr);

			// store end delegate
			auto& endDelegateRef = m_endDelegates.emplace_back();
			endDelegateRef.attach<&System::end>(systemPtr);

			// create system data
			auto& data = m_systemDataContainer.emplace_back();
			data.m_name = systemName;

			return systemPtr;
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

			auto& systemRef = m_systemContainer[id];
			System* systemPtr = static_cast<System*>(systemRef.get());

			// store update delegate
			auto& updateDelegates = m_updateDelegates.emplace_back();
			updateDelegates.attach<&System::update>(systemPtr);
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

			auto& systemRef = m_systemContainer[id];
			System* systemPtr = static_cast<System*>(systemRef.get());

			// store update delegate
			auto& fixedUpdateDelegates = m_fixedUpdateDelegates.emplace_back();
			fixedUpdateDelegates.attach<&System::fixed_update>(systemPtr);
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

		std::vector<Rilek::delegate<void()>> m_initDelegates;
		std::vector<Rilek::delegate<void(float)>> m_updateDelegates;
		std::vector<Rilek::delegate<void(float)>> m_fixedUpdateDelegates;
		std::vector<Rilek::delegate<void()>> m_endDelegates;

	};
}