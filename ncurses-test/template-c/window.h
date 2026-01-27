#include <ncurses.h>
#include <stdlib.h>

// create an alias for struct, so that we can just use WinBox
typedef struct {
    int w, h, sx, sy, px, py;
    char* title;
    bool display;
} WinBoxConfig;

WinBoxConfig create_winbox_config(int width, int height, int px, int py, bool d, int sx, int sy, char* t) {
    WinBoxConfig wb;
    wb.w = width;
    wb.h = height;
    wb.display = d;
    wb.sx = sx;
    wb.sy = sy;
    wb.px = px;
    wb.py = py;
    wb.title = t;
    return wb;
}

WINDOW* CreateWindow(WinBoxConfig* wb) {
    WINDOW *winTemp = newwin(wb->h, wb->w, wb->sy, wb->sx);
    box(winTemp, 0, 0);
    mvwin(winTemp, wb->py, wb->px);
    if (wb->display) {
	    mvwprintw(winTemp, 0, 2, "%s", wb->title);
    	wrefresh(winTemp);
    }

    return winTemp;
}

void WindowFocusedTitle(WinBoxConfig *wb, WINDOW *win) {
	wattron(win, A_STANDOUT);
    mvwprintw(win, 0, 2, "%s", wb->title);
	wrefresh(win);
	wattroff(win, A_STANDOUT);
}

void WindowUnFocusedTitle(WinBoxConfig *wb, WINDOW *win) {
	wattron(win, A_NORMAL);
    mvwprintw(win, 0, 2, "%s", wb->title);
	wrefresh(win);
	wattroff(win, A_NORMAL);
}