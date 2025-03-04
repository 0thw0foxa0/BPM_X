#ifndef BMP_PROCESSOR_H
#define BMP_PROCESSOR_H

#include <vector>
#include <string>
#include <windows.h>

class BMPProcessor {
private:
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    std::vector<unsigned char> pixels;
    int width, height, bytesPerPixel;

    void readBMP(const std::string& filename);
    void display() const;
    void drawLine(int x1, int y1, int x2, int y2);
    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void saveBMP(const std::string& filename) const;

public:
    BMPProcessor(const std::string& inputFile);
    void processAndSave();
    ~BMPProcessor();
};

#endif // BMP_PROCESSOR_H