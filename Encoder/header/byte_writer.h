/**
 *  Usage: write bits from. the first come bits would occupy the MSB of a byte. Additionally, the ByteWriter needs to write a integral Byte
 *  Example:
 * 	std::ofstream outfile("test.txt", std::fstream::out | std::fstream::binary);
	B bw(outfile);
	bw.write(0x1,2);
	bw.write(0x2,2);
	bw.write(0x0,3);
	bw.write(0x1,1);
	outfile.close();
	Date: 2021/5/16
	email: 376301008@qq.com
	Author: Kim
 * **/
#pragma once
#include "common.h"
#include <fstream>
class ByteWriter {
public:
    std::ofstream& outFile;
private:
    int _position;
    byte buffer;
    void writeBit(byte value) {
        buffer = (buffer << 1) | (value & 0x01);
        _position = _position + 1;
        if(_position == 8) {
            putByte(outFile, buffer);
            _position = 0;
            buffer = 0;
        }
    }
public:
    ByteWriter(std::ofstream& os): outFile(os), _position(0), buffer(0){}
    int getPosition() {
        return _position;
    }
    void align() {
        while (_position != 0) {
            writeBit(0);
        }
    }
    void write(byte value, int bit_num) {
        if(bit_num > 8 || bit_num <= 0) throw "bit_num > 8 or bit_num < 0";
        if(_position == 0 && bit_num == 8) {
            putByte(outFile, value);
        } else {
            byte mask = 1 << (bit_num - 1); 
            for(int i = 0; i < bit_num; i++) {
                byte temp = (value & mask) >> (bit_num - i - 1);
                mask = mask >> 1;
                writeBit(temp);
            }
        }
    }
};