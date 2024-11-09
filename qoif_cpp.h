#ifndef GOIF_CPP_H
#define GOIF_CPP_H

/*
    QOIF specificaiton: https://qoiformat.org/qoi-specification.pdf
*/

#include <cstdint>
#include <vector>
#include <assert.h>
#include <string>
#include <fstream>
#include <iostream>

struct Pixel
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;
    Pixel(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 255): r(red), g(green), b(blue), a(alpha) {}
    Pixel(): r(0), g(0), b(0), a(255) {}
};

struct QoiHeader
{
    char magic[4] = {'q', 'o', 'i', 'f'}; // magic bytes "qoif"
    uint32_t width; // image width in pixels (BE)
    uint32_t height; // image height in pixels (BE)
    uint8_t channels; // 3 = RGB, 4 = RGBA
    uint8_t colorspace; // 0 = sRGB with linear alpha // 1 = all channels linear
};

class QOIF
{
    private:
    public:
        static bool check_QOI_OP_RUN(Pixel pixel, Pixel lastPixel, std::uint8_t run);
        static bool check_QOI_OP_INDEX(Pixel pixel, Pixel* array);
        static bool check_QOI_OP_DIFF(Pixel pixel, Pixel lastPixel);
        static bool check_QOI_OP_LUMA(Pixel pixel, Pixel lastPixel);
        
        static uint8_t QOI_OP_RUN(std::uint8_t run);
        static uint8_t QOI_OP_INDEX(Pixel pixel);
        static uint8_t QOI_OP_DIFF(Pixel pixel, Pixel lastPixel);
        static uint8_t* QOI_OP_LUMA(Pixel pixel, Pixel lastPixel);
        static uint8_t* QOI_OP_RGB(Pixel pixel);
        static uint8_t* QOI_OP_RGBA(Pixel pixel);
        static void savePixelToArray(Pixel pixel, Pixel* array);
        static void uint32_to_big_endian(uint32_t value, uint8_t *dest);

        static std::vector<std::uint8_t> encode(QoiHeader qoiHeader, std::vector<Pixel> pixels);
        static void saveQOIF(std::vector<std::uint8_t>& qoifEncodings, std::string fileName);

        //static QoiHeader decodeHeader(std::vector<std::uint8_t> qoifEncodings);
        //static std::vector<Pixel> decodePixels(std::vector<std::uint8_t> qoifEncodings);
};

void QOIF::uint32_to_big_endian(uint32_t value, uint8_t *dest) {
    dest[0] = (value >> 24) & 0xFF;
    dest[1] = (value >> 16) & 0xFF;
    dest[2] = (value >> 8) & 0xFF;
    dest[3] = value & 0xFF;
}

std::string printBits(uint8_t num) {
    std::string s = "";
    for (int i = 7; i >= 0; i--) {  // Loop over the 8 bits
        s += std::to_string((num >> i) & 1);
    }
    return s;
}

bool QOIF::check_QOI_OP_RUN(Pixel pixel, Pixel lastPixel, std::uint8_t run){
    if (run > 62)
    {
        return false;
    }
    
    return ( (pixel.r == lastPixel.r) && (pixel.g == lastPixel.g) && (pixel.b == lastPixel.b) && (pixel.a == lastPixel.a) );
}

uint8_t QOIF::QOI_OP_RUN(std::uint8_t run){
    return uint8_t(128 + 64 + run);
}

bool QOIF::check_QOI_OP_INDEX(Pixel pixel, Pixel* array){
    int index = ( int(pixel.r) * 3 + int(pixel.g) * 5 + int(pixel.b) * 7 + int(pixel.a) * 11) % 64;
    Pixel pixelAtIndex = array[index];
    if ((pixel.r == pixelAtIndex.r) && (pixel.g == pixelAtIndex.g) && (pixel.b == pixelAtIndex.b) && (pixel.a == pixelAtIndex.a))
    {
        return true;
    }
    return false;
}

uint8_t QOIF::QOI_OP_INDEX(Pixel pixel){
    std::uint8_t index = ( int(pixel.r) * 3 + int(pixel.g) * 5 + int(pixel.b) * 7 + int(pixel.a) * 11) % 64;
    return index;
}

