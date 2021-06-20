#include "../header/yuv_reader.h"

bool YUVReader::open(std::string& path) {
    std::ifstream inFile = std::ifstream(path, std::ios::in | std::ios::binary);
    if(!inFile) {
        return false;
    }
    inFile.seekg(0, std::ios::end);
    long size = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    //frameSize = size / (height * width * ()) 
    //luma = new byte[height * width];
    cr = new byte[height * width];
    cb = new byte[height * width];
    for(int i = 0; i < height * width; i++) {
        luma[i] = inFile.get();
    }
    for(int i = 0; i < height * width / 4; i++) {
        cr[i] = inFile.get();
    }
    for(int i = 0; i < height * width / 4; i++) {
        cr[i] = inFile.get();
    }
}
YUVReader::~YUVReader(){
    if(luma) {
        delete[] luma;
        luma = nullptr;
    }

    if(cr) {
        delete[] cr;
        cr = nullptr;
    }
    if(cb) {
        delete[] cb;
        cb = nullptr;
    }
}