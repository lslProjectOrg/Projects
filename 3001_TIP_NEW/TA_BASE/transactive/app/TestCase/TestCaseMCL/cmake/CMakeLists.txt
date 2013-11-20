PROJECT(LSLOCLTest)

SET(CMAKE_VERBOSE_MAKEFILE on )#for more error info

SET(PROJECT_ROOT_PATH "/export/home/tabuild/SVNWork/MyLearn/MyProject/DBProject/oracle/OCL/OCL_test/")#/LSL_ocilib

SET(PROJECT_SOURCE_DIR "/export/home/tabuild/SVNWork/MyLearn/MyProject/DBProject/oracle/OCL/OCL_test/src/")#/LSL_ocilib


SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/")#CMAKE_SOURCE_DIR=cmake

SET(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/")#lib save path

INCLUDE_DIRECTORIES("${PROJECT_ROOT_PATH}/src/")#where is .h file

INCLUDE_DIRECTORIES("/export/home/tabuild/SVNWork/MyLearn/MyProject/DBProject/oracle/OCL/LSL_OCL/src/")#where is .h file

INCLUDE_DIRECTORIES("/export/home/tabuild/SVNWork/MyLearn/MyProject/DBProject/oracle/OCL/OCL_test/src/")#where is .h file


INCLUDE_DIRECTORIES("/export/home/tabuild/cots/oracle/10.2.0/SOLARIS_5.10_i86pc/inc/")#where is .h file

LINK_DIRECTORIES("/export/home/tabuild/SVNWork/MyLearn/MyProject/DBProject/oracle/OCL/LSL_OCL/cmake/lib/")#where is lib file

LINK_DIRECTORIES("/export/home/tabuild/cots/oracle/10.2.0/SOLARIS_5.10_i86pc/lib/")#where is lib file

ADD_SUBDIRECTORY(src)#add path src


message("==========build====================")

message("PROJECT_ROOT_PATH:")
message(${PROJECT_ROOT_PATH})
message("PROJECT_SOURCE_DIR:")
message(${PROJECT_SOURCE_DIR})
message("PROJECT_BINARY_DIR:")
message(${PROJECT_BINARY_DIR})
message("EXECUTABLE_OUTPUT_PATH:")
message(${EXECUTABLE_OUTPUT_PATH})
message("LIBRARY_OUTPUT_PATH:")
message(${LIBRARY_OUTPUT_PATH})








