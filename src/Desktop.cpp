#include <global.h>
#include <Desktop.h>
#include <System.h>

Desktop::Desktop()
{
    m_foregroundWindow = -1;
}

Desktop::~Desktop()
{
}

void Desktop::Init()
{
    m_systemFonts[SYSFONT_WINDOW_TITLE] = SF->BuildFont("Arial", 10);
    m_systemFonts[SYSFONT_WINDOW_TEXT] = SF->BuildFont("Arial", 12);
}

int32 Desktop::RegisterWindow(SMWindow* target)
{
    int32 tmphandle = -1;
    for (int32 i = 0; i < m_windowHandles.size(); i++)
    {
        if (m_windowHandles[i] == NULL)
        {
            tmphandle = i;
            break;
        }
    }

    target->SetDrawPosition(-1);

    if (tmphandle != -1)
    {
        m_windowHandles[tmphandle] = target;
        return tmphandle;
    }

    tmphandle = m_windowHandles.size();

    if (tmphandle > MAX_WINDOW_HANDLES)
        return -1;

    m_windowHandles.resize(tmphandle+1);
    m_windowHandles[tmphandle] = target;

    return tmphandle;
}

void Desktop::UnregisterWindow(int32 handle, SMWindow* target)
{
    int32 drawPos = target->GetDrawPosition();

    for (int32 i = 0; i < m_windowHandles.size(); i++)
    {
        if (m_windowHandles[i] == NULL)
            continue;

        if (m_windowHandles[i]->GetDrawPosition() > drawPos)
        {
            m_windowHandles[i]->SetDrawPosition(m_windowHandles[i]->GetDrawPosition() - 1);
            if (m_windowHandles[i]->GetDrawPosition() == FOREGROUND_DRAW_POSITION)
                SetForegroundWindow(i);
        }

        if (m_windowHandles[i] == target && handle == i)
        {
            m_windowHandles[i] = NULL;
            if (i == m_windowHandles.size()-1)
                m_windowHandles.resize(i);
        }
    }
}

void Desktop::SetForegroundWindow(int32 handle)
{
    if (handle >= m_windowHandles.size() || m_windowHandles[handle] == NULL)
        return;

    // change window draw positions only if we are changing position of already drawn window and not "always on top" window
    //if (m_windowHandles[handle]->GetDrawPosition() >= FOREGROUND_DRAW_POSITION)
    {
        for (int32 i = 0; i < m_windowHandles.size(); i++)
        {
            if (m_windowHandles[i] != NULL && m_windowHandles[i]->GetDrawPosition() >= FOREGROUND_DRAW_POSITION)
                m_windowHandles[i]->SetDrawPosition(m_windowHandles[i]->GetDrawPosition() + 1);
        }
    }

    m_foregroundWindow = handle;
    m_windowHandles[handle]->SetDrawPosition(FOREGROUND_DRAW_POSITION);
}

SMWindow* Desktop::GetWindow(int32 handle)
{
    if (handle < 0 || m_windowHandles.size() <= handle)
        return NULL;

    return m_windowHandles[handle];
}

bool SortHelper(int32 handle_first, int32 handle_second)
{
    SMWindow* frst = sDesktop->GetWindow(handle_first);
    SMWindow* scnd = sDesktop->GetWindow(handle_second);

    if (!frst || !scnd)
        return false;

    return (frst->GetDrawPosition() > scnd->GetDrawPosition());
}

void Desktop::Draw()
{
    std::vector<int32> m_drawPosHandles;
    int32 tmppos = -1;

    for (int32 i = 0; i < m_windowHandles.size(); i++)
    {
        if (m_windowHandles[i] == NULL)
            continue;

        tmppos = m_windowHandles[i]->GetDrawPosition();
        if (tmppos == -1)
            continue;

        m_drawPosHandles.push_back(i);
    }

    std::sort(m_drawPosHandles.begin(), m_drawPosHandles.end(), &SortHelper);

    SMWindow* trg;
    for (int32 i = 0; i < m_drawPosHandles.size(); i++)
    {
        trg = GetWindow(m_drawPosHandles[i]);
        if (!trg)
            continue;

        if (!trg->IsActive())
            continue;

        trg->Draw();
    }
}

void Desktop::MouseEvent(bool left, bool press)
{
    int32 tx = sSystem->GetMouseX();
    int32 ty = sSystem->GetMouseY();

    SMWindow* target = NULL;
    SMWindow* fgw = GetWindow(m_foregroundWindow);

    if (fgw && tx > fgw->GetPositionLeft() && tx < fgw->GetPositionLeft() + (int32)fgw->GetWidth() &&
        ty > fgw->GetPositionTop() && ty < fgw->GetPositionTop() + (int32)fgw->GetHeight())
        target = fgw;
    else
    {
        std::vector<int32> m_drawPosHandles;
        int32 tmppos = -1;

        for (int32 i = 0; i < m_windowHandles.size(); i++)
        {
            if (m_windowHandles[i] == NULL)
                continue;

            tmppos = m_windowHandles[i]->GetDrawPosition();
            if (tmppos == -1)
                continue;

            m_drawPosHandles.push_back(i);
        }

        std::sort(m_drawPosHandles.begin(), m_drawPosHandles.end(), &SortHelper);

        for (int32 i = m_drawPosHandles.size()-1; i >= 0; i--)
        {
            fgw = GetWindow(m_drawPosHandles[i]);
            if (!fgw)
                continue;

            if (tx > fgw->GetPositionLeft() && tx < fgw->GetPositionLeft() + fgw->GetWidth() &&
                ty > fgw->GetPositionTop() && ty < fgw->GetPositionTop() + fgw->GetHeight())
            {
                target = fgw;
                break;
            }
        }
    }

    if (target)
    {
        if (press && target->GetDrawPosition() != FOREGROUND_DRAW_POSITION)
            SetForegroundWindow(target->GetHandle());

        target->MouseEvent(left, press);
    }
}

void Desktop::MouseMove()
{
    // For now, we pass mouse move event only to foreground (active) window

    SMWindow* fgw = GetWindow(m_foregroundWindow);

    if (fgw)
        fgw->MouseMove();
}
