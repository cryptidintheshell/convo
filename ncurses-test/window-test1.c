#include "window.h"

int main(int argc, char const *argv[]) {
	const char* items[] = {"test1" , "test2", "test3"};
	int itemSize = sizeof(items) / sizeof(items[0]);

	initscr();
	noecho();  // dont display typed char
	curs_set(0); // dont display cursor
	nodelay(stdscr, TRUE); // make getch non blocking
	clear(); refresh();

	WinBoxConfig wConfig1 = create_winbox_config(40, 5, 0, 0, true, 1, 1, "Window 1");
	WinBoxConfig wConfig2 = create_winbox_config(40, 10, 0, wConfig1.h, true, 1, 1, "Logs");

	WINDOW *win1 = CreateWindow(&wConfig1);
	WINDOW *win2 = CreateWindow(&wConfig2);

	int currentFocused = 1;
	WINDOW *windowList[] = {win1, win2};
	WinBoxConfig windowConfList[] = {wConfig1, wConfig2};

	for (;;) {
		char c = getch();
		if (c == 'q') break;
		else if (c == 'k') {
			int prev = currentFocused;
			currentFocused = ((currentFocused + 1) > 1) ? 0 : currentFocused + 1;
			WindowUnFocusedTitle(&windowConfList[prev], windowList[prev]);
		}

		WindowFocusedTitle(&windowConfList[currentFocused], windowList[currentFocused]);
	}

	getch(); endwin();
	return 0;
}	
