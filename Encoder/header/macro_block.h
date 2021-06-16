#pragma once
#include "common.h"
class MarcoBlockIPCM {
    //ue(v)
    byte mb_type = 25;
    //这之后的数据得8位对其
    byte luma[256];
    byte cb[64];
    byte cr[64];
};