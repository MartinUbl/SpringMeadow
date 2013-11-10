#include <global.h>
#include <System.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_MOUSEMOVE)
    {
        sSystem->MouseMove(LOWORD(lParam), HIWORD(lParam));
        return 1;
    }

    return sSimplyFlat->SFWndProc(hWnd, msg, wParam, lParam);
}

int main()
{
    if (!sSimplyFlat->CreateMainWindow("SpringMeadow", WIN_WIDTH, WIN_HEIGHT, 32, false, 60, &MyWndProc))
        return -1;

    sSystem->Init();

    sSystem->Run();

    return 0;
}
