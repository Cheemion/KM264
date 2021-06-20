#pragma once
#include "common.h"
#include "configuration.h"
class YUVReader {
private:
    int height = CONFIGURE::HEIGHT;
    int width = CONFIGURE::WIDTH;
    int index = 0;
    int frameSize;
    byte* luma;
    byte* cr;
    byte* cb;
public:
    YUVReader() = default;
    ~YUVReader();
    bool open(std::string& path);
};