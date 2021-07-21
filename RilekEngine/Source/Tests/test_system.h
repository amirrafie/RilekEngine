#pragma once
#include "ECS/world.h"
#include "Events/event_manager.h"

namespace Rilek
{
	class test_system
	{
		int test = 0;
	public:
		void init(Rilek::ECS::world&);
		void update(Rilek::ECS::world&, float);
		void fixed_update(float);
		void end() {};

		void test_event_trigger(const Events::test_event& e);

		Events::EventDispatcher m_ev_dispatcher;
		Events::EventListener m_ev_listener;
	};
}