bool QOIF::check_QOI_OP_DIFF(Pixel pixel, Pixel lastPixel){
    
    int diffs[3] = { pixel.r - lastPixel.r, pixel.g - lastPixel.g, (pixel.b - lastPixel.b) };

    if (pixel.a - lastPixel.a != 0)
    {
        return false;
    }

    for (int i = 0; i<3; i++) {
        diffs[i] = diffs[i] > +127 ? diffs[i] - 256 : diffs[i];
        diffs[i] = diffs[i] < -127 ? diffs[i] + 256 : diffs[i];
    }

    return (-2 <= diffs[0] && diffs[0] <= 1) && (-2 <= diffs[1] && diffs[1] <= 1) && (-2 <= diffs[2] && diffs[2] <= 1);
}

uint8_t QOIF::QOI_OP_DIFF(Pixel pixel, Pixel lastPixel){
    int diffs[3] = { pixel.r - lastPixel.r, pixel.g - lastPixel.g, pixel.b - lastPixel.b };

    for (int i = 0; i<3; i++) {
        diffs[i] = diffs[i] > +127 ? diffs[i] - 256 : diffs[i];
        diffs[i] = diffs[i] < -127 ? diffs[i] + 256 : diffs[i];
    }

    uint8_t res = 64 + 16*(diffs[2] + 2) + 4*(diffs[1] + 2) + (diffs[0] + 2);
    return res;
}
        
bool QOIF::check_QOI_OP_LUMA(Pixel pixel, Pixel lastPixel){
    
    int diffs[3] = { int(pixel.r) - int(lastPixel.r), int(pixel.g) - int(lastPixel.g), int(pixel.b) - int(lastPixel.b) };

    if (pixel.a - lastPixel.a != 0)
    {
        return false;
    }
    
    for (int i = 0; i<3; i++) {
        diffs[i] = diffs[i] > +127 ? diffs[i] - 256 : diffs[i];
        diffs[i] = diffs[i] < -127 ? diffs[i] + 256 : diffs[i];
    }
    
    bool valid = true;

    int dr_dg = diffs[0] - diffs[1];
    if (!( dr_dg <= 7 && dr_dg >= -8))
    {
        valid = false;
    }

    if (!(diffs[1] <= 31 && diffs[1] >= -32))
    {
        valid = false;
    }

    int db_dg = diffs[2] - diffs[1];
    if (!(db_dg <= 7 && db_dg >= -8))
    {
        valid = false;
    }

    return valid;
}

uint8_t* QOIF::QOI_OP_LUMA(Pixel pixel, Pixel lastPixel){
    int diffs[3] = { int(pixel.r) - int(lastPixel.r), int(pixel.g) - int(lastPixel.g), int(pixel.b) - int(lastPixel.b) };

    
    for (int i = 0; i<3; i++) {
        diffs[i] = diffs[i] > +127 ? diffs[i] - 256 : diffs[i];
        diffs[i] = diffs[i] < -127 ? diffs[i] + 256 : diffs[i];
    }
    
    uint8_t firstByte = 128 + ( diffs[1] + 32);
    uint8_t secondByte = 16 * (diffs[0] - diffs[1] + 8) + (diffs[2] - diffs[1] + 8);
    uint8_t* bytes = new std::uint8_t[2] {firstByte, secondByte};
    return bytes;
}

uint8_t* QOIF::QOI_OP_RGB(Pixel pixel){
    std::uint8_t byte0 = 0xfe;
    std::uint8_t r = pixel.r;
    std::uint8_t g = pixel.g;
    std::uint8_t b = pixel.b;

    std::uint8_t* bytes = new std::uint8_t[4] {byte0, r, g, b};
    return bytes;
}

uint8_t* QOIF::QOI_OP_RGBA(Pixel pixel){
    std::uint8_t byte0 = 0xfe;
    std::uint8_t r = pixel.r;
    std::uint8_t g = pixel.g;
    std::uint8_t b = pixel.b;
    std::uint8_t a = pixel.a;

    std::uint8_t* bytes = new std::uint8_t[5] {byte0, r, g, b, a};
    return bytes;
}

