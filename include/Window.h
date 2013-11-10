#ifndef SM_WINDOW_H
#define SM_WINDOW_H

enum DefSMWindowValues
{
    SMW_DEF_HEADER_HEIGHT       = 18,
    SMW_DEF_HEADER_TEXT_SIZE    = 14,
};

class SMWindow
{
    public:
        SMWindow();
        ~SMWindow();
        void Create(int32 posTop, int32 posLeft, uint32 width, uint32 height, const char* title);

        void Draw();

        bool IsActive();
        bool IsCreated();

        void CloseWindow();

        int32 GetPositionTop() { return m_positionTop; };
        int32 GetPositionLeft() { return m_positionLeft; };
        uint32 GetWidth() { return m_width; };
        uint32 GetHeight() { return m_height; };

        int GetDrawPosition() { return m_drawPosition; };
        void SetDrawPosition(int32 pos) { m_drawPosition = pos; };

        void MouseEvent(bool left, bool press);
        void MouseMove();

        int32 GetHandle() { return m_handle; };

    private:
        int32 m_positionTop;
        int32 m_positionLeft;
        uint32 m_width;
        uint32 m_height;
        const char* m_title;

        bool m_isDragging;
        uint32 m_dragStartPosX, m_dragStartPosY;

        int32 m_handle;

        int32 m_drawPosition;

        bool m_created;
        bool m_active;
};

#endif
