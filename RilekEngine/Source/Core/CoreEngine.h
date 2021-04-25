#pragma once
#include <memory>
#include <vector>

#include "Tools/Delegate.h"

#define CREATE_SYSTEM(s) CreateSystem<s>(#s)

namespace Rilek::Window
{
	class WindowSystem;
}

namespace Rilek::Core
{
	struct SystemData
	{
		const char* m_name;
		float m_prev_dt;
	};

	class Engine
	{
		using SystemType = std::unique_ptr<void, void(*)(void*)>;
		using SystemID = unsigned int;



		SystemID GenerateSystemID()
		{
			static SystemID systemID = 0;
			return systemID++;
		}

		template<typename System>
		SystemID GetSystemID()
		{
			static SystemID ID = GenerateSystemID();
			return ID;
		}


		// Create instance of a system and store the init and end delegate
		template<typename System>
		System* CreateSystem(const char* systemName)
		{
			// get the system id, and check if the id corresponds to the latest index that will be assigned
			SystemID id = GetSystemID<System>();
			assert((id == m_systemContainer.size()) && "Error creating system");

			//  Create the system
			SystemType system(new System{}, [](void* ptr) {delete static_cast<System*>(ptr); });
			auto& systemRef = m_systemContainer.emplace_back(std::move(system));
			System* systemPtr = static_cast<System*>(systemRef.get());

			// store init delegate
			auto& initDelegateRef = m_initDelegates.emplace_back();
			initDelegateRef.Attach<&System::Init>(systemPtr);

			// store end delegate
			auto& endDelegateRef = m_endDelegates.emplace_back();
			endDelegateRef.Attach<&System::End>(systemPtr);

			// create system data
			auto& data = m_systemDataContainer.emplace_back();
			data.m_name = systemName;

			return systemPtr;
		}


		// To register multiple systems
		template<typename... Systems>
		void RegisterUpdateSystems()
		{
			(RegisterUpdateSystem<Systems>(), ...);
		}

		// Register a system to run once per engine loop
		template<typename System>
		void RegisterUpdateSystem()
		{
			SystemID id = GetSystemID<System>();
			assert((id < m_systemContainer.size()) && "System was not created!");

			auto& systemRef = m_systemContainer[id];
			System* systemPtr = static_cast<System*>(systemRef.get());

			// store update delegate
			auto& updateDelegates = m_updateDelegates.emplace_back();
			updateDelegates.Attach<&System::Update>(systemPtr);
		}

		// To register multiple fixed update systems
		template<typename... Systems>
		void RegisterFixedUpdateSystems()
		{
			(RegisterFixedUpdateSystem<Systems>(), ...);
		}

		// Register a system to run once per dt
		template<typename System>
		void RegisterFixedUpdateSystem()
		{
			SystemID id = GetSystemID<System>();
			assert((id < m_systemContainer.size()) && "System was not created!");

			auto& systemRef = m_systemContainer[id];
			System* systemPtr = static_cast<System*>(systemRef.get());

			// store update delegate
			auto& fixedUpdateDelegates = m_fixedUpdateDelegates.emplace_back();
			fixedUpdateDelegates.Attach<&System::FixedUpdate>(systemPtr);
		}


	public:
		Engine() {}
		Engine(const Engine&) = delete;
		Engine& operator= (const Engine&) = delete;
		Engine& operator= (Engine&&) = delete;

		inline static Engine* GetEngine() { return mInstance; }
		inline bool EngineIsRunning() { return isRunning; };

		void CreateSystems();
		void RegisterSystems();

		void Init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
		void Update();
		void End();


		// Stop the engine
		void Stop();

		// Show console terminal
		void ShowConsole();

	private:
		static Engine* mInstance;


		bool isRunning = true;

		// Pointers to important systems
		Window::WindowSystem* m_windowsSystem = nullptr;

		std::vector<SystemType> m_systemContainer;
		std::vector<SystemData> m_systemDataContainer;

		std::vector<Rilek::Delegate<void()>> m_initDelegates;
		std::vector<Rilek::Delegate<void(float)>> m_updateDelegates;
		std::vector<Rilek::Delegate<void(float)>> m_fixedUpdateDelegates;
		std::vector<Rilek::Delegate<void()>> m_endDelegates;

	};
}