#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>
#include <vector>

// the formatting needed to describe a single unit of the terminal screen. 
// "a pixel", but not. "a character", but not. both!
struct CharFormat {
	uint8_t fg_color_index; // this is an index into the xterm_colours array
	uint8_t bg_color_index;
	uint8_t char_index;
};

// contains all the buffers needed to render a screen.
//struct RenderableScreenBuffer {
//	std::string char_buffer;
//	std::vector<ftxui::Color> fg_colour_buffer;
//	std::vector<ftxui::Color> bg_colour_buffer;
//};

#endif // TYPES_H
