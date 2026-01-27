#include "../headers/messenger.h"

int Messenger::GetServerList() {
    printw("[+] Requesting server list...\n");
    refresh();

    SendToServer(mainClientSocket, "servers");
    std::vector<std::string> serverList;

    while (1) {
        std::string reply = ListenToServer(mainClientSocket);

        if (reply.find("servers:")) {
            // int pos = reply.find(":");
            // ss.seekg(pos, std::ios::beg);

            std::string buffer;
            std::stringstream ss(reply);

            while (ss >> buffer) {
                serverList.push_back(buffer);
            } break;
        }
    }

    int ch;
    int currentSelected = 0;

    PrintServerListMenu(serverList, currentSelected);
    refresh();

    bool selected = false;
    while((ch = getch()) != 'q') {
        switch (ch) {
        case 'w':
            if ((currentSelected - 1) != -1) currentSelected -= 1;
            break;
        case 's':
            if ((currentSelected + 1) != 5) currentSelected += 1;
            break;
        case KEY_ENTER : case 10 :  case 13:

            selected = true;
            break;

        default:
            break;
        }

        if (selected) {
            selectedServerPort = std::stoi(serverList[currentSelected]);
            break;
        }

        erase();
        PrintServerListMenu(serverList, currentSelected);
        refresh();
    }

    printw("Selected: %s", serverList[currentSelected].c_str());
    refresh();
    getch();

    return currentSelected;
}

void Messenger::PrintServerListMenu(std::vector<std::string> serverList, int currentSelected) {
    erase();
    printw("\n\n========== Server list ==========\n");
    refresh();

    // loop through server list
    int count = 0;
    for (std::string s : serverList) {
        printw("%i. %s", count+1, s.c_str());
        if (count == currentSelected) printw(" <-");
        printw("\n");
        refresh();

        count++;
    }

    printw("========== Server list ==========\n");
    refresh();
}