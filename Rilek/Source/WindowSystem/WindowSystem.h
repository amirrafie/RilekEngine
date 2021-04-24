#pragma once


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
namespace Rilek::Window
{
	class WindowSystem
	{
	public:
		void Init(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
		void Update();
		void End();
	};
}