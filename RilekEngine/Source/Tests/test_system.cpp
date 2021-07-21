#include "stdafx.h"
#include "test_system.h"

#include <iostream>

#include "ECS/Components/test_component.h"

void Rilek::test_system::init(Rilek::ECS::world& world)
{
	for (int i = 0; i < 10; ++i)
	{
		world.create_entity();
	}
	m_ev_listener.subscribe<Events::test_event, &test_system::test_event_trigger>(this);

}

void Rilek::test_system::update(Rilek::ECS::world& world, float dt)
{
	//RLK_TRACE("{0}", test);
	test = 0;

	int command, val;

	std::cout << "Component values: ";
	world.update_component_all<test_component>([](test_component& c) { std::cout << c.m_value << " "; });
	std::cout << std::endl;

	std::cin >> command;

	switch (command)
	{
	case 0:
	{
		std::cin >> val;
		world.remove_component<test_component>(val);
	}
		break;
	case 1:
	{
		std::cin >> val;
		test_component c;
		c.m_value = val;
		world.add_component<test_component>(val, c);
	}
		break;
	case 2:
	{
		std::cin >> val;
		Events::test_event ev;
		ev.value = val;
		m_ev_dispatcher.dispatch_event(ev);
	}
		break;
	default:
	{
		RLK_TRACE("Input 0 to remove component. Input 1 to add component. Input 2 to send event ");
	}
		break;
	}
}

void Rilek::test_system::fixed_update(float dt)
{
	++test;
	
}

void Rilek::test_system::test_event_trigger(const Events::test_event& e)
{
	RLK_TRACE("EVENT : {0}", e.value);
}
