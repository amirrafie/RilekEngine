// RilekEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "Tools/Delegate.h"
#include "Core/CoreEngine.h"

#define MAX_LOADSTRING 100


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool isRunning = true;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow){
    /////////////////////////////////////////////////////////////////
    Rilek::Core::Engine engine;
    engine.Init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    engine.Update();
    engine.End();
    /////////////////////////////////////////////////////////////////
    return 0;
}
