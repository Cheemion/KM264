#pragma once
#include <list>
#include "common.h"
uint32 TwoPowerOf(int a) {
    if(a == 0)
        return 1;
    else
        return 1 << a;
}
int abs(int a) {
    if(a > 0) return a;
    else return -a;
}
class ByteString {
private:
    std::list<byte> data;
    int _position;
    byte buffer;
    int data_index;
    void writeBit(byte value) {
        buffer = (buffer << 1) | (value & 0x01);
        _position = _position + 1;
        if(_position == 8) {
            data.push_back(buffer);
            _position = 0;
            buffer = 0;
        }
    }
    void write(byte value, int bit_num) {
        if(bit_num == 0) return;
        if(bit_num > 8 || bit_num < 0) throw "bit_num > 8 or bit_num < 0";
        if(_position == 0 && bit_num == 8) {
            data.push_back(value);
        } else {
            byte mask = 1 << (bit_num - 1);
            for(int i = 0; i < bit_num; i++) {
                byte temp = (value & mask) >> (bit_num - i - 1);
                mask = mask >> 1;
                writeBit(temp);
            }
        }
    }
    void writeUint32(uint32 v, int num_bit) {
        if(num_bit == 0) return;
        if(num_bit > 33 || num_bit < 0) throw "bit_num > 33 or bit_num < 0";
        uint32 mask = 0xff;
        while(num_bit > 0) {
            write(mask & v, num_bit > 8 ? 8 : num_bit);
            v = v >> 8;
            num_bit = num_bit - 8;
        }
    }
public:
    ByteString(): _position(0), buffer(0), data_index(0){}
    void u(uint32 v, int num_bit) {
        writeUint32(v, num_bit);
    }
    void ue(uint32 v) {
        int leadingZeros = 0;
        while(TwoPowerOf(leadingZeros) <= (v + 1))
            ++leadingZeros;
        leadingZeros = leadingZeros - 1;
        for(int i = 0; i < leadingZeros; i++) {
            writeBit(0x00);
        }
        writeBit(0x01);
        v = v + 1 - TwoPowerOf(leadingZeros);
        writeUint32(v, leadingZeros);
    }
    void b8(byte v){
        write(v, 8);
    }
    void ae(byte v) {

    }
    void ce(byte v) {

    }
    void f(byte v){

    }
    void i(int v) {

    }
    void se(int v) {
        int absValue = abs(v);
        uint32 codeNum = absValue * 2;
        if(v > 0)
            codeNum = codeNum - 1;
        ue(codeNum);
    }
    void me(byte v) {

    }
    void te(byte v) {

    }
    int getNextByte(byte& holder) {
        if(data.empty()) {
            if(_position == 0) {
                holder = 0;
                return 0;
            } else {
                int res = _position;
                holder = buffer;
                _position = 0;
                buffer = 0;
                return res;
            }
        } else {
            holder = data.front();
            data.pop_front();
            return 8;
        }
    }
};