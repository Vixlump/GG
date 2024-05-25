#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>
#include <vector>

// the formatting needed to describe a single unit of the terminal screen. 
// "a pixel", but not. "a character", but not. both!
struct CharFormat {
	uint8_t fg_color_index; // this is an index into the xterm_colors array
	uint8_t bg_color_index;
	uint8_t char_index;
};

// The parameters to minimze for computing the color lookup table
enum ColorSchemeKind {
    Arbitrary, MSE, Contrast
};

// contains all the buffers needed to render a screen.
//struct RenderableScreenBuffer {
//	std::string char_buffer;
//	std::vector<ftxui::Color> fg_color_buffer;
//	std::vector<ftxui::Color> bg_color_buffer;
//};

#endif // TYPES_H
