#pragma once

#include "Tools/Delegate/Delegate.h"

namespace Rilek::Events
{
	using EventID = unsigned int;

	EventID GenerateEventID()
	{
		static EventID systemID = 0;
		return systemID++;
	}

	template<typename System>
	EventID GetEventID()
	{
		static EventID ID = GenerateSystemID();
		return ID;
	}

	template <typename EventType>
	class EventManager
	{
	public:
		static EventManager<EventType>& GetInstance()
		{
			static EventManager<EventType> instance;
			return instance;
		}

		template <auto Fn, typename T>
		void AddListener(T* instance)
		{
			auto& delegateRef = m_eventHandlers.emplace_back();
			delegateRef.Attach<Fn>(instance);
		}

		template <auto Fn>
		void AddListener()
		{
			auto& delegateRef = m_eventHandlers.emplace_back();
			delegateRef.Attach<Fn>();
		}

		void HandleEvent(EventType& ev)
		{
			for (auto& eventHandler : m_eventHandlers)
			{
				eventHandler(ev);
			}
		}

	private:
		EventManager<EventType>(){}
		std::vector<Rilek::Delegate<void(EventType&)>> m_eventHandlers;
	};


	class EventDispatcher
	{
		template <typename EventType>
		void DispatchEvent(EventType& ev)
		{
			EventManager<EventType>::GetInstance().HandleEvent(ev);
		}
	};


	// Cannot unsubscribe from events atm
	class EventListener
	{
		template <typename EventType, auto Fn>
		void Subscribe()
		{
			EventManager<EventType>::GetInstance().HandleEvent(ev);
		}

		template <typename EventType, auto Fn, typename InstanceType>
		void Subscribe(InstanceType* instance)
		{

		}
	};
}