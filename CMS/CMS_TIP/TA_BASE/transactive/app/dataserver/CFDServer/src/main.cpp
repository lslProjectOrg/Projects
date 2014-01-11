//#include "vld.h"
#include <signal.h>
#include <iostream>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "CFDServerCommonData.h"
#include "MarketDataPathManager.h"
#include "core/utilities/src/WorkTime.h"
#include "PrepareDataForCFDRequest.h"

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
	//boost::log::trivial::severity_level nLogLevel = boost::log::trivial::info;


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

void Test_CInstrumentBarInfoRequest()
{
	TA_Base_Core::CAWorkTime* pWorkTime = NULL;
	CMarketDataPathManager* pMarketDataPathManager = NULL;

	CInstrumentBarInfoRequest instrumentBarInfoRequest; 
	instrumentBarInfoRequest.m_strHistoryMarketDataTotal = "c://TestData//HistoryMarketDataTotal";
	instrumentBarInfoRequest.m_strInstrumetBarInfoTotal = "C://TestData//InstrumetBarInfoTotal";

	pWorkTime = new TA_Base_Core::CWorkTimeNoLock();
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


void Test_CInstrumentMarketDataRequest()
{
	TA_Base_Core::CAWorkTime* pWorkTime = NULL;
	CMarketDataPathManager* pMarketDataPathManager = NULL;

	CInstrumentMarketDataRequest instrumentMarketDataRequest; 
	instrumentMarketDataRequest.m_strHistoryMarketDataTotal = "c://TestData//HistoryMarketDataTotal";
	instrumentMarketDataRequest.m_strHistoryMarketDataInstrument = "C://TestData//HistoryMarketDataInstrument";

	pWorkTime = new TA_Base_Core::CWorkTimeNoLock();
	pWorkTime->workBegin();

	pMarketDataPathManager = new CMarketDataPathManager();
	pMarketDataPathManager->setInstrumentMarketDataRequest(instrumentMarketDataRequest);
	pMarketDataPathManager->analieAllFiles();

	if (NULL != pMarketDataPathManager)
	{
		delete pMarketDataPathManager;
		pMarketDataPathManager = NULL;
	}

	pWorkTime->workEnd();

	LOG_INFO<<"Test_CInstrumentMarketDataRequest work time = "<<pWorkTime->getWorkTime();

	if (NULL != pWorkTime)
	{
		delete pWorkTime;
		pWorkTime = NULL;
	}

}

void Test_CCFDRequest()
{
	//c://TestData//HistoryMarketDataTotal   20131220.csv 
	//C://TestData//HistoryMarketDataInstrument   3320.csv
	//C://TestData//CFDBarInfoTotal   SQLiteDB_CFD_3320_3321.db
	//C://TestData//InstrumetBarInfoTotal   SQLiteDB_3320.db

	int nFunRes = 0;

	CCFDRequest cfdRequest;
	CPrepareDataForCFDRequest* pPrepareDataForCFDRequest = NULL;
	CPrepareDataForCFDRequest::LstBarInfoT  lstCFDBarInfo;
	TA_Base_Core::CAWorkTime* pWorkTime = NULL;
	pWorkTime = new TA_Base_Core::CWorkTimeNoLock();
	pWorkTime->workBegin();

	cfdRequest.m_strPathHistoryMarketDataInstrument = "C://TestData//HistoryMarketDataInstrument";
	cfdRequest.m_strCFDBarInfoTotal = "C://TestData//CFDBarInfoTotal";
	cfdRequest.m_nCFDInstrumentIDFirst = 3620;
	cfdRequest.m_nCFDInstrumentIDSecond = 3621;
	cfdRequest.m_nXValue = 1;
	cfdRequest.m_nYValue = 1;
	cfdRequest.m_nCFDInterval = TA_Base_Core::TIME_BASE_S_1MIN;

	pPrepareDataForCFDRequest = new CPrepareDataForCFDRequest(cfdRequest);
	
	nFunRes = pPrepareDataForCFDRequest->getCFDBarInfo(lstCFDBarInfo);

	pWorkTime->workEnd();

	LOG_INFO<<"Test_PrepareDataForCFDRequest work time = "<<pWorkTime->getWorkTime();
	if (NULL != pWorkTime)
	{
		delete pWorkTime;
		pWorkTime = NULL;
	}

	if (NULL != pPrepareDataForCFDRequest)
	{
		delete pPrepareDataForCFDRequest;
		pPrepareDataForCFDRequest = NULL;
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

	//Test_CInstrumentBarInfoRequest();
	//Test_CInstrumentMarketDataRequest();
	Test_CCFDRequest();

	//sleep
	{	
		boost::mutex::scoped_lock lock(g_mutexMainRun);
		//g_conditionMainRun.wait(lock);
	}

	logUnInit();

	return 0;
}












