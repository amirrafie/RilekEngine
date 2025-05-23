#include "stdafx.h"
#include "glfw_system.h"

#include "GLFW/glfw3.h"

// Doesn't really do much apart from init and terminate glfw

namespace Rilek::Glfw
{

void glfw_system::init()
{
	glfwInit();
}

void glfw_system::update(Rilek::ECS::world&, float)
{

}
void glfw_system::end(Rilek::ECS::world&)
{
	glfwTerminate();
}

} // namespace Rilek::Glfw