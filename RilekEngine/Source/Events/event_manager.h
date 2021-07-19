#pragma once

#include "Tools/Delegate/Delegate.h"

/// WIP

namespace Rilek::Events
{
	//using event_ID = size_t;

	//event_ID generate_event_ID()
	//{
	//	static event_ID systemID = 0;
	//	return systemID++;
	//}

	//template<typename System>
	//event_ID get_event_ID()
	//{
	//	static event_ID ID = GenerateSystemID();
	//	return ID;
	//}

	template <typename EventType>
	class event_manager
	{
		friend class EventListener;
	public:
		static event_manager<EventType>& get_instance()
		{
			static event_manager<EventType> instance;
			return instance;
		}

		template <auto Fn, typename T>
		void add_listener(T* instance)
		{
			auto& delegate_ref = m_event_handlers.emplace_back();
			delegate_ref.attach<Fn>(instance);
		}

		template <auto Fn>
		void add_listener()
		{
			auto& delegate_ref = m_event_handlers.emplace_back();
			delegate_ref.attach<Fn>();
		}

		void handle_event(EventType& ev)
		{
			for (auto& eventHandler : m_event_handlers)
			{
				eventHandler(ev);
			}
		}

	private:
		event_manager<EventType>(){}
		std::vector<Rilek::delegate<void(EventType&)>> m_event_handlers;
	};


	class EventDispatcher
	{
		template <typename EventType>
		void dispatch_event(EventType& ev)
		{
			event_manager<EventType>::get_instance().handle_event(ev);
		}
	};


	// Cannot unsubscribe from events atm
	class EventListener
	{
	public:
		template <typename EventType, auto Fn>
		void subscribe()
		{
			event_manager<EventType>::add_listener<Fn>();
		}

		template <typename EventType, auto Fn, typename InstanceType>
		void subscribe(InstanceType* instance)
		{
			event_manager<EventType>::add_listener<Fn>(instance);
		}
	};
}