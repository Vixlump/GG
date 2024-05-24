#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <string>
#include <utility>
#include <cstdlib>
#include <memory>

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
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

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

#endif // UTILS_H