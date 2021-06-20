#pragma once
#include "common.h"
#include "configuration.h"
class YUVReader {
private:
    int height = CONFIGURE::HEIGHT;
    int width = CONFIGURE::WIDTH;
    int index = 0;
public:
    YUVReader() = default;
    bool open(std::string& path);
};