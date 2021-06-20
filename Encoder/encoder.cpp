#include <iostream>
#include "header/common.h"
#include "header/configuration.h"
#include "header/byte_writer.h"
#include "header/nal_unit.h"
#include "header/slice.h"
#include "header/sps.h"
#include "header/pps.h"
#include "header/macro_block.h"
#include "header/macro_block_IPCM.h"
int main() {
	std::ofstream outfile("test.264", std::fstream::out | std::fstream::binary);
	SPS sps = SPS::generateBaselineMode();
	PPS pps = PPS::generateBaselineMode(sps.getID());
	NalContainer nal1(NAL_REF_IDC::REFERENCE, NAL_UNIT_TYPE::SEQUENCE_PARAMETER_SET, &sps);
	NalContainer nal2(NAL_REF_IDC::REFERENCE, NAL_UNIT_TYPE::PICTURE_PARAMETER_SET, &pps);
	MacroBlockIPCM* mbs = new MacroBlockIPCM[CONFIGURE::HEIGHT * CONFIGURE::WIDTH / 256];
	MacroBlockIPCM* mbs1 = new MacroBlockIPCM[CONFIGURE::HEIGHT * CONFIGURE::WIDTH / 256];
	MacroBlockIPCM* mbs2 = new MacroBlockIPCM[CONFIGURE::HEIGHT * CONFIGURE::WIDTH / 256];
	for(int i = 0; i < CONFIGURE::HEIGHT * CONFIGURE::WIDTH / 256; i++) {
		for(int j = 0; j < 256; j++) {
			mbs1[i].luma[j]= 125;	
			mbs2[i].luma[j]= 188;	
		}
	}
	SliceWithoutPartition slice1 = SliceWithoutPartition::generateBaselineSliceWithoutPartition(&pps,&sps, SLICE_TYPE::I, 0, NAL_UNIT_TYPE::CODEC_SLICE_OF_AN_IDR_PICTURE_SLICE_LAYER_WITHOUT_PARTITIONING_RBSP);
	SliceWithoutPartition slice2 = SliceWithoutPartition::generateBaselineSliceWithoutPartition(&pps,&sps, SLICE_TYPE::I, 0, NAL_UNIT_TYPE::CODEC_SLICE_OF_AN_IDR_PICTURE_SLICE_LAYER_WITHOUT_PARTITIONING_RBSP);
	SliceWithoutPartition slice3 = SliceWithoutPartition::generateBaselineSliceWithoutPartition(&pps,&sps, SLICE_TYPE::I, 0, NAL_UNIT_TYPE::CODEC_SLICE_OF_AN_IDR_PICTURE_SLICE_LAYER_WITHOUT_PARTITIONING_RBSP);
	for(int i = 0; i < CONFIGURE::HEIGHT * CONFIGURE::WIDTH / 256; i++) {
		slice1.addMacroBlock(&mbs[i]);
		slice2.addMacroBlock(&mbs1[i]);
		slice3.addMacroBlock(&mbs2[i]);
	}

	NalContainer nal3(NAL_REF_IDC::REFERENCE, NAL_UNIT_TYPE::CODEC_SLICE_OF_AN_IDR_PICTURE_SLICE_LAYER_WITHOUT_PARTITIONING_RBSP, &slice1);
	NalContainer nal4(NAL_REF_IDC::REFERENCE, NAL_UNIT_TYPE::CODEC_SLICE_OF_AN_IDR_PICTURE_SLICE_LAYER_WITHOUT_PARTITIONING_RBSP, &slice2);
	NalContainer nal5(NAL_REF_IDC::REFERENCE, NAL_UNIT_TYPE::CODEC_SLICE_OF_AN_IDR_PICTURE_SLICE_LAYER_WITHOUT_PARTITIONING_RBSP, &slice3);

	writeNALSequences({nal1, nal2, nal3, nal4, nal5}, outfile);
	outfile.close();
	delete[] mbs;
	std::cout << "successful" << std::endl;
	return 0;
}