//#include "vld.h"
#include <signal.h>
#include <iostream>


#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "ProjectCommonData.h"
#include "WorkTime.h"
#include "ConfigInfo.h"

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

	std::string strLogPath = "ALL_HistoryDataServer_LOG_PATH";
	std::string strLogFileName = "HistoryDataServer_Log_%Y-%m-%d_%H_%M_%S_%f_%4N.log";
	//boost::log::trivial::severity_level nLogLevel = boost::log::trivial::trace;
	boost::log::trivial::severity_level nLogLevel = boost::log::trivial::info;


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

void Test_Request()
{
	int nFunRes = 0;
	CConfigInfo::getInstance();
	TA_Base_App::CAWorkTime* pWorkTime = NULL;

	pWorkTime = new TA_Base_App::CWorkTimeNoLock();
	pWorkTime->workBegin();

	LOG_INFO<<"cfg start="<<CConfigInfo::getInstance().getStartTime();
	pWorkTime->workEnd();

	LOG_INFO<<"Test_Request work time = "<<pWorkTime->getWorkTime();
	

	if (NULL != pWorkTime)
	{
		delete pWorkTime;
		pWorkTime = NULL;
	}

	CConfigInfo::removeInstance();

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

	Test_Request();

	//sleep
	{	
		boost::mutex::scoped_lock lock(g_mutexMainRun);
		//g_conditionMainRun.wait(lock);
	}

	logUnInit();

	return 0;
}












