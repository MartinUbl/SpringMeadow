#include <global.h>
#include <Window.h>
#include <Desktop.h>
#include <System.h>

SMWindow::SMWindow()
{
    m_created = false;
    m_active = false;
    m_isDragging = false;
}

SMWindow::~SMWindow()
{
}

void SMWindow::Create(int32 posTop, int32 posLeft, uint32 width, uint32 height, const char* title)
{
    m_positionTop = posTop;
    m_positionLeft = posLeft;
    m_width = width;
    m_height = height;
    m_title = title;

    m_created = true;
    m_active = true;

    m_handle = sDesktop->RegisterWindow(this);
}

void SMWindow::Draw()
{
    SF->Drawing->DrawRectangle(m_positionLeft, m_positionTop, m_width, m_height, MAKE_COLOR_RGBA(255,255,255,255));
    SF->Drawing->DrawRectangle(m_positionLeft+1, m_positionTop+1, m_width-2, m_height-2, MAKE_COLOR_RGBA(200,200,200,255));

    uint32 headercolor = MAKE_COLOR_RGBA(190,190,190,255);
    if (GetDrawPosition() == FOREGROUND_DRAW_POSITION)
        headercolor = MAKE_COLOR_RGBA(80,80,80,255);
    SF->Drawing->DrawRectangle(m_positionLeft+1, m_positionTop+1, m_width-2, SMW_DEF_HEADER_HEIGHT, headercolor);

    glColor3f(0,0,0);
    char* title_print = new char[strlen(m_title)+3];
    memset(title_print, 0, strlen(m_title)+3);
    uint32 howmany = SF->Drawing->GetTextAmountToWidth(FONT(SYSFONT_WINDOW_TITLE), 0, m_width - 10, L"%S", m_title);
    if (howmany < strlen(m_title))
    {
        if (howmany > 2)
            howmany -= 2;
        strncpy(title_print, m_title, howmany);
        title_print[howmany] = '.';
        title_print[howmany+1] = '.';
        title_print[howmany+2] = '.';
    }
    else
        sprintf(title_print, m_title);
    SF->Drawing->PrintText(FONT(SYSFONT_WINDOW_TITLE), m_positionLeft + 4, m_positionTop + 5, 0, WW_NO_WRAP, L"%S", title_print);

    SF->Drawing->DrawRectangle(m_positionLeft + m_width - 16 - 2, m_positionTop + 2, 16, 16, MAKE_COLOR_RGBA(255,255,255,255));
    SF->Drawing->DrawRectangle(m_positionLeft + m_width - 16 - 1, m_positionTop + 3, 14, 14, MAKE_COLOR_RGBA(100,100,105,255));

    glColor3f(0,0,0);
    SF->Drawing->PrintText(FONT(SYSFONT_WINDOW_TITLE), m_positionLeft + m_width - 13, m_positionTop + 4, 0, WW_NO_WRAP, L"x");
}

void SMWindow::CloseWindow()
{
    sDesktop->UnregisterWindow(GetHandle(), this);

    // a bit dangerous, but since we roll up the stack by series of returns, and we identify windows by their handles (and search them by it as well), it's safe
    delete this;
}

void SMWindow::MouseEvent(bool left, bool press)
{
    int32 mx = sSystem->GetMouseX();
    int32 my = sSystem->GetMouseY();

    if (left)
    {
        if (press)
        {
            if (mx > m_positionLeft && mx < m_positionLeft + (int32)m_width &&
                my > m_positionTop  && my < m_positionTop + (int32)SMW_DEF_HEADER_HEIGHT)
            {
                if (mx > m_positionLeft + m_width - 16 - 2 && mx < m_positionLeft + m_width - 2 &&
                    my > m_positionTop + 2 && my < m_positionTop + 2 + 16)
                {
                    CloseWindow();
                    return;
                }

                m_isDragging = true;
                m_dragStartPosX = mx;
                m_dragStartPosY = my;
            }
        }
        else
        {
            if (m_isDragging)
                m_isDragging = false;
        }
    }
}

void SMWindow::MouseMove()
{
    uint32 mx = sSystem->GetMouseX();
    uint32 my = sSystem->GetMouseY();

    if (m_isDragging)
    {
        m_positionLeft += (mx - m_dragStartPosX);
        m_positionTop  += (my - m_dragStartPosY);

        m_dragStartPosX = mx;
        m_dragStartPosY = my;
    }
}

bool SMWindow::IsActive()
{
    return (m_created && m_active);
}

bool SMWindow::IsCreated()
{
    return m_created;
}
