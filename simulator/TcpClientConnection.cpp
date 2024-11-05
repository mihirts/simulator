#include "TcpClientConnection.h"

TcpClientConnection::TcpClientConnection(const std::string& serverIp, int port) {
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        isConnected = false;
        return;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        isConnected = false;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);
    isConnected = true;
}

TcpClientConnection::~TcpClientConnection() {
    disconnect();
    WSACleanup();
}

bool TcpClientConnection::connectToServer() {
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
        isConnected = false;
        return false;
    }
    std::cout << "Connected to server" << std::endl;
    return true;
}

bool TcpClientConnection::sendMessage(const std::string& message) {
    if (!isConnected) return false;
    if (send(sockfd, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
        std::cerr << "Error sending message: " << WSAGetLastError() << std::endl;
        return false;
    }
    std::cout << "Sent to server: " << message << std::endl;
    return true;
}

std::string TcpClientConnection::receiveMessage() {
    if (!isConnected) return "";
    char buffer[1024] = { 0 };
    int bytesRead = recv(sockfd, buffer, sizeof(buffer), 0);
    if (bytesRead == SOCKET_ERROR) {
        std::cerr << "Error receiving message: " << WSAGetLastError() << std::endl;
        return "";
    }
    std::cout << "Received from server: " << buffer << std::endl;
    return std::string(buffer, bytesRead);
}

void TcpClientConnection::disconnect() {
    if (isConnected) {
        closesocket(sockfd);
        std::cout << "Disconnected from server" << std::endl;
        isConnected = false;
    }
}
