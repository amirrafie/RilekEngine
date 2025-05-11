#pragma once
#include "ECS/world.h"
#include "Events/event_manager.h"

namespace Rilek
{
	class test_system
	{
		int test = 0;


		void ComponentTest(Rilek::ECS::world& world, float dt);
		void VectorTest(Rilek::ECS::world& world);
	public:
		void init(Rilek::ECS::world&);
		void update(Rilek::ECS::world&, float);
		void fixed_update(float);
		void end() {};

		void test_event_trigger(const Rilek::test_event& e);

		Rilek::event_dispatcher m_ev_dispatcher;
		Rilek::event_listener m_ev_listener;
	};
}