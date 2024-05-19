#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <iostream>

class Connection
{

	int listen_backlog = 32;

	void error_msg(std::string &msg)
	{
		std::cout << "ERROR: " << msg << '\n';
	}

	bool connect(std::string &url)
	{

		struct addrinfo *res;
		struct addrinfo hints;
		std::memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_flags = AI_ALL | AI_V4MAPPED; // so that all eligible IPv4 and IPv6 addresses are returned
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = 0;

		if (getaddrinfo(url, nullptr,  &hints, &res) != 0)
		{
			error_msg("getaddrinfo");
			return false;
		}

		int sock;
		
		for (struct addrinfo *r = res; r != nullptr; r = r->ai_next)
		{
			// only IPv4 addrresses
			if ( r->ai_family == AF_INET  )
			{
				if ( (sock = socket(r->ai_family, r->ai_socktype, r->ai_protocol)) == -1 )
				{
					error_msg("socket connection");
					return false;
				}
			}
		}
	}


	bool serve(int port)
	{
		int accepter = socket(AF_INET, SOCK_STREAM, 0);	
		
		struct sockaddr_in addr;
		std::memset(&addr, 0, sizeof(struct sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		
		if (bind(accepter, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)	
		{
			error_msg("bind");
			return false;
		}

		if (listen(accepter, listen_backlog) == -1)
		{
			error_msg("listen");
			return false;
		}

		for (;;)
		{
			int cli_sock = accept(accepter, nullptr, nullptr);	
			if (cli_sock == -1)
			{
				error_msg("connect");
				return false;
			}
			
			// let client know that connection has been established
			message(cli_sock, "Connection Established\n");
			
		}	

		return true;

	}

	bool message(int client, std::string &msg)
	{
		std::size_t bytes_written = 0;
		std::size_t send_result = 0;

		char *msg_ptr = msg.c_str();
		
		while (bytes_written < msg.length())
		{
			send_result = send(client, (void *)msg_ptr, (std::size_t)msg.length(), 0);
			if (send_result == -1)
			{
				error_msg("send");
				return false;
			}
		}

		return true;
	
	}


	bool onMessage()
	{
		
	}
};
