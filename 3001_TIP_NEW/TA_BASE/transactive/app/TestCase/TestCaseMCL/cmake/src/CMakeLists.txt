#AUX_SOURCE_DIRECTORY(/export/home/tabuild/SVNWork/MyLearn/MyProject/DBProject/oracle/OCL/OCL_test/src/ DIR_SRCS)#where is the *.cpp

#FILE(GLOB DIR_SRCS "/export/home/tabuild/SVNWork/MyLearn/MyProject/DBProject/oracle/OCL/OCL_test/src/*.cpp")#where is the *.cpp


FILE(GLOB DIR_SRCS "${PROJECT_ROOT_PATH}/src/*.cpp")#where is *.cpp

ADD_EXECUTABLE(LSLOCLTest ${DIR_SRCS})#build to exe file

TARGET_LINK_LIBRARIES(LSLOCLTest LSLOCL)

TARGET_LINK_LIBRARIES(LSLOCLTest clntsh)

TARGET_LINK_LIBRARIES(LSLOCLTest nnz10)



message("=========src==============")
message("DIR_SRCS:")
message(${DIR_SRCS})



