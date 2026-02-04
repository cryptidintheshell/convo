#include <ncurses.h>
#include <string>

class WinBox {
private:
    int w, h, sx, sy, px, py;
    std::string title;
    bool display;
    WINDOW* winHandle;

public:

    // Helper method for drawing titles with specific attributes
    void drawTitle(attr_t attribute);
    void focus();
    void unfocus();
    WINDOW* getHandle() { return winHandle; }
};