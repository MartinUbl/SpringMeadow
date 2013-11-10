#include <global.h>
#include <System.h>
#include <Desktop.h>
#include <Helpers.h>

SMSystem::SMSystem()
{
    m_mouseX = 0;
    m_mouseY = 0;
}

SMSystem::~SMSystem()
{
}

void _KeyEvent(uint16 key, bool press)
{
    sSystem->KeyEvent(key, press);
}

void _MouseEvent(bool left, bool press)
{
    sSystem->MouseEvent(left, press);
}

void SMSystem::Init()
{
    sDesktop->Init();
    SF->Interface->HookEvent(0, &_KeyEvent);
    SF->Interface->HookMouseEvent(&_MouseEvent);
}

void SMSystem::KeyEvent(uint16 key, bool press)
{
    //
}

void SMSystem::MouseEvent(bool left, bool press)
{
    sDesktop->MouseEvent(left, press);
}

void SMSystem::MouseMove(uint32 newX, uint32 newY)
{
    m_mouseX = newX;
    m_mouseY = newY;

    sDesktop->MouseMove();
}

void SMSystem::Run()
{
    MSG msg;

    SMWindow* a = new SMWindow();
    a->Create(20, 50, 400, 300, "Test Window na hovno");
    sDesktop->SetForegroundWindow(a->GetHandle());

    SMWindow* b = new SMWindow();
    b->Create(100, 230, 300, 200, "Test Window na 2 hovna");
    sDesktop->SetForegroundWindow(b->GetHandle());

    while (true)
    {
        if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
        {
            if (msg.message != WM_QUIT)
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else
                break;
        }

        SF->BeforeDraw();

        sDesktop->Draw();

        SF->AfterDraw();
    }
}
