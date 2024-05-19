#include <iostream>
#include <cstring>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


namespace gg {

    std::string network_custom(std::string custom_network_message) {
        //Create a UDP socket
        int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
        if (clientSocket == -1) {
            std::cout << "Error creating socket" << std::endl;
            return "Error: Could Not Make A Socket To Contact Server With";
        }

        //Set up server address
        struct sockaddr_in serverAddress;
        std::memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr("204.83.169.199"); //Server's IP address
        serverAddress.sin_port = htons(25587); //Server's Port

        //Send a message to the server
        sendto(clientSocket, custom_network_message.c_str(), custom_network_message.length(), 0,
               (struct sockaddr*)&serverAddress, sizeof(serverAddress));

        char buffer[1024];

        //Receive the response from the server
        ssize_t receivedBytes = recvfrom(clientSocket, buffer, sizeof(buffer), 0, nullptr, nullptr);
        if (receivedBytes == -1) {
            std::cout << "Error receiving data." << std::endl;
            close(clientSocket);
            return "Error: No Responce From Server.";
        }

        //Process received data
        buffer[receivedBytes] = '\0';

        //Close socket
        close(clientSocket);
        return buffer;
    }

}