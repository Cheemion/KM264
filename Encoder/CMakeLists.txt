# CMakeList.txt: KM264 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
cmake_minimum_required (VERSION 3.8)
project ("KM264_encoder")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 将源代码添加到此项目的可执行文件。
add_executable (encoder 
                "encoder.cpp"  
                "header/common.h"
                "header/configuration.h"
                "header/nal_unit.h" 
                "header/byte_writer.h"
                "header/sps.h"
                "header/pps.h"
                "header/slice.h"
                "header/macro_block.h"
               )



