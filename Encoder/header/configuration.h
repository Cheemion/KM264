/**
 *  Usage: Encoder Configuration File
	Date: 2021/5/16
	email: 376301008@qq.com
	Author: Kim
 * **/
#pragma once
#include <string>
namespace CONFIGURE {
	//file
	std::string INPUT_FILE = "C:\\Users\\kim\\Desktop\\out.yuv";
	std::string OUTPUT_FILE = "test.h264";
	int OUTPUT_FILE_MODE = OUTPUT_MODE::ANNEX_B;
	int FRAMES_TO_BE_ENCODED = 10;
	int FRAME_RATE = 30;
	int WIDTH = 320;
	int HEIGHT = 176;
	//Encoder Control
	int PROFILE_IDC = PROFILE_IDC::BASELINE; //baseline
	int LEVEL_IDC = LEVEL_IDC::LEVEL3;   //3.0
	int CHROMA_FORMAT_IDC = 1; //4:2:0
	int MB_WIDTH_C = 8;
	int MB_HEIGHT_C = 8;
}