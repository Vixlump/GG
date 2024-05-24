#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <array>

inline void debug_log(std::string contents) {
	std::ofstream file;
	file.open("log.txt", std::ios_base::app);
	file << contents;
	file.close();
}

// for getting stdout from terminal commands
inline std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    auto pclose_wrapper = [](FILE* file) { return pclose(file); };
    std::unique_ptr<FILE, decltype(pclose_wrapper)> pipe(popen(cmd, "r"), pclose_wrapper);
    //std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

// https://stackoverflow.com/questions/166630/how-can-i-repeat-a-string-a-variable-number-of-times-in-c
inline std::string repeat(const std::string& input, unsigned num) {
    std::string repeated_string;
    repeated_string.reserve(input.size() * num);
    while (num--)
        repeated_string += input;
    return repeated_string;
}

// TODO: upgrade to c++20, b/c std::lerp has better numerical properties
inline float naive_lerp(float a, float b, float t) {
    return a + t * (b - a);
}

inline int get_fg_bg_contrast(uint32_t fg_colour, uint32_t bg_colour) {
    int fg_r = (fg_colour & 0xff0000) >> 16;
	int fg_g = (fg_colour & 0x00ff00) >> 8;
	int fg_b = fg_colour & 0x0000ff;
    
	int bg_r = (bg_colour & 0xff0000) >> 16;
	int bg_g = (bg_colour & 0x00ff00) >> 8;
	int bg_b = bg_colour & 0x0000ff;

    return (int) std::abs(
        (fg_r * 0.2126 + fg_g * 0.7152 + fg_b * 0.0722) - 
        (bg_r * 0.2126 + bg_g * 0.7152 + bg_b * 0.0722)
    );
}

inline int get_fg_bg_mse(uint32_t fg_colour, uint32_t bg_colour) {
    int fg_r = (fg_colour & 0xff0000) >> 16;
	int fg_g = (fg_colour & 0x00ff00) >> 8;
	int fg_b = fg_colour & 0x0000ff;
    
	int bg_r = (bg_colour & 0xff0000) >> 16;
	int bg_g = (bg_colour & 0x00ff00) >> 8;
	int bg_b = bg_colour & 0x0000ff;

    return (fg_r - bg_r) * (fg_r - bg_r) + (fg_g - bg_g) * (fg_g - bg_g) + (fg_b - bg_b) * (fg_b - bg_b);
}


#endif // UTILS_H