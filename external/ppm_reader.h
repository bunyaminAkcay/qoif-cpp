#include "../qoif_cpp.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdint>

class PPM
{
    private:
        std::vector<Pixel> pixels;
        int size[2];
        int maxColor;
    public:
        PPM(std::string fileName);
        ~PPM();

        std::vector<Pixel> getPixels();
        int* getSize();
};

PPM::PPM(std::string fileName)
{

    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Error opening file.\n";
        exit(-1);
    }
    
    std::string format;
    
    file >> format >> size[0] >> size[1] >> maxColor;
    
    if (format != "P3") {
        std::cerr << "Unsupported PPM format.\n";
        file.close();
        exit(-1);
    }

    for (int i = 0; i < size[0] * size[1]; ++i) {
        std::string r, g, b;
        file >> r >> g >> b;
        Pixel p;
        p.r = std::uint8_t(std::stoi(r));
        p.g = std::uint8_t(std::stoi(g));
        p.b = std::uint8_t(std::stoi(b));
        pixels.push_back(p);
    }
    
    file.close();
}

PPM::~PPM()
{
}

std::vector<Pixel> PPM::getPixels(){
    return this->pixels;
}

int* PPM::getSize(){
    return this->size;
}