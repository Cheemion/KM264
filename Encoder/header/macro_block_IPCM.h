#pragma once
#include "common.h"
#include "macro_block.h"
class MacroBlockIPCM : public MacroBlock {
private:
    //ue(v)
    byte mb_type = 25;
    //这之后的数据得8位对其
    byte luma[256];
    byte cb[64];
    byte cr[64];
public:
    static MacroBlockIPCM generateMacroBlockIPCM(byte* luma, byte* cb, byte* cr) {
        MacroBlockIPCM mb;
        for(int i = 0; i < 256; i++)
            mb.luma[i]  = luma[i];
        for(int i = 0; i < 64; i++) {
            mb.cb[i] = cb[i];
            mb.cr[i] = cr[i];
        }
        return mb;
    }
    virtual void getNextSODB(ByteString& byteString){
       byteString.ue(mb_type);
        while(!byteString.isAligned()) {
            byteString.zeroBit();
        }
        for(int i = 0; i < 256; i++) {
            byteString.u(luma[i], 8);
        }
        for(int i = 0; i < 64; i++) {
            byteString.u(cb[i], 8);
        }
        for(int i = 0; i < 64; i++) {
            byteString.u(cr[i], 8);
        }
	}
};