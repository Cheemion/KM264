#include <iostream>
#include "header/common.h"
#include "header/configuration.h"
#include "header/byte_writer.h"
#include "header/nal_unit.h"
#include "header/slice.h"
#include "header/sps.h"
#include "header/pps.h"
int main() {
	std::ofstream outfile("test.264", std::fstream::out | std::fstream::binary);
	SPS sps = SPS::generateBaselineMode();
	PPS pps = PPS::generateBaselineMode(sps.getID());
	NalContainer nal1(NAL_REF_IDC::REFERENCE, NAL_UNIT_TYPE::SEQUENCE_PARAMETER_SET, &sps);
	NalContainer nal2(NAL_REF_IDC::REFERENCE, NAL_UNIT_TYPE::PICTURE_PARAMETER_SET, &pps);
	writeNALSequences({nal1, nal2}, outfile);
	outfile.close();
	std::cout << CONFIGURE::INPUT_FILE << std::endl;
	return 0;
}