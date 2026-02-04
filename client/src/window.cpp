#include "../headers/window.h"

// Constructor replaces create_winbox_config
WinBox::WinBox(int width, int height, int x, int y, bool show, std::string t)
    : w(width), h(height), px(x), py(y), display(show), title(std::move(t)) {
    
    winHandle = newwin(h, w, 0, 0);
    box(winHandle, 0, 0);
    mvwin(winHandle, py, px);
    
    if (display) {
        DrawTitle(A_NORMAL);
        wrefresh(winHandle);
    }
}

WinBox::~WinBox() {
    if (winHandle) {
        delwin(winHandle);
    }
}

std::string WinBox::GetInput() {
    curs_set(1); //show cursor
    echo(); // show typed char
    keypad(winHandle, true);

    char buffer[256];
    wgetnstr(winHandle, buffer, 256);

    curs_set(0); wrefresh(winHandle);
    noecho();
    return std::string(buffer);
}

void WinBox::DrawTitle(attr_t attribute) {
    wattron(winHandle, attribute);
    mvwprintw(winHandle, 0, 2, " %s ", title.c_str());
    wattroff(winHandle, attribute);
    wrefresh(winHandle);
}

void WinBox::Focus() {
    DrawTitle(A_STANDOUT);
}

void WinBox::Unfocus() {
    DrawTitle(A_NORMAL);
}

void WinBox::Print(int px, int py, std::string txt){
    // set new cursor position
    if (px != cursorPosX) cursorPosX = px+1;
    if (px != cursorPosY) cursorPosY = py;

    mvwprintw(winHandle, py, px, "%s", txt.c_str());
    wrefresh(winHandle);
}

void WinBox::Print(std::string txt){
    mvwprintw(winHandle, cursorPosX, cursorPosY, "%s", txt.c_str());
    wrefresh(winHandle);
    cursorPosX++;
}
