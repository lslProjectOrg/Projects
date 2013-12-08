// #include <vector>
// #include <string>


#include "CommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

//#include "ace/OS.h"
#include "Logger.h"
#include "ClientManager.h"


boost::mutex g_mutexMainRun;
boost::condition_variable g_conditionMainRun;









int main( int argc, char* argv[] )
{
	TA_Base_Test::CLogger::getInstance();
	std::string strLogFile = "logFile_TestCaseCMSClient100.log";
// 	strLogFile += argv[1];
// 	strLogFile += "__.log";
	TA_Base_Test::CLogger::getInstance().setlogFile(strLogFile);
	TA_Base_Test::CLogger::getInstance().setlogLevel(TA_Base_Test::DebugInfo);//DebugTrace DebugInfo

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugInfo, "main begin");


	TA_Base_Test::CClientManager* pClientManager = new TA_Base_Test::CClientManager();
	pClientManager->start();


	//sleep
	{	
		boost::mutex::scoped_lock lock(g_mutexMainRun);
		g_conditionMainRun.wait(lock);
	}

	delete pClientManager;
	pClientManager = NULL;

	_SysLog(LogSourceFLInfo, TA_Base_Test::DebugInfo, "main end");

	TA_Base_Test::CLogger::removeInstance();
	return 0;
}










