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
	std::string OUTPUT_FILE_MODE = "AnnexB";
	int FRAMES_TO_BE_ENCODED = 10;
	int FRAME_RATE = 30;
	int WIDTH = 320;
	int HEIGHT = 176;
	//Encoder Control
	int PROFILE_IDC = 66; //baseline
	int LEVEL_IDC = 30;   //3.0
}