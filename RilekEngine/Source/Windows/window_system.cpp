#include "stdafx.h"
#include "window_system.h"

#include "Core/core_engine.h"
#include "window.h"

#include "Glfw/window_glfw.h"

namespace Rilek::Window
{
	void window_system::create_window()
	{
		mp_current_window = new Rilek::Glfw::window_glfw();
	}

	window_system::window_system()
        : mp_current_window(nullptr)
    {
    }

	void window_system::init()
	{
		create_window();
		mp_current_window->init(*this);
	}

    void window_system::update(Rilek::ECS::world&, float)
	{
		if (!mp_current_window->update(*this))
			Rilek::Core::engine::get_engine()->stop();
	}

	void window_system::end(Rilek::ECS::world&)
	{
		if (mp_current_window)
		{
			mp_current_window->end(*this);
			delete mp_current_window;
		}
	}
}