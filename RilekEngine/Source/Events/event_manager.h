/******************************************************************************/
/*!
\file event_manager.h
\author Amir Rafie
\par email: amirrafiebin.m\@digipen.edu
\par DigiPen login: amirrafiebin.m
*/
/******************************************************************************/
#pragma once
#include <functional>

#include "Tools/Delegate/Delegate.h"
#include "Tools/Containers/sparse_set.h"

namespace Rilek
{
	struct test_event
	{
		int value;
	};

	template <typename EventType>
	class event_manager
	{
		friend class event_listener;
	public:
		static event_manager<EventType>& get_instance()
		{
			static event_manager<EventType> instance;
			return instance;
		}

		template <auto Fn, typename T>
		size_t add_listener(T* instance)
		{
			size_t id = m_event_handlers.size();
			if (m_recycle_list.size())
			{
				id = m_recycle_list.back();
				m_recycle_list.pop_back();
			}

			auto& delegate_ref = m_event_handlers.emplace(id);
			delegate_ref.attach<Fn>(instance);

			return id;
		}

		template <auto Fn>
		size_t add_listener()
		{
			size_t id = m_event_handlers.size();
			if (m_recycle_list.size())
			{
				id = m_recycle_list.back();
				m_recycle_list.pop_back();
			}

			auto& delegate_ref = m_event_handlers.emplace(id);
			delegate_ref.attach<Fn>();

			return id;
		}


		void remove_listener(size_t t_id)
		{
			m_event_handlers.erase(t_id);
			m_recycle_list.push_back(t_id);
		}

		void handle_event(const EventType& ev)
		{
			for (auto& eventHandler : m_event_handlers)
			{
				eventHandler(ev);
			}

			
		}

	private:
		event_manager<EventType>() {}
		sparse_set<Rilek::delegate<void(const EventType&)>> m_event_handlers;
		std::vector<size_t> m_recycle_list;
	};


	class event_dispatcher
	{
	public:
		template <typename EventType>
		void dispatch_event(const EventType& ev)
		{
			event_manager<EventType>::get_instance().handle_event(ev);
		}
	};


	// Cannot unsubscribe from events atm
	class event_listener
	{
		typedef std::function<void()> remove_listener_fn;

	public:
		~event_listener();

		template <typename EventType, auto Fn>
		size_t subscribe()
		{
			size_t remove_function_id = m_remove_functions.size();
			if (m_remove_function_recycle_list.size())
			{
				remove_function_id = m_remove_function_recycle_list.back();
				m_remove_function_recycle_list.pop_back();
			}

			size_t unsub_id = event_manager<EventType>::get_instance().add_listener<Fn>();

			m_remove_functions.insert(remove_function_id, [remove_function_id]() {event_manager<EventType>::get_instance().remove_listener(remove_function_id); });

			return unsub_id;
		}

		template <typename EventType, auto Fn, typename InstanceType>
		size_t subscribe(InstanceType* instance)
		{
			size_t remove_function_id = m_remove_functions.size();
			if (m_remove_function_recycle_list.size())
			{
				remove_function_id = m_remove_function_recycle_list.back();
				m_remove_function_recycle_list.pop_back();
			}

			size_t unsub_id = event_manager<EventType>::get_instance().add_listener<Fn>(instance);

			m_remove_functions.insert(remove_function_id, [remove_function_id]() {event_manager<EventType>::get_instance().remove_listener(remove_function_id); });

			return unsub_id;
		}

		template <typename EventType>
		void unsubscribe(size_t t_id)
		{
			m_remove_functions.at(t_id)();
			m_remove_functions.erase(t_id);
			m_remove_function_recycle_list.push_back(t_id);
		}

	private:
		sparse_set<remove_listener_fn> m_remove_functions;
		std::vector<size_t> m_remove_function_recycle_list;
	};
}