#include "stdafx.h"
#include "test_system.h"

#include <iostream>

#include "ECS/Components/test_component.h"
#include "Tools/Containers/vector.h"

#define NO_TEST 0
#define RUN_COMPONENT_TEST 1
#define RUN_VECTOR_TEST 2
#define RUN_TEST RUN_COMPONENT_TEST

void Rilek::test_system::ComponentTest(Rilek::ECS::world& world, float dt)
{
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
		std::cout << "Insert Component value to remove: ";
		std::cin >> val;
		world.remove_component<test_component>(val);
	}
	break;
	case 1:
	{
		std::cout << "Insert Component value to add: ";
		std::cin >> val;
		test_component c;
		c.m_value = val;
		world.add_component<test_component>(val, c);
	}
	break;
	case 2:
	{
		std::cout << "Insert event value to test: ";
		std::cin >> val;
		Rilek::test_event ev;
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

class vector_test_obj
{
public:
	int m_a;
	float m_b, m_c;
	vector_test_obj()
		: m_a(0)
		, m_b(0.f)
		, m_c(0.f)
	{
		RLK_TRACE("vector_test_obj(). m_a : {0}, m_b: {1}, m_c: {2}", m_a, m_b, m_c);
	}
	vector_test_obj(int a)
		: m_a(a)
		, m_b(0.f)
		, m_c(0.f)
	{
		RLK_TRACE("vector_test_obj(int). m_a : {0}, m_b: {1}, m_c: {2}", m_a, m_b, m_c);
	}
	vector_test_obj(float b, float c)
		: m_a(0)
		, m_b(b)
		, m_c(c)
	{
		RLK_TRACE("vector_test_obj(float, float). m_a : {0}, m_b: {1}, m_c: {2}", m_a, m_b, m_c);
	}
	vector_test_obj(const vector_test_obj& other)
		: m_a(other.m_a)
		, m_b(other.m_b)
		, m_c(other.m_c)
	{
		RLK_TRACE("vector_test_obj(const vector_test_obj&). m_a : {0}, m_b: {1}, m_c: {2}", m_a, m_b, m_c);
	}
	vector_test_obj(vector_test_obj&& other)
		: m_a(other.m_a)
		, m_b(other.m_b)
		, m_c(other.m_c)
	{
		RLK_TRACE("vector_test_obj(vector_test_obj&&). m_a : {0}, m_b: {1}, m_c: {2}", m_a, m_b, m_c);
	}
	~vector_test_obj()
	{
		RLK_TRACE("~vector_test_obj(). m_a : {0}, m_b: {1}, m_c: {2}", m_a, m_b, m_c);
	}

	void print()
	{
		RLK_TRACE("print(). m_a : {0}, m_b: {1}, m_c: {2}", m_a, m_b, m_c);
	}
};

void Rilek::test_system::VectorTest(Rilek::ECS::world& world)
{
	Rilek::vector<vector_test_obj> vec;
	vec.emplace_back();
	vec.emplace_back(1);
	vec.emplace_back(1.0f, 2.0f);
	vec.reserve(6);
}

void Rilek::test_system::init(Rilek::ECS::world& world)
{
	for (int i = 0; i < 10; ++i)
	{
		world.create_entity();
	}
	m_ev_listener.subscribe<Rilek::test_event, &test_system::test_event_trigger>(this);

#if RUN_TEST == RUN_VECTOR_TEST
	RLK_TRACE("dt: {0}", test);
	VectorTest(world);
#endif
}

void Rilek::test_system::update(Rilek::ECS::world& world, float dt)
{
#if RUN_TEST == RUN_COMPONENT_TEST
	RLK_TRACE("dt: {0}", test);
	ComponentTest(world, dt);
#endif
}

void Rilek::test_system::fixed_update(float dt)
{
	++test;
}

void Rilek::test_system::test_event_trigger(const Rilek::test_event& e)
{
	RLK_TRACE("EVENT : {0}", e.value);
}
