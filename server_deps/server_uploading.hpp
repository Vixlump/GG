//this
#define loop while(true)
namespace gg{
	void upload_server_listening_loop() {
    	constexpr size_t BUFFER_SIZE = 1024;
    	constexpr int PORT = 25588;

    	std::string message = "confirm_file_recived";

        int server_fd;
        int new_socket;
        struct sockaddr_in address;
        struct sockaddr_in client_addr;
        int addrlen = sizeof(address);
        char buffer[BUFFER_SIZE] = {0};
    	socklen_t client_addrlen = sizeof(client_addr);

        //socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            std::cout<<"socket failed\n";
            return;
        }

        //bind the socket to the port
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            std::cout<<"bind failed\n";
            close(server_fd);
            return;
        }

        //listen for incoming connections
        if (listen(server_fd, 3) < 0) {
            std::cout<<"listen\n";
            close(server_fd);
            return;
        }

        std::cout << "Server listening on port: " << PORT << std::endl;

        loop {
            //accept an incoming connection
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                std::cout<<"accept\n";
                close(server_fd);
                return;
            }

            //open file to save the received data
            std::ofstream outfile("archive/temp.GIF", std::ios::binary);
            if (!outfile) {
                std::cout<<"file open\n";
                close(new_socket);
                close(server_fd);
                return;
            }

            //read the data from the socket and write it to the file
            int bytes_read;
            while ((bytes_read = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
                outfile.write(buffer, bytes_read);
            }

            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            std::cout << "File received from<"<<client_ip << ":" << ntohs(client_addr.sin_port) <<"> and saved as 'temp.GIF'" << std::endl;

            //clean up
            outfile.close();
        }
            close(new_socket);
            close(server_fd);


        return;
    }
}