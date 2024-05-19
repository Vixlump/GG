#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 25589
#define BUFFER_SIZE 1024

#define loop while(true)

namespace gg{

void sendFile(int clientSocket, std::string filename) {
    std::ifstream file("archive/"+filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    char buffer[BUFFER_SIZE];
    while (file.read(buffer, sizeof(buffer))) {
        send(clientSocket, buffer, sizeof(buffer), 0);
    }
    send(clientSocket, buffer, file.gcount(), 0); //Send remaining bytes

    file.close();
}

void server_downloader() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(serverSocket);
        return;
    }

    if (listen(serverSocket, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Waiting for connections..." << std::endl;

    loop{
	    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
	    if (clientSocket < 0) {
	        std::cerr << "Accept failed" << std::endl;
	        close(serverSocket);
	        return;
	    }

	    char filename[BUFFER_SIZE] = {0};
	    ssize_t read_values = read(clientSocket, filename, BUFFER_SIZE);
	    std::cout << "Requested file: " << filename << std::endl;

	    sendFile(clientSocket, filename);

	    close(clientSocket);
	}
    close(serverSocket);

    return;
}

}