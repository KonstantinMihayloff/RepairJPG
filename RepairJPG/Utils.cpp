#include <iostream>
#include <filesystem>

#include "Utils.hpp"

void printHex(const std::vector<unsigned char>& buffer) {
    for (size_t i = 0; i < buffer.size(); ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(buffer[i]) << " ";

        if ((i + 1) % 16 == 0)
            std::cout << "\n";
    }
    std::cout << "The binary code of the file is:\n" << std::endl;
    std::cout << std::dec << std::endl; // revenir en base 10
}

bool readWord(std::ifstream& in, unsigned short& out) {
    unsigned char a, b;
    if (!in.read(reinterpret_cast<char*>(&a), 1)) return false;
    if (!in.read(reinterpret_cast<char*>(&b), 1)) return false;
    out = (a << 8) | b;
    return true;
}