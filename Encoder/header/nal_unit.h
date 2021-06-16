/**
 * Usage: write NAL units to a file
 * Example: 
 * int main() {
	std::ofstream outfile("test.txt", std::fstream::out | std::fstream::binary);
	SPS sps1;
	SPS sps2;
	NalContainer nal1(NAL_REF_IDC_REFERENCE, SEQUENCE_PARAMETER_SET, &sps1);
	NalContainer nal2(NAL_REF_IDC_REFERENCE, PICTURE_PARAMETER_SET, &sps2);
	writeNALSequences({nal1, nal2}, outfile);
	outfile.close();
	std::cout << CONFIGURE::INPUT_FILE << std::endl;
	return 0;
}
	Date: 2021/5/16
	email: 376301008@qq.com
	Author: Kim
 * **/
#pragma once
#include "configuration.h"
#include "common.h"
#include <iostream>
#include <fstream>
#include "byte_writer.h"
#include <vector>
#include "byte_string.h"
struct NalHeader {
	byte forbidden_zero_bit = 0; //f(1) shall be equal to 0.
	byte nal_ref_idc; //u(2)  not equal to 0 specifies that the content of the NAL unit contains a sequence parameter set or a picture parameter set or a slice of a reference picture or a slice data partition of a reference picture.
	byte nal_unit_type; //u(5) 
};

class NalUnit {
public:
	virtual	int getNextSODB(byte& holder) { //0 means end
		std::cout << "the return value tells how many bit in the holder, from right to left" << std::endl;
		std::cout << "the holder gives the byte information" << std::endl;
		return -1;
	}
};

class NalContainer {
public:
	NalHeader header;
	NalUnit* nal_unit_p;
public:
	NalContainer(byte nal_ref_idc, byte nal_unit_type, NalUnit* nal_unit_p) {
		this->header.nal_ref_idc = nal_ref_idc;
		this->header.nal_unit_type = nal_unit_type;
		this->nal_unit_p = nal_unit_p;
	}
	//每个Nal unit都必须保证写完1个byte
	// 做了emulation prevention 处理
	void writeNAL(std::ofstream& os) {
		//NALheader 如果是0x00的话 不知道会不会对解码过程有影响。得看看nal里面的数据喝nal header 会不会构成一个 start_code
		//我这边先是没有管，后面写其他东西的时候需要关注一下
		ByteWriter bw(os);
		bw.write(header.forbidden_zero_bit, 1);
		bw.write(header.nal_ref_idc, 2);
		bw.write(header.nal_unit_type,5);

		byte b0 = emulation_preverntion_three_byte;
		byte b1 = emulation_preverntion_three_byte;
		byte b2 = emulation_preverntion_three_byte;
		int bit_num;
		//write integral 8 bits
		while((bit_num = nal_unit_p->getNextSODB(b2)) == 8) {
			if(b0 == 0x00 && b1 == 0x00 && (b2 == 0x00 || b2 == 0x01 || b2 == 0x02 || b2 == 0x03)) {
				bw.write(emulation_preverntion_three_byte, 8);
				bw.write(b2,8);
				b0 = emulation_preverntion_three_byte;
				b1 = b2;
			} else {
				bw.write(b2,8);
				b0 = b1;
				b1 = b2;
			}
		}
		//没有考虑 cabac_zero_word
		b2 = (b2 << (8 - bit_num)) + (1 << (8 - bit_num - 1)); // rbsp_stop_one_bit and rbsp_trailing_bits;
		if(b0 == 0x00 && b1 == 0x00 && (b2 == 0x00 || b2 == 0x01 || b2 == 0x02 || b2 == 0x03)) {
			bw.write(emulation_preverntion_three_byte, 8);
			bw.write(b2,8);
		} else {
			bw.write(b2,8);
		}
	}
};


void writeNALSequences(std::vector<NalContainer> nals, std::ofstream& os) {
	if(CONFIGURE::OUTPUT_FILE_MODE != OUTPUT_MODE::ANNEX_B)
		throw "only support AnnexB for the moment";

	for(auto first = nals.begin(); first != nals.cend(); first++) {
		if(first->header.nal_unit_type == SEQUENCE_PARAMETER_SET || first->header.nal_unit_type == PICTURE_PARAMETER_SET || first->header.nal_unit_type == CODEC_SLICE_OF_AN_IDR_PICTURE_SLICE_LAYER_WITHOUT_PARTITIONING_RBSP || first->header.nal_unit_type == ACCESS_UNIT_DELIMITER) {
			putByte(os, 0x00);
			putByte(os, 0x00);
			putByte(os, 0x00);
			putByte(os, 0x01);
		} else {
			putByte(os, 0x00);
			putByte(os, 0x00);
			putByte(os, 0x01);
		}
		first->writeNAL(os);
	}
}