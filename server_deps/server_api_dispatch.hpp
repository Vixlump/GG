//this
#define loop while(true)
namespace gg {

	void api_disptch_listener() {
		//Setup
		const int PORT = 25542;//port for development remember to switch back for deployment
		const int BUFFER_SIZE = 1024;//if you find the buffer too small change this number
		int sockfd;
	    char buffer[BUFFER_SIZE];
	    std::string buffer_manager;//string type to allow easier buffer parcing and management

	    //Create socket
	    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	    if (sockfd < 0) {
	        std::cerr << "Socket creation failed." << std::endl;
	        return;
	    }

	    //Configure socket address
	    struct sockaddr_in serverAddr, clientAddr;
	    serverAddr.sin_family = AF_INET;
	    serverAddr.sin_port = htons(PORT);
	    serverAddr.sin_addr.s_addr = INADDR_ANY;

	    //Bind socket
	    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
	        std::cerr << "Binding failed." << std::endl;
	        return;
	    }

	    std::cout << "Dispatch Server UDP is listening on port " << PORT << std::endl;

	    //Start server listening loop
	    std::string return_message;
	    loop {
	    	//reset buffer
	        memset(buffer, 0, sizeof(buffer));
	        socklen_t clientLen = sizeof(clientAddr);

	        //Receive packet-------------------------------------------------------------------
	        //idk what any of this does, but it seems to work so I just won't touch it.
	        ssize_t bytesRead = recvfrom(sockfd, buffer, sizeof(buffer), 0,
	        	(struct sockaddr *)&clientAddr, &clientLen);
	        if (bytesRead < 0) {
	            std::cerr << "Error receiving packet." << std::endl;
	            continue;
	        }
	        buffer_manager = buffer;
	        //Output buffer to the terminal
	        std::cout << "Received message from " << inet_ntoa(clientAddr.sin_addr)
	        	<< ":" << ntohs(clientAddr.sin_port) << ": " << buffer_manager << "\n";
	       	//Do something with the buffer
	        return_message = buffer_processor(buffer_manager);

	        //Send response back --------------------------------------------------------------
	        if (return_message.length()>1000) {
	        	return_message == "Requested data return was too large for server to process.";
	        }
	        sendto(sockfd, return_message.c_str(), return_message.length(), 0,
	               (struct sockaddr *)&clientAddr, clientLen);
	    }

	    //Close socket
	    close(sockfd);
	    return;
	}

	std::string generate_api_key() {

	}
	bool check_api_key() {

	}
}