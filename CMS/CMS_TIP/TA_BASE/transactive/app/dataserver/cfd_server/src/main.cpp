//#include "vld.h"
#include <signal.h>
#include <iostream>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "CFDServerCommonData.h"
#include "MarketDataPathManager.h"
#include "WorkTime.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_Base_App;

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
	TA_Base_App::CBoostLogger::getInstance();

	std::string strLogPath = "ALL_CFDServer_LOG_PATH";
	std::string strLogFileName = "CFDServer_Log_%Y-%m-%d_%H_%M_%S_%f_%4N.log";
	boost::log::trivial::severity_level nLogLevel = boost::log::trivial::trace;
	//boost::log::trivial::severity_level nLogLevel = boost::log::trivial::info;


	TA_Base_App::CBoostLogger::getInstance().setLogPath(strLogPath);
	TA_Base_App::CBoostLogger::getInstance().setLogFileName(strLogFileName);
	TA_Base_App::CBoostLogger::getInstance().setLogLevel(nLogLevel);
	TA_Base_App::CBoostLogger::getInstance().logInit();
	TA_Base_App::CBoostLogger::getInstance().testBoostLog();
}

void logUnInit()
{
	TA_Base_App::CBoostLogger::removeInstance();
}


//////////////////////////////////////////////////////////////////////////

void Test_CInstrumentBarInfoRequest()
{
	TA_Base_App::CAWorkTime* pWorkTime = NULL;
	CMarketDataPathManager* pMarketDataPathManager = NULL;

	CInstrumentBarInfoRequest instrumentBarInfoRequest; 
	instrumentBarInfoRequest.m_strHistoryMarketDataTotal = "c://TestData//HistoryMarketDataTotal";
	instrumentBarInfoRequest.m_strInstrumetBarInfoTotal = "C://TestData//InstrumetBarInfoTotal";

	pWorkTime = new TA_Base_App::CWorkTimeNoLock();
	pWorkTime->workBegin();

	pMarketDataPathManager = new CMarketDataPathManager();
	pMarketDataPathManager->setInstrumentBarInfoRequest(instrumentBarInfoRequest);
	pMarketDataPathManager->analieAllFiles();
	
	if (NULL != pMarketDataPathManager)
	{
		delete pMarketDataPathManager;
		pMarketDataPathManager = NULL;
	}

	pWorkTime->workEnd();

	LOG_INFO<<"Test_CInstrumentBarInfoRequest work time = "<<pWorkTime->getWorkTime();

	if (NULL != pWorkTime)
	{
		delete pWorkTime;
		pWorkTime = NULL;
	}

}


//////////////////////////////////////////////////////////////////////////



int main( int argc, char* argv[])
{ 	   
	logInit();
	BOOST_LOG_FUNCTION();

	signal(SIGINT, usr_signal);
#ifndef WIN32
	signal(SIGHUP, usr_signal);	//close putty
#endif

	Test_CInstrumentBarInfoRequest();

	//sleep
	{	
		boost::mutex::scoped_lock lock(g_mutexMainRun);
		//g_conditionMainRun.wait(lock);
	}

	logUnInit();

	return 0;
}












