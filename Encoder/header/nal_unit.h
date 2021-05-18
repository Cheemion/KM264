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
		//8 bits
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

class SPS : public NalUnit{
private:
	byte profile_idc;    //u(8)
	byte constraint_set0_flag = 0; // u(1) equals 1 obey Baseline profile
	byte constraint_set1_flag = 0; // u(1) equals 1 obey Main
	byte constraint_set2_flag = 0; // u(1) equals 1 obey Extended
	byte constraint_set3_flag = 0; // u(1) just set to 0, its not liked to be used.
	byte reserved_zero_4bits = 0; //u(4)
	byte level_idc; // u(8) level_idc shall be set equal to a value of ten times the level number specified in Table A - 1
	byte seq_parameter_set_id; // ue(v) [0 31]
	byte log2_max_frame_num_minus4; //ue(v) [0 12] MaxFrameNum = 2^( log2_max_frame_num_minus4 + 4 )
	byte pic_order_cnt_type = 0;// ue(v) [0 2] // 这边限定poc 通过slice 来传送
	byte log2_max_pic_order_cnt_lsb_minus4; //ue(v) [0 12] MaxPicOrderCntLsb = 2 ^ (log2_max_pic_order_cnt_lsb_minus4 + 4 )
	byte num_ref_frames; // ue(v) //specifies the maximum number of short-term and long-term reference frames, complementary reference field pairs, and non-paired reference fields
				         //The value of num_ref_frames shall be in the range of 0 to MaxDpbSize
    byte gaps_in_frame_num_value_allowed_flag = 0; // u(1) If gaps_in_frame_num_value_allowed_flag is equal to 0, the value of frame_num for the current picture shall be equal to ( PrevRefFrameNum + 1 ) % MaxFrameNum.
	byte pic_width_in_mbs_minus1;  // ue(v) PicWidthInMbs = pic_width_in_mbs_minus1 + 1
	byte pic_height_in_map_units_minus1; // ue(v)  PicHeightInMapUnits = pic_height_in_map_units_minus1 + 1
										//Depending on frame_mbs_only_flag, semantics are assigned to pic_height_in_map_units_minus1 as follows. 
										//– If frame_mbs_only_flag is equal to 0, pic_height_in_map_units_minus1 plus 1 is the height of a field in units of macroblocks. 
										//– Otherwise (frame_mbs_only_flag is equal to 1), pic_height_in_map_units_minus1 plus 1 is the height of a frame in units of macroblocks.
	byte frame_mbs_only_flag = 1; //u(1) when equals to 1 specifies that every coded picture of the coded video sequence is a coded frame containing only frame macroblocks. 
	byte direct_8x8_inference_flag = 1;//u(1) When frame_mbs_only_flag is equal to 0, direct_8x8_inference_flag shall be equal to 1.
	byte frame_cropping_flag = 0; // u(1)
	byte vui_parameters_present_flag = 0; //u(1)
private:
	SPS(){};
	ByteString byteString;
	bool isInitialzed = false;
public:
	static SPS generateBaselineMode(byte log2_max_frame_num_minus4, byte log2_max_pic_order_cnt_lsb_minus4, byte num_ref_frames) {
		static byte id = 0;
		SPS res;
		res.profile_idc = PROFILE_IDC::BASELINE;
		res.level_idc = LEVEL_IDC::LEVEL3;	
		res.seq_parameter_set_id = id;
		id = (id + 1) % 32;
		res.log2_max_frame_num_minus4 = log2_max_frame_num_minus4;
		res.log2_max_pic_order_cnt_lsb_minus4 = log2_max_pic_order_cnt_lsb_minus4;
		res.num_ref_frames = num_ref_frames;
		res.pic_width_in_mbs_minus1 = (CONFIGURE::WIDTH / 16) - 1;
		res.pic_height_in_map_units_minus1 = (CONFIGURE::HEIGHT / 16) - 1;
		return res;
	}
	byte getID() {
		return seq_parameter_set_id;
	}
	virtual	int getNextSODB(byte& holder) { //0 means end
		if(!isInitialzed) {
			isInitialzed = true;
			byteString.u(profile_idc, 8);
			byteString.u(constraint_set0_flag, 1);
			byteString.u(constraint_set1_flag, 1);
			byteString.u(constraint_set2_flag, 1);
			byteString.u(constraint_set3_flag, 1);
			byteString.u(reserved_zero_4bits, 4);
			byteString.u(level_idc, 8);
			byteString.ue(seq_parameter_set_id);
			byteString.ue(log2_max_frame_num_minus4);
			byteString.ue(pic_order_cnt_type);
			byteString.ue(log2_max_pic_order_cnt_lsb_minus4);
			byteString.ue(num_ref_frames);
			byteString.u(gaps_in_frame_num_value_allowed_flag, 1);
			byteString.ue(10);
			byteString.ue(8);
			//byteString.ue(pic_width_in_mbs_minus1);
			//byteString.ue(pic_height_in_map_units_minus1);
			byteString.u(frame_mbs_only_flag, 1);
			byteString.u(direct_8x8_inference_flag, 1);
			byteString.u(frame_cropping_flag, 1);
			byteString.u(vui_parameters_present_flag, 1);
		}
		return byteString.getNextByte(holder);
	}
};

class PPS : public NalUnit {

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