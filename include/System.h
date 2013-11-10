#ifndef SM_SYSTEM_H
#define SM_SYSTEM_H

#include <Singleton.h>

class SMSystem
{
    public:
        SMSystem();
        ~SMSystem();

        void Init();
        void Run();

        void KeyEvent(uint16 key, bool press);
        void MouseEvent(bool left, bool press);
        void MouseMove(uint32 newX, uint32 newY);

        uint32 GetMouseX() { return m_mouseX; };
        uint32 GetMouseY() { return m_mouseY; };

    private:
        uint32 m_mouseX;
        uint32 m_mouseY;
};

#define sSystem Singleton<SMSystem>::getInstance()

#endif
