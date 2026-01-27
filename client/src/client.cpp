#include "../headers/client.h"

SocketClient::~SocketClient() {
    close(mainClientSocket);
}

void SocketClient::SetupSocket(int& cSocket, sockaddr_in &server, int port) {
    // clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    cSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (cSocket < 0) {
        perror("[!] cSocket: ");
        exit(1);
    }

    // clear
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0) {
        perror("[!!] inet_pton");
        close(cSocket);
        exit(1);
    }

    printw("[+] Done setting up the socket.\n");
    refresh();
}

bool SocketClient::ConnectToTheServer(int& clientSocket, const sockaddr_in& serverInfo) {
    if (connect(clientSocket, reinterpret_cast<const sockaddr*>(&serverInfo), sizeof(serverInfo)) < 0) {
        perror("[!] connect");
        return false;
    }

    printw("[+] Connected to the server\n");
    refresh();

    return true;
}

void SocketClient::SendToServer(int& socketFd, const std::string& msg) {
    // const char* info = (msg + '\0').c_str();
    size_t msgLength = msg.length();
    int res = send(socketFd, msg.c_str(), msgLength, 0);

    if (res < 0) {
        perror("[!] Send error");
        refresh();
    }
}

std::string SocketClient::ReceiveFromServer(int& socketFd) {
	char buffer[BUFFER_SIZE];
    std::string msg(buffer);

    ssize_t bytes = recv(socketFd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return std::string(buffer);
    } else if (bytes < 0) {
        printw("[!] Server closed. Press any key to exit");
        refresh();
        getch();
        exit(1);
    }
}
