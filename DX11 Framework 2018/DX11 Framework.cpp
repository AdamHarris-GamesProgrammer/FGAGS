#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    Application* app = new Application();


	if (FAILED(app->Initialise(hInstance, nCmdShow)))
	{
		return -1;
	}

    // Main message loop
    MSG msg = {0};

    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            //translates messages from windows such as mouse moving
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            //Game loop
			app->Update();
            app->Draw();
        }
    }

    delete app;
    app = nullptr;

    return (int) msg.wParam;
}