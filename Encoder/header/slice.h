#pragma once
#include "nal_unit.h"
#include "common.h"
class SliceData {
 
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
            
    */
    byte frame_num;
    /*

    */
    byte idr_pic_id;
    // if pic_order_cnt_type == 0
    byte pic_order_cnt_lsb;// u(v) pic_order_cnt_lsb specifies the picture order count modulo MaxPicOrderCntLsb for the top field of a coded frame or for a coded field. The size of the pic_order_cnt_lsb syntax element is log2_max_pic_order_cnt_lsb_minus4 + 4 bits.  The value of the pic_order_cnt_lsb shall be in the range of 0 to MaxPicOrderCntLsb – 1, inclusive.

    
};
class Slice : public NalUnit {
private:
    SliceHeader sliceHeader;
    SliceData sliceData;
public:
    
};