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

	if (command == 1)
	{
		std::cin >> val;
		test_component c;
		c.m_value = val;
		world.add_component<test_component>(val, c);
	}
	else if (command == 0)
	{
		std::cin >> val;
		world.remove_component<test_component>(val);
	}
	else
	{
		RLK_TRACE("Input 1 to add component. Input 0 to remove component");
	}

}

void Rilek::test_system::fixed_update(float dt)
{
	++test;
	
}
