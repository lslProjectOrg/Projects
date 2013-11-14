#!/bin/bash
#! this file is server as a one step build the third-part mysql connection library(mcl)
#! under Solaris platform by using g++ compiler
#! 1. insert SET(MYSQL_CXXFLAGS "") at the end of the file FindMySQL.cm
#! 2. make sure that the cmake installed and the bin files path included in the PATH environment variable
#! 3. change the macro value to the real environment like the following example
#! 4. run this file at the root of MCL path
BOOST_ROOT="/u02/demo/PRIV_3001_MYSQL/Base_3001/Base/code/cots/boost/boost_1_39_0"
MYSQL_INCLUDE_DIR="/u02/demo/PRIV_3001_MYSQL/Base_3001/Base/code/cots/mysql/5.5.14/SOLARIS_5.10_i86pc/inc"
MYSQL_LIB_DIR="/u02/demo/PRIV_3001_MYSQL/Base_3001/Base/code/cots/mysql/5.5.14/SOLARIS_5.10_i86pc/lib"
MYSQL_DIR="/u02/demo/PRIV_3001_MYSQL/Base_3001/Base/code/cots/mysql/5.5.14/SOLARIS_5.10_i86pc"
CMAKE_BIN_DIR="/u02/demo/PRIV_3001_MYSQL/Base_3001/Base/code/cots/cmake/cmake-2.8.3/bin"
OUTPUT_DIR="./lib/SOLARIS_5.10_i86pc"

CMAKECACHETXT="./CMakeCache.txt"
CMAKEFILE_DIR="./CMakeFiles"
CPACKCONFIG="./CPackConfig.cmake"
CPACKSRCCONFIG="./CPackSourceConfig.cmake"
CMAKEINSTALL="./cmake_install.cmake"

make clean
if test -d $CMAKE_BIN_DIR
then
	echo "cmake bin dir: "$CMAKE_BIN_DIR
else
	echo "Error: The cmake bin directory does not exists..."
	echo $CMAKE_BIN_DIR
	exit
fi

if test -a $CMAKECACHETXT
then
	echo "remove the old cmake cached files..."
	rm -r $CMAKECACHETXT
	rm -r $CMAKEFILE_DIR
	rm -r $CPACKCONFIG
	rm -r $CPACKSRCCONFIG
	rm -r $CMAKEINSTALL
fi

echo "export the needed environment varibles..."
export BOOST_ROOT=$BOOST_ROOT
export MYSQL_INCLUDE_DIR=$MYSQL_INCLUDE_DIR
export MYSQL_LIB_DIR=$MYSQL_LIB_DIR
export MYSQL_DIR=$MYSQL_DIR
export PATH=$CMAKE_BIN_DIR:$PATH

echo "generate the makefile..."
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DMYSQLCPPCONN_BUILD_EXAMPLES:BOOL=0 -DMYSQLCLIENT_STATIC_BINDING:BOOL=1
make DEBUG=1 

echo "copy the output libraries..."
mv ./driver/libTA_mcl-static.a    $OUTPUT_DIR"/libTA_mcl.a"
cp ./driver/libTA_mcl.so.5.1.1.0  $OUTPUT_DIR"/libTA_mcl.so"
cp ./driver/libTA_mcl.so.5.1.1.0  $OUTPUT_DIR"/libTA_mcl.so.5"
mv ./driver/libTA_mcl.so.5.1.1.0  $OUTPUT_DIR"/libTA_mcl.so.5.1.1.0"




