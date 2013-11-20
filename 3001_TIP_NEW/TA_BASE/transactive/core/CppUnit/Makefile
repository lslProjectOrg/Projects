#############################################################################################################################################
#
# $File: //depot/COTS/CppUnit/Makefile $
# $Revision: #1 $
# $DateTime: 2006/11/10 10:41:03 $ 
# $Author: grace.koh $
#
# Builds the CppUnit unit test library.
#
############################################################################################################################################

PROJ_ROOT_DIR = ../../..

PROJ_NAME = CppUnit

PROJ_SOURCE = \
	AssertionFailedError.cpp \
	ConfigData.cpp \
	CppUnitException.cpp \
	Flag.cpp \
	INIFileException.cpp \
	INIFileTestCases.cpp \
	MethodsInputs.cpp \
	NoSuiteMethodException.cpp \
	ReadINIFile.cpp \
	TestCase.cpp \
	TestFailure.cpp \
	TestFrameworkBase.cpp \
	TestResult.cpp \
	TestRunner.cpp \
	TestSuite.cpp \
	TextTestResult.cpp \
	TextOutputter.cpp \
	XmlOutputter.cpp

PROJ_TYPE	= lib

PROJ_OBJDIR = ./obj

PROJ_INC_DIR = ./src

PROJ_LIBDIRS = 

PROJ_LIBS = 

PROJ_INCDIRS = 

PROJ_CODE_DIR = ./src

PROJ_OUT_DIR = $(PROJ_ROOT_DIR)/sdk/$(OPERATING_SYSTEM)_$(STRING_FLAG)$(RELEASE_FLAG)

#
# Libraries that this library is dependent upon.
#
DEPENDENT_LIBRARIES = 

##############################################################
# The main include file that has all the wiz bang stuff for neato compiling
##############################################################

include $(PROJ_ROOT_DIR)/code/tools/build/CommonMakefile.mak

install:
	#Do Nothing	


