//this
#define loop while(true)
namespace gg {

	std::string key_maker() {
		std::mt19937_64 key_generator(time(nullptr));
		return std::to_string(key_generator());
	}

	bool check_api_key(std::string input) {
		std::ifstream key_checker("api_key_list.txt");
		std::string key_check_storage;
		while(std::getline(key_checker, key_check_storage)) {
			if (key_check_storage==input) {
				return true;
			}
		}
		return false;
	}

	std::string generate_api_key() {
		
		std::string api_key = "api_"+key_maker();
		if (check_api_key(api_key)) {
			return "0";
		}
		std::ofstream outfile;

		outfile.open("api_key_list.txt", std::ios_base::app); //append instead of overwrite
		outfile << std::endl << api_key;
		outfile.close();
		return api_key;
	}

	void parse_string(std::string input, std::vector <std::string> &parced_list, char delimiter) {
	    std::istringstream stream(input);
	    std::string token;
	    
	    while (std::getline(stream, token, delimiter)) {
	        parced_list.push_back(token);
	    }
	}

	std::string get_v_key() {
		std::ifstream key_checker("v_list.txt");
		std::string key_check_storage;
		while(std::getline(key_checker, key_check_storage)) {
			continue;
		}
		return key_check_storage;
	}

	std::vector <std::string> list_o_strings;
	std::string buffer_processor(std::string input) {
		list_o_strings.clear();
		parse_string(input, list_o_strings, ';');
		if (list_o_strings.size()>0) {
			if (list_o_strings[0]=="request") {
				return generate_api_key();
			} else if (list_o_strings[0]=="check_key"&&list_o_strings.size()>1) {
				return std::to_string(check_api_key(list_o_strings[1]));
			} else if (list_o_strings[0]=="get_v") {
				return get_v_key();
			}
		}
		return "0";
	}

	void api_disptch_listener() {
		//Setup
		constexpr int PORT = 25587;
		constexpr int BUFFER_SIZE = 1024;
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
	        if (return_message.length()>1024) {
	        	return_message = "Requested data return was too large for server to process.";
	        }
	        sendto(sockfd, return_message.c_str(), return_message.length(), 0,
	               (struct sockaddr *)&clientAddr, clientLen);
	    }

	    //Close socket
	    close(sockfd);
	    return;
	}
	
}