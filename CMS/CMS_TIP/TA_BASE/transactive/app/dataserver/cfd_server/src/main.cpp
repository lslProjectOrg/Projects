//#include "vld.h"
#include <signal.h>
#include <iostream>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "MarketDataPathManager.h"

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
	boost::log::trivial::severity_level nLogLevel = boost::log::trivial::trace;


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

int main( int argc, char* argv[])
{ 	   
	signal(SIGINT, usr_signal);
#ifndef WIN32
	signal(SIGHUP, usr_signal);	//close putty
#endif
	
	logInit();
	BOOST_LOG_FUNCTION();

	//C:\\LSL\\SVNWork\\CMS\\PUBLIC\\MarketData\\sample
	std::string strPathName="G:\\LSL\\LSL_Code\\Svn_Work\\PUBLIC\\MarketData\\HistoryMarketData\\";//home
	//std::string strPathName="C:\\LSL\\SVNWork\\CMS\\PUBLIC\\MarketData\\HistoryMarketData\\";//company
	CMarketDataPathManager* pMarketDataPathManager = NULL;
	pMarketDataPathManager = new CMarketDataPathManager();
	pMarketDataPathManager->setPathName(strPathName);
	pMarketDataPathManager->analieAllFiles();




	//sleep
	{	
		boost::mutex::scoped_lock lock(g_mutexMainRun);
		//g_conditionMainRun.wait(lock);
	}

	delete pMarketDataPathManager;
	pMarketDataPathManager = NULL;
	logUnInit();

	return 0;
}












