#include "stdafx.h"
#include "window_system.h"

#include "Core/core_engine.h"
#include "window.h"

#include "Glfw/glfw_system.h"

namespace Rilek::Window
{
    window_system::window_system()
        : mp_current_window(nullptr)
    {
    }

	void window_system::init()
	{
	}

    void window_system::update(Rilek::ECS::world&, float)
	{
	}

	void window_system::end(Rilek::ECS::world&)
	{
	}
}