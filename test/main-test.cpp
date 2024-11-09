#include "../qoif_cpp.h"
#include "../external/ppm_reader.h"
#include <vector>
#include <cstdint>


int main(){

    /*
        PPM (P3) is very basic image format.
        Pixels values are parsed from ppm,
        but you can extract pixel values from any format.
    */
    PPM ppmImage("images/sample.ppm");    

    std::vector<Pixel> pixels = ppmImage.getPixels();
    int width = ppmImage.getSize()[0];
    int height = ppmImage.getSize()[1];
    
    QoiHeader qoiHeader;
    qoiHeader.width = width;
    qoiHeader.height = height;
    qoiHeader.channels = 3;
    qoiHeader.colorspace = 1;

    std::vector<std::uint8_t> qoifEncondings = QOIF::encode(qoiHeader, pixels);
    QOIF::saveQOIF(qoifEncondings, "sample.qoi");

    return 0;
}