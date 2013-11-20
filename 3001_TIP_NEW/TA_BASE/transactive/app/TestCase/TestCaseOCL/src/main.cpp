//WIN32;_DEBUG;_CONSOLE;__WIN32__;_USE_32BIT_TIME_T

//#include "vld.h"
#include <iostream>


#include "core/CppUnit/src/TestRunner.h"
#include "core/CppUnit/src/TextOutputter.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

#include "TestClassForGetDataSameTime.h"


int main()
{
	//step one set Debug working dir      for  ./cfg  ./output
	//eg. debug working dir "C:\LSL\SVNWork\MyWork\MyTestCode\MyTestCode_BIN"
	
	//step two set log param
	TA_Base_Core::DebugUtil::getInstance().setLevel(TA_Base_Core::DebugUtil::DebugDatabase);//DebugDatabase DebugDebug		 DebugSQL DebugTrace	DebugTrace
	TA_Base_Core::DebugUtil::getInstance().setFile("./output/LOG_TestCaseOCL.log");
	TA_Base_Core::DebugUtil::getInstance().setMaxFiles( 50 );
	TA_Base_Core::DebugUtil::getInstance().setMaxSize(20000000);

	//set run params
// #define RPARAM_DBONLINE             "DataBaseGood"
// #define RPARAM_DBOFFLINE            "DataBaseBad"

	//mysql
	TA_Base_Core::RunParams::getInstance().set("DataBase_TRANSACT@192.168.123.43","DataBaseGood");
	TA_Base_Core::RunParams::getInstance().set(RPARAM_DBCONNECTION,"Oracle,IT271350_5,IT271350_5");

	TA_Base_Core::RunParams::getInstance().set(RPARAM_DBCONNECTIONFILE,"./cfg/OCConnectionStrings.csv");
		
	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "main  start ....");

	TestRunner runner;

	CTestClassForGetDataSameTime CTestClassObject("CTestClassForGetDataSameTime");
	runner.addSuite( CTestClassObject.suite() );


	try
	{
		//step one set Debug working dir      for  ./cfg  ./output
		//eg. debug working dir "C:\mytestpro7\DaiTestGetSetValuePro\bin"

		// Run config file. Make sure it is in the cfg directory.
		runner.begin( "./cfg/config_TestCaseOCL.ini" );
	}
	catch (...)
	{
		std::cout << "Caught an unhandled exception" << std::endl;
	}
	
	
	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "main  end");

	return 0;
}

























