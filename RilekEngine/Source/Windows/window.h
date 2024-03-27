#pragma once

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

namespace Rilek::Window
{
	class window_system;
	// base class for an object representing our window
	class window
	{
	public:
		window();
		virtual bool init(window_system& t_window_system) { return false; };
		virtual bool update(window_system & t_window_system) { return false; };
		virtual bool end(window_system& t_window_system) { return false; };


	protected:
		int m_width, m_height;
	};

}