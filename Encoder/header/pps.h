#pragma once
#include "common.h"
#include "nal_unit.h"
class PPS : public NalUnit {
private:
	byte pic_parameter_set_id;                      //[0 255] ue(v);
	byte seq_parameter_set_id;                      //[0 31] ue(v);
	byte entropy_coding_mode_flag;                  //u(1) entropy_coding_mode_flag is equal to 0, (Exp-Golomb coded, see subclause 9.1 or CAVLC. 1 CABAC
	/*
	// u(1) equal to 1 specifies poc 相关的参数是否在slice header出现, 还是使用默认的参数.
	called bottom_field_pic_order_in_frame_present_flag in later version of h.264 recommandation
	*/
	byte pic_order_present_flag = 0;              
	byte num_slice_groups_minus1 = 0;               // ue(v) plus 1 specifies the number of slice groups for a picture
	byte num_ref_idx_l0_active_minus1; 			    // [0 31] ue(v) specifies the maximum reference index for reference picture list 0
	byte num_ref_idx_l1_active_minus1;  // ue(v) same as above
	byte weighted_pred_flag = 0; //u(1) equal to 0 specifies that weighted prediction shall not be applied to P and SP slices
	byte weighted_bipred_idc = 0; // u(2) equal to 0 specifies that the default weighted prediction shall be applied to B slices.
	int pic_init_qp_minus26 = 0; // se(v) [-(26 + (QpBdOffsetY = 0))  +25] pecifies the initial value minus 26 of SliceQPY for each slice.
	int pic_init_qs_minus26 = 0; // se(v)  [-26 25] specifies the initial value minus 26 of SliceQSY for all macroblocks in SP or SI slices.
	int chroma_qp_index_offset = 0; //se(v) [-12 12] specifies the offset that shall be added to QPY and QSY for addressing the table of QPC values for the Cb chroma component
	byte deblocking_filter_control_present_flag = 0; //u(1) equal to 1 specifies that a set of syntax elements controlling the characteristics of the deblocking filter is present in the slice header
	byte constrained_intra_pred_flag = 0; //u(1) 在intra模式下，能否使用通过inter预测的MB 作为使用.  0代表可以,1代表有constrained
	byte redundant_pic_cnt_present_flag = 0; //u(1)  equal to 0 specifies that the redundant_pic_cnt syntax element is not present in slice headers, data partitions B, and data partitions C that refer (either directly or by association with a corresponding data partition A) to the picture parameter set.
private:
    bool isInitialzed = false;
	ByteString byteString;
	PPS(){};
public:
	static PPS generateBaselineMode(byte seq_parameter_set_id, byte num_ref_idx_l0_active_minus1 = 10, byte num_ref_idx_l1_active_minus1 = 10) {
		static byte id = 0;
		PPS res;
		res.pic_parameter_set_id = id;
		id = (id + 1) % 256;
		res.seq_parameter_set_id = seq_parameter_set_id;
		res.entropy_coding_mode_flag = ENTROPY_CODING_MODE_FLAG::CAVLC;
		res.num_ref_idx_l0_active_minus1 = num_ref_idx_l0_active_minus1;
		res.num_ref_idx_l1_active_minus1 = num_ref_idx_l1_active_minus1;
		return res;
	}
	virtual	int getNextSODB(byte& holder) {
		if(!isInitialzed) {
			isInitialzed = true;
			byteString.ue(pic_parameter_set_id);
			byteString.ue(seq_parameter_set_id);
			byteString.u(entropy_coding_mode_flag, 1);
			byteString.u(pic_order_present_flag, 1);
			byteString.ue(num_slice_groups_minus1);
			byteString.ue(num_ref_idx_l0_active_minus1);
			byteString.ue(num_ref_idx_l1_active_minus1);
			byteString.u(weighted_pred_flag, 1);
			byteString.u(weighted_pred_flag, 2);
			byteString.se(pic_init_qp_minus26);
			byteString.se(pic_init_qs_minus26);
			byteString.se(chroma_qp_index_offset);
			byteString.u(deblocking_filter_control_present_flag, 1);
			byteString.u(constrained_intra_pred_flag, 1);
			byteString.u(redundant_pic_cnt_present_flag, 1);
		}
		return byteString.getNextByte(holder);
	}
	byte getID() {
		return pic_parameter_set_id;
	}
};
