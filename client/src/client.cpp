#include "../headers/client.h"

SocketClient::~SocketClient() {
    close(mainClientSocket);
}

bool SocketClient::SetupSocket(int& cSocket, sockaddr_in &server, int port) {

    cSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (cSocket < 0) {
        perror("[!] cSocket: ");
        exit(1);
        return false;
    }

    // clear
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0) {
        perror("[!!] inet_pton");
        close(cSocket);
        exit(1);
        return false;
    }

    return true;
}

bool SocketClient::ConnectToTheServer(int& clientSocket, const sockaddr_in& serverInfo) {
    if (connect(clientSocket, reinterpret_cast<const sockaddr*>(&serverInfo), sizeof(serverInfo)) < 0) {
        perror("[!] connect");
        return false;
    }

    return true;
}

bool SocketClient::SendToServer(int& socketFd, const std::string& msg) {
    // const char* info = (msg + '\0').c_str();
    size_t msgLength = msg.length();
    int res = send(socketFd, msg.c_str(), msgLength, 0);

    if (res < 0) {
        perror("[!] Send error");
        return false;
    } return true;
}

std::string SocketClient::ReceiveFromServer(int& socketFd) {
	char buffer[BUFFER_SIZE];
    std::string msg(buffer);

    ssize_t bytes = recv(socketFd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes < 0) {
        perror("[!] Server closed.");
        exit(1);
    }

    buffer[bytes] = '\0';
    return std::string(buffer);
}
