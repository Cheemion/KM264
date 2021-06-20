#pragma once
#include "common.h"
class MacroBlock {
public:
    virtual void getNextSODB(ByteString& byteString) {
        std::cout << "macroblock layer" << std::endl;
	}
};