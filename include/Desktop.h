#ifndef SM_DESKTOP_H
#define SM_DESKTOP_H

#include <Singleton.h>

#include <Window.h>

#define MAX_WINDOW_HANDLES 50000
#define FOREGROUND_DRAW_POSITION 1
#define ALWAYS_ON_TOP_DRAW_POSITION 0

enum SystemFonts
{
    SYSFONT_WINDOW_TITLE = 0,
    SYSFONT_WINDOW_TEXT  = 1,
    MAX_SYSFONT
};

class Desktop
{
    public:
        Desktop();
        ~Desktop();

        void Init();

        void MouseEvent(bool left, bool press);
        void MouseMove();

        int32 RegisterWindow(SMWindow* target);
        void UnregisterWindow(int32 handle, SMWindow* target);
        void SetForegroundWindow(int32 handle);
        SMWindow* GetWindow(int32 handle);

        int32 GetFontID(SystemFonts fontpos) { return m_systemFonts[fontpos]; };

        void Draw();

    private:
        std::vector<SMWindow*> m_windowHandles;
        int32 m_foregroundWindow;

        int32 m_systemFonts[MAX_SYSFONT];
};

#define sDesktop Singleton<Desktop>::getInstance()

#define FONT(a) sDesktop->GetFontID(a)

#endif
