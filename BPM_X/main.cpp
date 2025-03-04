#include "BMPProcessor.h"
#include <iostream>

int main() {
    std::string inputFile;
    std::cout << "Enter input BMP file name: ";
    std::cin >> inputFile;

    try {
        BMPProcessor bmp(inputFile);
        bmp.processAndSave();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}