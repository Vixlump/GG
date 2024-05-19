//this
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 25589
#define BUFFER_SIZE 1024

namespace gg {

void receiveFile(int serverSocket, std::string & filename) {
    std::ofstream file("downloads/"+filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to create file: " << filename << std::endl;
        return;
    }

    char buffer[BUFFER_SIZE];
    int bytesReceived;
    while ((bytesReceived = recv(serverSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        file.write(buffer, bytesReceived);
    }

    file.close();
}

int util_download_file(std::string filename) {
    int clientSocket;
    struct sockaddr_in serverAddr;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("204.83.169.199");

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Connection failed" << std::endl;
        close(clientSocket);
        return -1;
    }
    std::cout << "Sending Info:";

    send(clientSocket, filename.c_str(), filename.size(), 0);

    std::cout << "Reciving Info...";

    receiveFile(clientSocket, filename);

    std::cout << "Info Recived.";

    close(clientSocket);

    return 0;
}

}