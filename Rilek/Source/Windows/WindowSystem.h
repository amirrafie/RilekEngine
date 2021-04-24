#pragma once


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
namespace Rilek::Window
{
	class WindowSystem
	{
	public:
		void Init() {} // to be able to create for engine
		void InitWindows(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
		void Update(float);
		void End();
	};
}