#include <iostream>
#include <fstream>
#include <vector> 
#include "header/common.h"
#include "header/configuration.h"
#include "header/byte_writer.h"
#include "header/nal_unit.h"
int main() {
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