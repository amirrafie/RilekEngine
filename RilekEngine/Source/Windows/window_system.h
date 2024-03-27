#pragma once
#include "ECS/world.h"

#define WINDOW_API_NONE 0
#define WINDOW_API_WINDOWS 1
#define WINDOW_API_GLFW 2
#define WINDOW_API WINDOW_API_NONE

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
namespace Rilek::Window
{
	class window;
	class window_system
	{
	private:
		void create_window();

	public:
		window_system();
		void init();
		void update(Rilek::ECS::world&, float);
		void end(Rilek::ECS::world&);

	private:

		window* mp_current_window;
	};
}