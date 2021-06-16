#pragma once
#include "nal_unit.h"
#include "common.h"
#include "macro_block.h"
class DecRefPicMarking {
    /*
    if nal_unit_type == 5, IDR frame
    */
    // u(1)
    byte no_output_of_prior_pics_flag = 0;
    // u(1)
    byte long_term_reference_flag = 0;  
    /* else
        adaptive_ref_pic_marking_mode_flag = 0; 
        Sliding window reference picture marking mode: A marking mode 
        providing a first-in first-out mechanism for short-term reference 
        pictures.  
        = 1
        Adaptive reference picture marking mode: A reference picture 
        marking mode providing syntax elements to specify marking of 
        reference pictures as “unused for reference” and to assign long-term 
        frame indices. 
     */
    // u(1)
    byte adaptive_ref_pic_marking_mode_flag = 0;
};
class SliceData {
    //不是I和SI 
        //cavlc的话
    byte mb_skip_run = 0;
    

};
class RefPicListReordering {
public:
    /*
    if( slice_type != I && slice_type != SI ) 
    u(1)
    */
    // 不是I 或者 SI的话， 就可能管理list0
    byte ref_pic_list_reordering_flag_l0 = 0;

    /*
    if( slice_type = = B )
    u(1)
    */
    //B 帧可能需要管理list1
    byte ref_pic_list_reordering_flag_l1 = 0;
};
class SliceHeader {
private:
    /*  ue(v)
        When arbitrary slice order is not allowed as 
        specified in Annex A, the value of first_mb_in_slice shall not be less than the value of first_mb_in_slice for any other 
        slice of the current picture that precedes the current slice in decoding order.
        当MbaffFrameFlag为0时，first_mb_in_slice就是该Slice中
        第一个宏块的地址，并且first_mb_in_slice的取值范围为[0, PicSizeInMbs-1]
        如果MbaffFrame = 1,  first_mb_in_slice * 2 is the macroblock address of the first macroblock 
        in the slice, which is the top macroblock of the first macroblock pair in the slice, and first_mb_in_slice shall be in 
        the range of [0, PicSizeInMbs / 2 - 1].
    */
    uint32 first_mb_in_slice; 
    /*  ue(v)
        0 P (P slice) 
        1 B (B slice) 
        2 I (I slice) 
        3 SP (SP slice) 
        4 SI (SI slice) 
        5 P (P slice) 
        6 B (B slice) 
        7 I (I slice) 
        8 SP (SP slice) 
        9 SI (SI slice)
    */
    byte slice_type;   
    /*  ue(v) [0 255] */
    byte pic_parameter_set_id;
    /* u(v) shall be represented by log2_max_frame_num_minus4 + 4 bits
        1.If the current picture is an IDR picture, frame_num shall be equal to 0.
        2.gaps_in_frames not allowed
            PrevRefFrameNum is set equal to the value of frame_num for the previous access unit in decoding order that contained a reference picture. 
            并且当
            the current picture and the preceding reference picture belong to consecutive access units in decoding order
        
        also known as decoding order
    */
    uint32 frame_num;
    /*
    if nal_unit_type = 5; 
    ue(v)
    idr_pic_id identifies an IDR picture. The values of idr_pic_id in all the slices of an IDR picture shall remain 
    unchanged. When two consecutive access units in decoding order are both IDR access units, the value of idr_pic_id in 
    the slices of the first such IDR access unit shall differ from the idr_pic_id in the second such IDR access unit. The value 
    of idr_pic_id shall be in the range of 0 to 65535, inclusive.
    */
    byte idr_pic_id;
    /*
    if pic_order_cnt_type == 0
    u(v)
    specifies the picture order count modulo MaxPicOrderCntLsb for the top field of a coded frame or 
    for a coded field. The size of the pic_order_cnt_lsb syntax element is log2_max_pic_order_cnt_lsb_minus4 + 4 bits. 
    The value of the pic_order_cnt_lsb shall be in the range of 0 to MaxPicOrderCntLsb – 1, inclusive.
    MaxPicOrderCntLsb = 2( log2_max_pic_order_cnt_lsb_minus4 + 4 )
    */
    uint16 pic_order_cnt_lsb;


    /*
        if( slice_type = = B )direct_spatial_mv_pred_flag
        u(1)
        direct_spatial_mv_pred_flag specifies the method used in the decoding process to derive motion vectors and reference 
        indices for inter prediction as follows. 
        – If direct_spatial_mv_pred_flag is equal to 1, the derivation process for luma motion vectors for B_Skip, 
        B_Direct_16x16, and B_Direct_8x8 in subclause 8.4.1.2 shall use spatial direct mode prediction as specified in 
        subclause 8.4.1.2.2. 
        – Otherwise (direct_spatial_mv_pred_flag is equal to 0), the derivation process for luma motion vectors for B_Skip, 
        B_Direct_16x16, and B_Direct_8x8 in subclause 8.4.1.2 shall use temporal direct mode prediction as specified in 
        subclause 8.4.1.2.3.
    */
    //B slice 才有的字段
    byte direct_spatial_mv_pred_flag;

    /*
    u(1)
    num_ref_idx_active_override_flag equal to 0 specifies that the values of the syntax elements 
    num_ref_idx_l0_active_minus1 and num_ref_idx_l1_active_minus1 specified in the referred picture parameter set are 
    in effect. num_ref_idx_active_override_flag equal to 1 specifies that the num_ref_idx_l0_active_minus1 and 
    num_ref_idx_l1_active_minus1 specified in the referred picture parameter set are overridden for the current slice (and 
    only for the current slice) by the following values in the slice header.
    */
   // p or sp or B 才有
    byte num_ref_idx_active_override_flag = 0;

    //一些管理list的参数在里面
    RefPicListReordering ref_pic_list_reordering;


    /**
     *  if( nal_ref_idc != 0 )
     * */
    DecRefPicMarking dec_ref_pic_marking; 

    /*
    se(v)
    slice_qp_delta specifies the initial value of QPY to be used for all the macroblocks in the slice until modified by the 
    value of mb_qp_delta in the macroblock layer. The initial QPY quantisation parameter for the slice is computed as: 
    SliceQPY = 25 + pic_init_qp_minus26 + slice_qp_delta (7-27) 
    The value of slice_qp_delta shall be limited such that SliceQPY is in the range of -QpBdOffsetY to +50, inclusive.
    */
   int slice_qp_delta = -1;

};
class SliceWithoutPartition : public NalUnit {
private:
    SliceHeader sliceHeader;
    SliceData sliceData;
public:
	virtual	int getNextSODB(byte& holder) { //0 means end
		std::cout << "the return value tells how many bit in the holder, from right to left" << std::endl;
		std::cout << "the holder gives the byte information" << std::endl;
		return -1;
	}
};