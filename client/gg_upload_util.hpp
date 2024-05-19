//this
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <deque>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <map>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define loop while(true)

namespace gg{



bool upload_file(std::string file_name) {
	std::string ADDRESS = "204.83.169.199";//204.83.169.199
	constexpr size_t BUFFER_SIZE = 1024;
	constexpr int PORT = 25588;

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    //create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    //convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ADDRESS.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return false;
    }

    //connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return false;
    }

    //open the file to be sent
    std::ifstream infile(file_name, std::ios::binary);
    if (!infile) {
        std::cout << "File open error" << std::endl;
        close(sock);
        return false;
    }

    //read from the file and send the data to the server
    while (infile.read(buffer, BUFFER_SIZE)) {
        send(sock, buffer, infile.gcount(), 0);
    }

    //send any remaining data
    if (infile.gcount() > 0) {
        send(sock, buffer, infile.gcount(), 0);
    }

    std::cout <<file_name<<" sent successfully" << std::endl;
    infile.close();

    /*// Receive the response from the server
    char new_buffer[1024];
    std::cout<<"Hit\n";
    ssize_t receivedBytes = recv(sock, new_buffer, sizeof(new_buffer) - 1, 0);
    std::cout<<"Hit2\n";
    if (receivedBytes == -1) {
        std::cout << "Error receiving data." << std::endl;
        close(sock);
        return -1;
    }

    // Null-terminate and process received data
    new_buffer[receivedBytes] = '\0';
    std::cout << "Received data: " << new_buffer << std::endl;
    */
    //clean up
    close(sock);

    return true;
}

}