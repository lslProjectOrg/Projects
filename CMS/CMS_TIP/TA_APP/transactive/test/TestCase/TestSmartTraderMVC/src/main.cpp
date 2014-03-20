//qt app
//#include "vld.h"
#include <QtGui/QApplication>
#include "QTDebugLogger.h"

#include "ClientTestCase.h"

#include "BoostLogger.h"
USING_BOOST_LOG;


void initLog()
{
	TA_App_App::CBoostLogger::getInstance();

	std::string strLogPath = "ALL_LOG_PATH";
	//std::string strLogFileName = "Test_Log_%Y-%m-%d_%H_%M_%S_%f_%4N.log";
	std::string strLogFileName = "Test_Log_%Y-%m-%d_%H_%M_%S.log";
	boost::log::trivial::severity_level nLogLevel = boost::log::trivial::trace;
	//boost::log::trivial::severity_level nLogLevel = boost::log::trivial::info;
	//boost::log::trivial::severity_level nLogLevel = boost::log::trivial::debug;


	TA_App_App::CBoostLogger::getInstance().setLogPath(strLogPath);
	TA_App_App::CBoostLogger::getInstance().setLogFileName(strLogFileName);
	TA_App_App::CBoostLogger::getInstance().setLogLevel(nLogLevel);
	TA_App_App::CBoostLogger::getInstance().logInit();
	TA_App_App::CBoostLogger::getInstance().testBoostLog();

	//
	TA_App_App::CQTDebugLogger::getInstance();

}




void unInitLog()
{
	TA_App_App::CQTDebugLogger::removeInstance();
	TA_App_App::CBoostLogger::removeInstance();
}

int main(int argc, char *argv[])
{
	int nFunRes = 0;
	QApplication app(argc, argv);
	initLog();

	//////////////////////////////////////////////////////////////////////////
	TA_App_App::CClientTestCase clientTestCase;
	//clientTestCase.test_get_exchange_info();
	//clientTestCase.test_TreeModelExchange();
	clientTestCase.test_TreeModel();





	//////////////////////////////////////////////////////////////////////////

	nFunRes = app.exec();

	unInitLog();
	
	return 0;
}