void QOIF::savePixelToArray(Pixel pixel, Pixel* array){
    int index = ( int(pixel.r) * 3 + int(pixel.g) * 5 + int(pixel.b) * 7 + int(pixel.a) * 11) % 64;
    array[index] = pixel;
}

std::vector<std::uint8_t> QOIF::encode(QoiHeader qoiHeader, std::vector<Pixel> pixels){
    
    std::vector<std::uint8_t> encodings;

    //magic
    for (int i = 0; i < 4; i++) {
        encodings.push_back(qoiHeader.magic[i]);
    }

    uint8_t widthBytes[4];
    uint32_to_big_endian(qoiHeader.width, widthBytes);

    for (int i = 0; i < 4; i++) {
        encodings.push_back(widthBytes[i]);
    }

    uint8_t heightBytes[4];
    uint32_to_big_endian(qoiHeader.height, heightBytes);

    for (int i = 0; i < 4; i++) {
        encodings.push_back(heightBytes[i]);
    }
    
    encodings.push_back(qoiHeader.channels);
    encodings.push_back(qoiHeader.colorspace);

    assert(qoiHeader.channels == 3 || qoiHeader.channels == 4);

    Pixel previousPixel;
    Pixel array[64];

    for (int i=0; i<64; i++) {
        array[i] = Pixel(0, 0, 0);
    }

    std::uint8_t run = 0;

    std::size_t imageSize = qoiHeader.height * qoiHeader.width;

    for (std::size_t i = 0; i < imageSize; i++)
    {
        Pixel pixel = pixels[i];
        
        if(check_QOI_OP_RUN(pixel, previousPixel, run))
        {
            run++;
        }
        else
        {
        
            
            if (run != 0)
            {
                std::uint8_t b = QOI_OP_RUN(run);
                encodings.push_back(b);
                run = 0;
            }
            
            if(check_QOI_OP_INDEX(pixel, array)){
                std::uint8_t b = QOI_OP_INDEX(pixel);
                encodings.push_back(b);
            }
            
            else if(check_QOI_OP_DIFF(pixel, previousPixel)){
                std::uint8_t b = QOI_OP_DIFF(pixel, previousPixel);
                encodings.push_back(b);
                savePixelToArray(pixel, array);
            }
            
            else if(check_QOI_OP_LUMA(pixel, previousPixel)){
                std::uint8_t* bs = QOI_OP_LUMA(pixel, previousPixel);
                for (int i = 0; i < 2; i++)
                {
                    encodings.push_back(bs[i]);
                }
                delete[] bs;
                savePixelToArray(pixel, array);
            }
            
            else
            
            {
                
                if (pixel.a == previousPixel.a)
                {
                    std::uint8_t* bs = QOI_OP_RGB(pixel);
                    for (int i = 0; i < 4; i++)
                    {
                        encodings.push_back(bs[i]);
                    }
                    delete[] bs;
                    savePixelToArray(pixel, array);
                }
                else
                
                {
                    std::uint8_t* bs = QOI_OP_RGBA(pixel);
                    for (int i = 0; i < 5; i++)
                    {
                        encodings.push_back(bs[i]);
                    }
                    delete[] bs;
                    savePixelToArray(pixel, array);
                }
            }
            
        }
        
        previousPixel = pixel;       
    }
    encodings.insert(encodings.end(), {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01});
    return encodings;
}

void QOIF::saveQOIF(std::vector<std::uint8_t>& qoifEncodings, std::string fileName){
    std::ofstream file(fileName, std::ios::binary);
    
    // Check if the file is open
    if (file.is_open()) {
        // Write the vector data to the file
        file.write(reinterpret_cast<const char*>(qoifEncodings.data()), qoifEncodings.size());
        
        // Close the file
        file.close();
    } else {
        // Handle error if file can't be opened
        std::cerr << "Failed to open file: " << fileName << std::endl;
    }
}

#endif