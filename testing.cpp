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

namespace gg_test{

}

int main() {
	std::string ADDRESS = "127.0.0.1";
	constexpr size_t BUFFER_SIZE = 1024;
	constexpr int PORT = 25588;

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    //create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    //convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ADDRESS.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    //connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }

    //open the file to be sent
    std::ifstream infile("temp.GIF", std::ios::binary);
    if (!infile) {
        std::cout << "File open error" << std::endl;
        close(sock);
        return -1;
    }

    //read from the file and send the data to the server
    while (infile.read(buffer, BUFFER_SIZE)) {
        send(sock, buffer, infile.gcount(), 0);
    }

    //send any remaining data
    if (infile.gcount() > 0) {
        send(sock, buffer, infile.gcount(), 0);
    }

    std::cout << "File sent successfully" << std::endl;

    //this segment here causes a segfault fix later
    // //receive confirmation message from server
    // int bytes_received = read(sock, buffer, BUFFER_SIZE);
    // if (bytes_received > 0) {
    //     std::cout << "Server said: " << std::string(buffer, bytes_received) << std::endl;
    // }

    //clean up
    infile.close();
    close(sock);

    return 0;
}