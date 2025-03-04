#include "BMPProcessor.h"
#include <iostream>

int main() {
    std::string inputFile;
    std::cout << "Enter input BMP file name: ";
    std::getline(std::cin, inputFile);

    try {
        BMPProcessor bmp(inputFile);
        bmp.processAndSave();
        std::cout << "File is ready!";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}