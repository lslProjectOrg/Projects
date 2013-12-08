// #include <vector>
// #include <string>


#include "CommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

//#include "ace/OS.h"
#include "Logger.h"
#include "TestCaseForCS.h"


boost::mutex g_mutexMainRun;
boost::condition_variable g_conditionMainRun;









int main( int argc, char* argv[] )
{
	TA_Base_Test::CLogger::getInstance();
	std::string strLogFile = "logFile_TestCaseCMSServer.log";
	TA_Base_Test::CLogger::getInstance().setlogFile(strLogFile);
	TA_Base_Test::CLogger::getInstance().setlogLevel(TA_Base_Test::DebugTrace);//DebugTrace DebugInfo

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugInfo, "main begin");

	TA_Base_Test::TestCaseForCS* pTEST_TestCaseCS = new TA_Base_Test::TestCaseForCS();
	pTEST_TestCaseCS->runTestCase_ForServer();
	//pTEST_TestCaseCS->runTestCase_ForClient();

	//sleep
	{	
		boost::mutex::scoped_lock lock(g_mutexMainRun);
		g_conditionMainRun.wait(lock);
	}

	delete pTEST_TestCaseCS;
	pTEST_TestCaseCS = NULL;

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugInfo, "main end");

	TA_Base_Test::CLogger::removeInstance();
	return 0;
}










