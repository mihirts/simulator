#include <iostream>
#include <thread>
#include <vector>
#include "TcpClientConnection.h"

const std::string SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 5000;
const int NUMBER_OF_CLIENTS = 5;

void runClient(int clientNumber) {
    TcpClientConnection client(SERVER_IP, SERVER_PORT);
    if (client.connectToServer()) {
        std::string message = "Hello from client " + std::to_string(clientNumber);
        client.sendMessage(message);

        std::string response = client.receiveMessage();
        std::cout << "Client " << clientNumber << " received: " << response << std::endl;

        client.disconnect();
    }
}

int main() {
    std::vector<std::thread> clientThreads;

    // Launch multiple clients as separate threads
    for (int i = 0; i < NUMBER_OF_CLIENTS; ++i) {
        clientThreads.push_back(std::thread(runClient, i + 1));
    }

    // Wait for all threads to complete
    for (auto& t : clientThreads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "All clients have completed communication with the server." << std::endl;
    return 0;
}
