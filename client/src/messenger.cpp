#include "../headers/messenger.h"

std::string Messenger::GetServerList() {
    SendToServer(mainClientSocket, "servers");
    std::vector<std::string> serverList;
    int attempts = 0;

    while (1) {
        if (attempts > 10) return "not ok";

        std::string reply = ReceiveFromServer(mainClientSocket);
        if (reply.find("servers.") != std::string::npos) {
            break;
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            attempts++;
        } 
    }

    return "ok";
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