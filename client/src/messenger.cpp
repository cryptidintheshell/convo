#include "../headers/messenger.h"

int Messenger::GetServerList() {

    SendToServer(mainClientSocket, "servers");
    std::vector<std::string> serverList;

    while (1) {
        std::string reply = ListenToServer(mainClientSocket);

        if (reply.find("servers.")) {
            std::vector<int> serverList;

            // if (sscanf(reply, "servers.%i,%i,%i,%i") == 4) {
            // }

        }
    }

    return currentSelected;
}

void Messenger::PrintServerListMenu(WINDOW *win, std::vector<std::int> serverList, int currentSelected) {
    erase();
    refresh();

    // loop through server list
    int count = 0;
    for (int s : serverList) {
        printw("%i. %s", count+1, s.c_str());
     
        if (count == currentSelected) printw(" <");
        wprintw("\n");
        wrefresh(win);

        count++;
    }

    wrefresh();
}