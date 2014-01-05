//#include "vld.h"
#include <signal.h>
#include <iostream>
// #include <QtCore/QCoreApplication>
// #include <QtSql/QSqlDatabase>
// #include <QtSql/QtSql>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "MarketDataPathManager.h"
#include "core/utilities/src/WorkTime.h"

#include "CreateCFDInstrumentBarInfo.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_Base_App;
using namespace TA_Base_Core;

std::string g_string_strServerAddress = "tcp://127.0.0.1:5555";

boost::mutex g_mutexMainRun;
boost::condition_variable g_conditionMainRun;

void usr_signal(int SigNo)
{
	if (SigNo == SIGINT)
	{
		g_conditionMainRun.notify_one();
	}
	
#ifndef WIN32
	if (SigNo == SIGHUP)
	{
	}
#endif
}



void logInit()
{
	TA_Base_Core::CBoostLogger::getInstance();

	std::string strLogPath = "ALL_CFDServer_LOG_PATH";
	std::string strLogFileName = "CFDServer_Log_%Y-%m-%d_%H_%M_%S_%f_%4N.log";
	//boost::log::trivial::severity_level nLogLevel = boost::log::trivial::trace;
	boost::log::trivial::severity_level nLogLevel = boost::log::trivial::info;


	TA_Base_Core::CBoostLogger::getInstance().setLogPath(strLogPath);
	TA_Base_Core::CBoostLogger::getInstance().setLogFileName(strLogFileName);
	TA_Base_Core::CBoostLogger::getInstance().setLogLevel(nLogLevel);
	TA_Base_Core::CBoostLogger::getInstance().logInit();
	TA_Base_Core::CBoostLogger::getInstance().testBoostLog();
}

void logUnInit()
{
	TA_Base_Core::CBoostLogger::removeInstance();
}


//////////////////////////////////////////////////////////////////////////

void Test_CMarketDataPathManager()
{
	TA_Base_Core::CAWorkTime* pWorkTime = NULL;
	CMarketDataPathManager* pMarketDataPathManager = NULL;

	//C:\\LSL\\SVNWork\\CMS\\PUBLIC\\MarketData\\sample
	//std::string strPathName="G:\\LSL\\LSL_Code\\Svn_Work\\PUBLIC\\MarketData\\HistoryMarketData\\";//home
	std::string strPathName="C:\\LSL\\SVNWork\\CMS\\PUBLIC\\MarketData\\HistoryMarketData\\";//company


	pWorkTime = new TA_Base_Core::CWorkTimeNoLock();
	pWorkTime->workBegin();

	pMarketDataPathManager = new CMarketDataPathManager();
	pMarketDataPathManager->setPathName(strPathName);

	pMarketDataPathManager->analieAllFiles();
	
	if (NULL != pMarketDataPathManager)
	{
		delete pMarketDataPathManager;
		pMarketDataPathManager = NULL;
	}

	pWorkTime->workEnd();

	LOG_INFO<<"work time = "<<pWorkTime->getWorkTime();

	if (NULL != pWorkTime)
	{
		delete pWorkTime;
		pWorkTime = NULL;
	}

}

void Test_CCreateCFDInstrumentBarInfo()
{
	CCreateCFDInstrumentBarInfo* pCreateCFDInstrumentBarInfo = NULL;
	pCreateCFDInstrumentBarInfo = new CCreateCFDInstrumentBarInfo();


	pCreateCFDInstrumentBarInfo->doOneTest();


	if (NULL != pCreateCFDInstrumentBarInfo)
	{
		delete pCreateCFDInstrumentBarInfo;
		pCreateCFDInstrumentBarInfo = NULL;
	}
}
//////////////////////////////////////////////////////////////////////////



int main( int argc, char* argv[])
{ 	   
	logInit();
	BOOST_LOG_FUNCTION();

// 	QCoreApplication a(argc, argv);
// 	LOG_INFO<<"Available drivers:";
// 	QStringList drivers = QSqlDatabase::drivers();
// 	foreach(QString driver, drivers)
// 	{
// 		LOG_INFO<<"qt support:"<<driver.toStdString();
// 	}

	signal(SIGINT, usr_signal);
#ifndef WIN32
	signal(SIGHUP, usr_signal);	//close putty
#endif
	//Test_CMarketDataPathManager();

	Test_CCreateCFDInstrumentBarInfo();

	//sleep
	{	
		boost::mutex::scoped_lock lock(g_mutexMainRun);
		//g_conditionMainRun.wait(lock);
	}

	logUnInit();

	return 0;
}












