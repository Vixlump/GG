#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <iostream>

class Client
{

	private : 
		int sock = 0;

		void error_msg(const std::string &msg) { std::cout << "ERROR: " << msg << '\n'; }

	public:
		bool connect(std::string &url)
		{

			struct addrinfo *res;
			struct addrinfo hints;
			std::memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_flags = AI_ALL | AI_V4MAPPED; // so that all eligible IPv4 and IPv6 addresses are returned
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = 0;

			const char *url_ptr = url.c_str();

			if (getaddrinfo(url_ptr, nullptr,  &hints, &res) != 0)
			{
				error_msg("getaddrinfo");
				return false;
			}
			
			for (struct addrinfo *r = res; r != nullptr; r = r->ai_next)
			{
				// only try IPv4 addresses
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

};