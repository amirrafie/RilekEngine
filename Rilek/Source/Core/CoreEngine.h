#pragma once
#include <memory>
#include <vector>

#include "Tools/Delegate.h"
#include "Tools/Logger.h"

#define CREATE_SYSTEM(s) CreateSystem<s>(#s)

namespace Core
{
	struct a
	{
		void Init() {}

		void End() {}
	};

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
		void CreateSystem(const char* systemName)
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
			auto& endDelegateRef = m_initDelegates.emplace_back();
			initDelegateRef.Attach<&System::End>(systemPtr);

			// create system data
			auto& data = m_systemDataContainer.emplace_back();
			data.m_name = systemName;
		}


		// To register multiple systems
		template<typename... Systems>
		void RegisterUpdateSystem()
		{
			(RegisterUpdateSystem<Systems>(), ...);
		}
		
		// Register a system to run once per engine loop
		template<typename System>
		void RegisterUpdateSystem()
		{
			SystemID id = GetSystemID<System>();
			assert((id >= m_systemContainer.size()) && "System was not created!");

			auto& systemRef = m_systemContainer[id];
			System* systemPtr = static_cast<System*>(systemRef.get());

			// store update delegate
			auto& m_updateDelegates = m_initDelegates.emplace_back();
			m_updateDelegates.Attach<&System::Update>(systemPtr);
		}


	public:
		void Test()
		{
			CREATE_SYSTEM(a);
		}

		void Init();

	private:

		std::vector<SystemType> m_systemContainer;
		std::vector<SystemData> m_systemDataContainer;

		std::vector<Rilek::Delegate<void()>> m_initDelegates;
		std::vector<Rilek::Delegate<void(float)>> m_updateDelegates;
		std::vector<Rilek::Delegate<void(float)>> m_fixedUpdateDelegates;
		std::vector<Rilek::Delegate<void()>> m_endDelegates;

	};
}