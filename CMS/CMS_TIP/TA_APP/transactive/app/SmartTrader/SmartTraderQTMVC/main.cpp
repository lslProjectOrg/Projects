//qt app
//#include "vld.h"
#include <QtGui/QApplication>
#include "QTDebugLogger.h"

#include "ClientTestCase.h"

#include "ExchangeInfoMVCMainWindow.h"

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
	Q_INIT_RESOURCE(SmartTraderQTMVC);

	int nFunRes = 0;
	QApplication app(argc, argv);
	initLog();

	//////////////////////////////////////////////////////////////////////////
// 	TA_App_App::CClientTestCase clientTestCase;
// 	//clientTestCase.test_get_exchange_info();
// 	//clientTestCase.test_TreeModelExchange();
// 	clientTestCase.test_TreeModel();

	//////////////////////////////////////////////////////////////////////////


	ExchangeInfoMVCMainWindow testExchangeInfoMVCMainWindow;
	testExchangeInfoMVCMainWindow.show();
	//////////////////////////////////////////////////////////////////////////


	nFunRes = app.exec();

	unInitLog();

	return 0;
}





#if 0

#include <QtGui>

#include "SmartTraderQTMVCMainWindow.h"

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(SmartTraderQTMVC);

	QApplication app(argc, argv);
	SmartTraderQTMVCMainWindow window;
#if defined(Q_OS_SYMBIAN)
	window.showMaximized();
#else
	window.show();
#endif
	return app.exec();
}
#endif
