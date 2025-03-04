#include "BMPProcessor.h"
#include <iostream>
#include <fstream>

BMPProcessor::BMPProcessor(const std::string& inputFile) {
    readBMP(inputFile);
}

BMPProcessor::~BMPProcessor() {
    // Ресурсы освобождаются автоматически
}


void BMPProcessor::readBMP(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open input file: " + filename);
    }

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
        throw std::runtime_error("Only 24-bit or 32-bit BMP supported");
    }

    width = infoHeader.biWidth;
    height = infoHeader.biHeight;
    bytesPerPixel = infoHeader.biBitCount / 8;
    rowSize = ((width * bytesPerPixel + 3) / 4) * 4; // Выравнивание до 4 байт
    pixels.resize(rowSize * height);

    file.seekg(fileHeader.bfOffBits, std::ios::beg);
    file.read(reinterpret_cast<char*>(pixels.data()), pixels.size());
    file.close();

    // Проверка цветов с ранним выходом
    for (size_t i = 0; i < pixels.size(); i += bytesPerPixel) {
        unsigned char b = pixels[i], g = pixels[i + 1], r = pixels[i + 2];
        if (!(r == 0 && g == 0 && b == 0) && !(r == 255 && g == 255 && b == 255)) {
            throw std::runtime_error("Image contains colors other than black and white");
        }
    }
}

int BMPProcessor::getPixelIndex(int x, int y) const {
    return y * rowSize + x * bytesPerPixel; // Единая функция для индекса
}

void BMPProcessor::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        int index = getPixelIndex(x, y);
        pixels[index] = b;
        pixels[index + 1] = g;
        pixels[index + 2] = r;
        if (bytesPerPixel == 4) pixels[index + 3] = 255;
    }
}

void BMPProcessor::drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    int x = x1, y = y1;
    while (true) {
        setPixel(x, y, 0, 0, 0); 
        if (x == x2 && y == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }
    }
}

void BMPProcessor::saveBMP(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open output file: " + filename);
    }

    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
    file.seekp(fileHeader.bfOffBits, std::ios::beg);
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
    file.close();
}

void BMPProcessor::processAndSave() {

    drawLine(0, 0, width - 1, height - 1);
    drawLine(width - 1, 0, 0, height - 1);

    std::string outputFile;
    std::cout << "Enter output BMP file name: ";
    std::getline(std::cin, outputFile);
    saveBMP(outputFile);
}