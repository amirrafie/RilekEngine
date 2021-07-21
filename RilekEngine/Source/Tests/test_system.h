#pragma once
#include "ECS/world.h"

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
	};
}