/**
 *  Usage: Common tools
	Date: 2021/5/16
	email: 376301008@qq.com
	Author: Kim
 * **/
#pragma once
#include <fstream>
#define emulation_preverntion_three_byte 0x03
enum NAL_UNIT_TYPE {
	UNSPECIFIED = 0,
	CODEC_SLICE_OF_AN_IDR_PICTURE_SLICE_LAYER_WITHOUT_PARTITIONING_RBSP = 5,
	SEQUENCE_PARAMETER_SET = 7,
	PICTURE_PARAMETER_SET = 8,
	ACCESS_UNIT_DELIMITER = 9
};
enum NAL_REF_IDC {
	NAL_REF_IDC_NON_REFERENCE = 0,
	NAL_REF_IDC_REFERENCE = 1
};
using byte = unsigned char;
using int16 = unsigned short;
using int32 = unsigned int; 
//write a 4-byte integer in little-endian
void putByte(std::ofstream& outFile, const byte v) {
    outFile.put(v);
}

void putInt32(std::ofstream& outFile, const int32 v) {
	outFile.put((v >> 24) & 0xFF);
	outFile.put((v >> 16) & 0xFF);
	outFile.put((v >> 8) & 0xFF);
	outFile.put((v >> 0) & 0xFF);
}

void putInt16(std::ofstream& outFile, const int16 v) {
	outFile.put((v >> 8) & 0xFF);
	outFile.put((v >> 0) & 0xFF);
}

