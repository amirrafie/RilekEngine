// RilekEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "Tools/Delegate/delegate.h"
#include "Core/core_engine.h"

#define MAX_LOADSTRING 100


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool isRunning = true;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow){
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    int argc = __argc;
    char** argv = __argv;
    /////////////////////////////////////////////////////////////////
    Rilek::Core::engine engine;
    engine.init(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    engine.update();
    engine.end();
    /////////////////////////////////////////////////////////////////
    return 0;
}
