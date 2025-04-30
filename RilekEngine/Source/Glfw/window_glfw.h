#pragma once
#include "Windows/window.h"

// GLFW classes
class GLFWwindow;

// Rilek classes
class Rilek::Window::window_system;

namespace Rilek::Glfw
{
	class window_glfw : public Window::window
	{
	public:
		window_glfw();
		virtual bool init(Rilek::Window::window_system& t_window_system) override;
		virtual bool update(Rilek::Window::window_system& t_window_system) override;
		virtual bool end(Rilek::Window::window_system& t_window_system) override;
	private:
		GLFWwindow* mp_window;
	};

}