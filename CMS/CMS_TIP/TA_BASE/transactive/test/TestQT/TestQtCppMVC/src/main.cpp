//qt app
#include <QtGui/QApplication>
#include "QTDebugLogger.h"

#include "ClientTestCase.h"
#include "TestCaseMVC.h"

#include "BoostLogger.h"
USING_BOOST_LOG;


void initLog()
{
	TA_Base_App::CBoostLogger::getInstance();

	std::string strLogPath = "ALL_LOG_PATH";
	//std::string strLogFileName = "Test_Log_%Y-%m-%d_%H_%M_%S_%f_%4N.log";
	std::string strLogFileName = "Test_Log_%Y-%m-%d_%H_%M_%S.log";
	boost::log::trivial::severity_level nLogLevel = boost::log::trivial::trace;
	//boost::log::trivial::severity_level nLogLevel = boost::log::trivial::info;
	//boost::log::trivial::severity_level nLogLevel = boost::log::trivial::debug;


	TA_Base_App::CBoostLogger::getInstance().setLogPath(strLogPath);
	TA_Base_App::CBoostLogger::getInstance().setLogFileName(strLogFileName);
	TA_Base_App::CBoostLogger::getInstance().setLogLevel(nLogLevel);
	TA_Base_App::CBoostLogger::getInstance().logInit();
	TA_Base_App::CBoostLogger::getInstance().testBoostLog();

	//
	TA_Base_App::CQTDebugLogger::getInstance();

}




void unInitLog()
{
	TA_Base_App::CQTDebugLogger::removeInstance();
	TA_Base_App::CBoostLogger::removeInstance();
}

int main(int argc, char *argv[])
{
	int nFunRes = 0;
	QApplication app(argc, argv);
	initLog();

	//////////////////////////////////////////////////////////////////////////
	//TA_Base_App::CClientTestCase clientTestCase;
	//clientTestCase.test_get_exchange_info();

	TA_Base_App::CTestCaseMVC testCaseMVC;
	testCaseMVC.test_TreeMode_QTDemo();






	//////////////////////////////////////////////////////////////////////////

	nFunRes = app.exec();

	unInitLog();
	
	return 0;
}




