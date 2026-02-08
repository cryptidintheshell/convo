#include "../headers/messenger.h"

std::string Messenger::GetServerList(std::atomic<bool> &stopFlag) {
    SendToServer(mainClientSocket, "servers");
    int attempts = 0;

    while (attempts < 10) {
        if (stopFlag.load()) {
            return "cancelled";
        }

        std::string reply = ReceiveFromServer(mainClientSocket);

        if (reply == "socket_error" || reply == "socket_closed") {
            return "connection_lost";
        }

        if (reply.find("servers.") != std::string::npos) {
            return "ok";
        }

        attempts++;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return "timeout";
}

// if (sscanf(reply, "servers.%i,%i,%i,%i") == 4) {}


// void Messenger::PrintServerListMenu(WINDOW *win, std::vector<std::int> serverList, int currentSelected) {
//     erase();
//     refresh();

//     // loop through server list
//     int count = 0;
//     for (int s : serverList) {
//         printw("%i. %s", count+1, s.c_str());
     
//         if (count == currentSelected) printw(" <");
//         wprintw("\n");
//         wrefresh(win);

//         count++;
//     }

//     wrefresh();
// }