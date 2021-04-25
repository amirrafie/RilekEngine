#pragma once
#include <vector>

#include "Tools/Delegate.h"

namespace Rilek::Events
{
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
	};

	class EventListener
	{
		template <typename EventType, auto Fn>
		void Subscribe()
		{

		}

		template <typename EventType, auto Fn>
		void Subscribe()
		{

		}
	};
}