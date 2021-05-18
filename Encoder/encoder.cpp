#include <iostream>
#include <fstream>
#include <vector> 
#include "header/common.h"
#include "header/configuration.h"
#include "header/byte_writer.h"
#include "header/nal_unit.h"
int main() {
	std::ofstream outfile("test.264", std::fstream::out | std::fstream::binary);
	SPS sps1 = SPS::generateBaselineMode(0, 0, 10);
	SPS sps2 = SPS::generateBaselineMode(0, 0, 10);
	NalContainer nal1(NAL_REF_IDC::REFERENCE, NAL_UNIT_TYPE::SEQUENCE_PARAMETER_SET, &sps1);
	NalContainer nal2(NAL_REF_IDC::REFERENCE, NAL_UNIT_TYPE::SEQUENCE_PARAMETER_SET, &sps2);
	writeNALSequences({nal1, nal2}, outfile);
	outfile.close();
	std::cout << CONFIGURE::INPUT_FILE << std::endl;
	return 0;
}