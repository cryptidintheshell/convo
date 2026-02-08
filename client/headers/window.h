#include "includes.h"

class WinBox {
private:
    WINDOW* winHandle;
    int w, h, sx, sy, px, py;
    bool display;
    std::string title;
    int cursorPosX = 1;
    int cursorPosY = 2;

public:
    WinBox(int width, int height, int x, int y, bool show, std::string t);
    ~WinBox();

    void DeleteBox();

    void Print(std::string txt);
    void Print(int px, int py, std::string txt);

    std::string GetInput();
    void DrawTitle(attr_t attribute);
    void Focus();
    void Unfocus();
    WINDOW* GetHandle() { return winHandle; }
};



// handler for windows (stacking, hierchy, etc..)
class WindowManager : public WinBox {
public:
    int stackHierchy = 0;

    std::vector<WinBox> allWindows;
    std::vector<WinBox> activeWindows;
};