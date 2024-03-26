#pragma once
#include "ECS/world.h"

namespace Rilek::Glfw
{
	class glfw_system
	{
	public:
		void init();
		void update(Rilek::ECS::world&, float);
		void end(Rilek::ECS::world&);
	};
}