#pragma once
#include "common.h"
#include "nal_unit.h"
class SPS : public NalUnit {
public:
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
	static SPS generateBaselineMode(byte log2_max_frame_num_minus4 = 0, byte log2_max_pic_order_cnt_lsb_minus4 = 0, byte num_ref_frames = 10) {
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
			byteString.ue(pic_width_in_mbs_minus1);
			byteString.ue(pic_height_in_map_units_minus1);
			byteString.u(frame_mbs_only_flag, 1);
			byteString.u(direct_8x8_inference_flag, 1);
			byteString.u(frame_cropping_flag, 1);
			byteString.u(vui_parameters_present_flag, 1);
		}
		return byteString.getNextByte(holder);
	}
};
