#pragma once
#include "nal_unit.h"
#include "common.h"
#include "macro_block.h"
#include "pps.h"
#include "sps.h"
#include <list>
class DecRefPicMarking {
public:
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
public:
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
public:
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
    byte direct_spatial_mv_pred_flag = 0;

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
    SliceQPY = 25 + pic_init_qp_minus26 + slice_qp_delta 
    The value of slice_qp_delta shall be limited such that SliceQPY is in the range of -QpBdOffsetY to +50, inclusive.
    */
   int slice_qp_delta = 0;

};
class SliceWithoutPartition : public NalUnit {
public:
    SliceHeader sliceHeader;
    SliceData sliceData;
    ByteString byteString;
    bool isInitialized = false;
    PPS* pps;
    SPS* sps;
    std::list<MacroBlock*> macroBlocks;
    int macroBlockSize;
public:
    SliceWithoutPartition(){}
    void addMacroBlock(MacroBlock* mb) {
        macroBlocks.push_back(mb);
        macroBlockSize++;
    }
    static SliceWithoutPartition generateBaselineSliceWithoutPartition(
                                                                    PPS* pps, SPS* sps, 
                                                                    SLICE_TYPE sliceType,
                                                                    uint32 frameNum,
                                                                    byte nalUnitType
                                                                    ) 
    {
        SliceWithoutPartition temp;
        temp.pps = pps;
        temp.sps = sps;
        temp.sliceHeader.first_mb_in_slice = 0; // 一个slice一帧, 所有的mb 都在里面
        temp.sliceHeader.slice_type = sliceType;
        temp.sliceHeader.pic_parameter_set_id = pps->pic_parameter_set_id;
        temp.sliceHeader.frame_num = frameNum;
        static byte idr_pic_id = 0;
        if(nalUnitType == NAL_UNIT_TYPE::CODEC_SLICE_OF_AN_IDR_PICTURE_SLICE_LAYER_WITHOUT_PARTITIONING_RBSP) {
            temp.sliceHeader.idr_pic_id = idr_pic_id++;
        }
        static uint32 pic_order_cnt_lsb = 0;
        temp.sliceHeader.pic_order_cnt_lsb = pic_order_cnt_lsb;
        pic_order_cnt_lsb = pic_order_cnt_lsb + 2;
        return temp;
    }    
	virtual	int getNextSODB(byte& holder) { //0 means end
        if(!isInitialized) {
            isInitialized = true;
            // slice header
            byteString.ue(sliceHeader.first_mb_in_slice);
            byteString.ue(sliceHeader.slice_type);
            byteString.ue(sliceHeader.pic_parameter_set_id);
            byteString.u(sliceHeader.frame_num, sps->log2_max_frame_num_minus4 + 4);
            if(nalHeader->nal_unit_type == NAL_UNIT_TYPE::CODEC_SLICE_OF_AN_IDR_PICTURE_SLICE_LAYER_WITHOUT_PARTITIONING_RBSP) {
                byteString.ue(sliceHeader.idr_pic_id);
            }
            if(sps->pic_order_cnt_type == 0) {
                byteString.u(sliceHeader.pic_order_cnt_lsb, sps->log2_max_pic_order_cnt_lsb_minus4 + 4);
            }
            if(sliceHeader.slice_type == SLICE_TYPE::B) {
               byteString.u(sliceHeader.direct_spatial_mv_pred_flag, 1);
            }
            if(sliceHeader.slice_type == SLICE_TYPE::P || sliceHeader.slice_type == SLICE_TYPE::SP || sliceHeader.slice_type == SLICE_TYPE::B) {
                byteString.u(sliceHeader.num_ref_idx_active_override_flag, 1);
            }
            // slice ref_pic_list_reordering
            if(sliceHeader.slice_type != SLICE_TYPE::I && sliceHeader.slice_type != SLICE_TYPE::SI) {
                byteString.u(sliceHeader.ref_pic_list_reordering.ref_pic_list_reordering_flag_l0, 1);
            }
            if(sliceHeader.slice_type == SLICE_TYPE::B) {
                byteString.u(sliceHeader.ref_pic_list_reordering.ref_pic_list_reordering_flag_l1, 1);
            }
            // slice dec_ref_pic_marking
            if(nalHeader->nal_ref_idc != NAL_REF_IDC::NON_REFERENCE) {
                if(nalHeader->nal_unit_type == NAL_UNIT_TYPE::CODEC_SLICE_OF_AN_IDR_PICTURE_SLICE_LAYER_WITHOUT_PARTITIONING_RBSP) {
                    byteString.u(sliceHeader.dec_ref_pic_marking.no_output_of_prior_pics_flag, 1);
                    byteString.u(sliceHeader.dec_ref_pic_marking.long_term_reference_flag, 1);
                } else {
                    byteString.u(sliceHeader.dec_ref_pic_marking.adaptive_ref_pic_marking_mode_flag, 1);
                }
            }
            byteString.se(sliceHeader.slice_qp_delta);
            //slice data
            for(auto i = macroBlocks.begin(); i != macroBlocks.end(); i++) {
                if(sliceHeader.slice_type != SLICE_TYPE::I && sliceHeader.slice_type != SLICE_TYPE::SI) {
                    if(!pps->entropy_coding_mode_flag) {
                        byteString.ue(sliceData.mb_skip_run);
                    } else {

                    }
                }
                (*i)->getNextSODB(byteString);
            }
        }
		return byteString.getNextByte(holder);
	}
};