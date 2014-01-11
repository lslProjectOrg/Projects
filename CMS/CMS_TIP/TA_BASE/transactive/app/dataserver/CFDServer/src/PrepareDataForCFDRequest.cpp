#include "PrepareDataForCFDRequest.h"


#include "CFDServerUtilityFun.h"
#include "CFDInstrumentBarInfo.h"
#include "CFDInstrumentBarInfoStorager.h"
#include "CreateCFDInstrumentBarInfo.h"



#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CPrepareDataForCFDRequest::CPrepareDataForCFDRequest(const CCFDRequest& cfdRequest)
{
	BOOST_LOG_FUNCTION();
	m_nCFDRequest = cfdRequest;
}

CPrepareDataForCFDRequest::~CPrepareDataForCFDRequest(void)
{
	BOOST_LOG_FUNCTION();
}





int CPrepareDataForCFDRequest::_GetCFDBarInfoFromStorager(CCFDInstrumentBarInfoStorager* pCFDStorager, LstBarInfoT&  lstCFDBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	Bar barCFDBarInfo;

	while (1)
	{
		nFunRes = pCFDStorager->getNextBarInfo(barCFDBarInfo);
		
		if (0 != nFunRes)
		{
			//not data 
			break;
		}
		lstCFDBarInfo.push_back(barCFDBarInfo);		
	}

	//check data
	if (lstCFDBarInfo.empty())
	{
		nFunRes = -1;
	}
	else
	{
		nFunRes = 0;
	}
	return nFunRes;
}

int CPrepareDataForCFDRequest::_TryGetCFDBarInfo(LstBarInfoT&  lstCFDBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	CCFDInstrumentBarInfoStorager*  pCFDStorager = NULL;

	//check data is in db
	pCFDStorager = new CCFDInstrumentBarInfoStorager(m_nCFDRequest);

	//get data from db
	nFunRes = pCFDStorager->beginGetBarInfo(m_nCFDRequest);
	if (0 == nFunRes)
	{
		nFunRes = _GetCFDBarInfoFromStorager(pCFDStorager, lstCFDBarInfo);
	}

	if (0 == nFunRes)
	{
		//get data from db
	}

	if (NULL != pCFDStorager)
	{
		delete pCFDStorager;
		pCFDStorager = NULL;
	}

	return nFunRes;
}


int CPrepareDataForCFDRequest::getCFDBarInfo(LstBarInfoT&  lstCFDBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CCreateCFDInstrumentBarInfo*  pCreateCFDInstrumentBarInfo = NULL;

	nFunRes = _TryGetCFDBarInfo(lstCFDBarInfo);	

	if (0 == nFunRes)
	{
		return nFunRes;
	}
	else
	{
		//data is not in db then create
		//get data from db no data then try to create data and save to db
		LOG_DEBUG<<"DB have not save CFD Bar info yet! begin create CFD Bar info"
			<<" "<<"m_nInstrumentIDFirst="<<m_nCFDRequest.m_nCFDInstrumentIDFirst
			<<" "<<"m_nInstrumentIDSecond="<<m_nCFDRequest.m_nCFDInstrumentIDSecond
			<<" "<<"m_nInterval="<<m_nCFDRequest.m_nCFDInterval;

		pCreateCFDInstrumentBarInfo = new CCreateCFDInstrumentBarInfo(m_nCFDRequest);
		//use MarketData to calc  CFD 1mins bar Info
		nFunRes = pCreateCFDInstrumentBarInfo->createCFDbarInfoByMarketData();
	}

	if (NULL != pCreateCFDInstrumentBarInfo)
	{
		delete pCreateCFDInstrumentBarInfo;
		pCreateCFDInstrumentBarInfo = NULL;
	}

	if (0 == nFunRes)
	{
		//create data ok, then get data again
		nFunRes = _TryGetCFDBarInfo(lstCFDBarInfo);	
	}
	else
	{
		//create data and save to db error
		LOG_ERROR<<"create CFD Bar info error!"
			<<" "<<"m_nInstrumentIDFirst="<<m_nCFDRequest.m_nCFDInstrumentIDFirst
			<<" "<<"m_nInstrumentIDSecond="<<m_nCFDRequest.m_nCFDInstrumentIDSecond
			<<" "<<"m_nInterval="<<m_nCFDRequest.m_nCFDInterval;

	}

	return nFunRes;

}



NS_END(TA_Base_App)
















