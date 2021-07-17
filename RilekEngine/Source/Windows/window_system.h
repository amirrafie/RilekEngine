#pragma once


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
namespace Rilek::Window
{
	class window_system
	{
	public:
		void init() {} // to be able to create for engine
		void init_windows(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
		void update(float);
		void end();
	};
}