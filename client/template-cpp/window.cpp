#include "window.h"

// Constructor replaces create_winbox_config
WinBox(int width, int height, int x, int y, bool showTitle, std::string t)
    : w(width), h(height), px(x), py(y), display(showTitle), title(std::move(t)) {
    
    winHandle = newwin(h, w, py, px);
    box(winHandle, 0, 0);
    
    if (display) {
        drawTitle(A_NORMAL);
    }
    wrefresh(winHandle);
}

~WinBox() {
    if (winHandle) {
        delwin(winHandle);
    }
}

void drawTitle(attr_t attribute) {
    wattron(winHandle, attribute);
    mvwprintw(winHandle, 0, 2, " %s ", title.c_str());
    wattroff(winHandle, attribute);
    wrefresh(winHandle);
}

void focus() {
    drawTitle(A_STANDOUT);
}

void unfocus() {
    drawTitle(A_NORMAL);
}