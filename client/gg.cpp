#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <iomanip>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include "gg.hpp"

std::string GG::bmp_to_ascii(const char *file_path, int w, int h, bool enable_colour, CharFormat* colour_lookup_table) {
	// I'm sorry, but I'm just quite familiar with SDL
	SDL_Surface *surf = SDL_LoadBMP(file_path);
	std::string buffer = "";

	if (surf == NULL) {
		std::cout << "Couldn't load pixel data from file " << file_path << ".\n";
		return buffer;
	}

	int og_w = w;
	int og_h = h;

	if (w > surf->w)
		w = surf->w;
	if (h > surf->h)
		h = surf->h;

	int Bpp = surf->format->BytesPerPixel;
	const char *chars = " .-:=+*oO#%@";
	float ystep = (float)surf->h / (float)h;
	float xstep = (float)surf->w / (float)w;
	int line_length, height = 0;

	int paddingy, paddingl;

	paddingy = og_h - h > 0 ? (og_h - h) / 2 : 0;
	paddingl = og_w - w > 0 ? (og_w - w) / 2 : 0;

	for (int i = 0; i < paddingy; i++) {
		buffer += '\n';
	}

	for (float yf = 0; (int)yf < surf->h; yf += ystep) {
		line_length = 0;

		for (int i = 0; i < paddingl; i++) {
			buffer += ' ';
		}

		for (float xf = 0; (int)xf < surf->w; xf += xstep) {
			// TODO: do bilinear or bicubic
			// int approx. (nearest neighbour)
			int y = (int)yf;
			int x = (int)xf;

			uint8_t *target_pixel = (uint8_t *)surf->pixels + y*surf->pitch + x*Bpp;
			uint32_t pixel = *(uint32_t *)target_pixel;
			// we don't care about the order of the RGB values in the 32 bits, sdl knows
			int r = (pixel & surf->format->Rmask) >> surf->format->Rshift;
			int g = (pixel & surf->format->Gmask) >> surf->format->Gshift;
			int b = (pixel & surf->format->Bmask) >> surf->format->Bshift;
			
			if (enable_colour) {
				// take 4 MSB of rgb 
				int r_small = (r & 0xf0) >> 4;
				int g_small = (g & 0xf0) >> 4;
				int b_small = (b & 0xf0) >> 4;

				CharFormat resultColour = colour_lookup_table[r_small * 16 * 16 + g_small * 16 + b_small];

				//int bg_r = (xterm_colours[resultColour.bg_color_index] & 0xff0000) >> 16;
				//int bg_g = (xterm_colours[resultColour.bg_color_index] & 0x00ff00) >> 8;
				//int bg_b = xterm_colours[resultColour.bg_color_index] & 0x0000ff;
				//buffer.bg_colour_buffer.push_back(ftxui::Color(bg_r, bg_g, bg_b));

				buffer += "\033[48;5;" + std::to_string(resultColour.bg_color_index) + "m";

				//int fg_r = (xterm_colours[resultColour.fg_color_index] & 0xff0000) >> 16;
				//int fg_g = (xterm_colours[resultColour.fg_color_index] & 0x00ff00) >> 8;
				//int fg_b = xterm_colours[resultColour.fg_color_index] & 0x0000ff;
				//buffer.fg_colour_buffer.push_back(ftxui::Color(fg_r, fg_g, fg_b));

				buffer += "\033[38;5;" + std::to_string(resultColour.fg_color_index) + "m";

				buffer += chars[resultColour.char_index];
			} else {
				pixel = r + g + b; // sum color components (<768)
				pixel /= 64; // (<96)
				buffer += chars[pixel];
			}

			line_length++;

			// just to make sure it doesn't go too far
			if (line_length == w) {
				break;
			}
		}

		buffer += '\n';
		height++;

		// just to make sure it doesn't go too far
		if (height == h) {
			break;
		}
	}

	if (enable_colour) {
		// reset all fancy modes & go back to default terminal text
		buffer += "\033[0m";
	}

	for (int i = 0; i < paddingy - 1; i++) {
		buffer += '\n';
	}

	SDL_FreeSurface(surf);
	return buffer;
}

bool GG::upload_file(std::string file_name) {
	std::string ADDRESS = "204.83.169.199";
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

	std::cout << "Connected to server." << std::endl;

	//open the file to be sent
	std::ifstream infile(file_name, std::ios::binary);
	if (!infile.is_open()) {
		std::cout << "File open error" << std::endl;
		close(sock);
		return false;
	}

	std::cout << "Sending frames";
	fflush(stdout);

	//read from the file and send the data to the server
	while (infile.read(buffer, BUFFER_SIZE)) {
		std::cout << ".";
		fflush(stdout);
		send(sock, buffer, infile.gcount(), 0);
	}

	//send any remaining data
	if (infile.gcount() > 0) {
		send(sock, buffer, infile.gcount(), 0);
	}

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

	std::cout << "\nDone sending frames." << std::endl;
	return true;
}

void GG::receive_file(int serverSocket, std::string & filetoken) {
	constexpr size_t BUFFER_SIZE = 1024;
	std::ofstream file("/tmp/gg/" + filetoken + ".tar", std::ios::binary);
	if (!file.is_open()) {
		std::cout << "Failed to create file: " << filetoken << std::endl;
		return;
	}

	char buffer[BUFFER_SIZE];
	int bytesReceived;
	while ((bytesReceived = recv(serverSocket, buffer, BUFFER_SIZE, 0)) > 0) {
		file.write(buffer, bytesReceived);
	}

	file.close();
}

int GG::download_file(std::string filetoken) {
	int clientSocket;
	struct sockaddr_in serverAddr;
	constexpr int PORT = 25589;

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
	std::cout << "Requesting stream...\n";

	send(clientSocket, filetoken.c_str(), filetoken.size(), 0);

	std::cout << "Receiving frames...\n";

	GG::receive_file(clientSocket, filetoken);

	std::cout << "Done.\n";

	close(clientSocket);

	return 0;
}

std::string GG::network_custom(std::string custom_network_message) {
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
