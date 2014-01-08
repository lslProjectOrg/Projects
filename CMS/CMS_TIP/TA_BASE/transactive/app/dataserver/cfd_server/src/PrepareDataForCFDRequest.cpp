#include "PrepareDataForCFDRequest.h"


#include "CFDServerUtilityFun.h"
#include "CFDInstrumentBarInfo.h"
#include "CFDInstrumentBarInfoStorager.h"
#include "CreateCFDInstrumentBarInfo.h"



#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CPrepareDataForCFDRequest::CPrepareDataForCFDRequest(unsigned int nInstrumentIDFirst, unsigned int nInstrumentIDSecond)
{
	BOOST_LOG_FUNCTION();
	m_nInstrumentIDFirst = nInstrumentIDFirst;
	m_nInstrumentIDSecond = nInstrumentIDSecond;
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_nCFDBarInterval = TA_Base_Core::TIME_BASE_S_1MIN;
	m_nInstrumentBarInterval = TA_Base_Core::TIME_BASE_S_5S;
}

CPrepareDataForCFDRequest::~CPrepareDataForCFDRequest(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}
}


void CPrepareDataForCFDRequest::setCFDBarInterval(int nCFDBarInterval)
{
	BOOST_LOG_FUNCTION();

	m_nCFDBarInterval = nCFDBarInterval;//1 mins
}

void CPrepareDataForCFDRequest::setInstrumentBarInterval(int nInstrumentBarInterval)
{
	BOOST_LOG_FUNCTION();

	m_nInstrumentBarInterval = nInstrumentBarInterval;// = 5;//second
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


int CPrepareDataForCFDRequest::getCFDBarInfo(LstBarInfoT&  lstCFDBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CCFDInstrumentBarInfoStorager*  pCFDStorager = NULL;
	CCreateCFDInstrumentBarInfo*  pCreateCFDInstrumentBarInfo = NULL;

	pCFDStorager = new CCFDInstrumentBarInfoStorager(m_nInstrumentIDFirst, m_nInstrumentIDSecond);

	//get data from db
	nFunRes = pCFDStorager->beginGetBarInfo(m_nInstrumentIDFirst, m_nInstrumentIDSecond, m_nCFDBarInterval);
	if (0 == nFunRes)
	{
		nFunRes = _GetCFDBarInfoFromStorager(pCFDStorager, lstCFDBarInfo);
	}

	if (0 != nFunRes)
	{
		//get data from db no data then try to create data and save to db
		LOG_DEBUG<<"DB have not save CFD Bar info yet!"
			<<" "<<"m_nInstrumentIDFirst="<<m_nInstrumentIDFirst
			<<" "<<"m_nInstrumentIDSecond="<<m_nInstrumentIDSecond
			<<" "<<"m_nInterval="<<m_nCFDBarInterval;

		LOG_DEBUG<<"begin create CFD Bar info "
			<<" "<<"m_nInstrumentIDFirst="<<m_nInstrumentIDFirst
			<<" "<<"m_nInstrumentIDSecond="<<m_nInstrumentIDSecond
			<<" "<<"m_nInterval="<<m_nCFDBarInterval;

		pCreateCFDInstrumentBarInfo = new CCreateCFDInstrumentBarInfo(m_nInstrumentIDFirst, m_nInstrumentIDSecond);
		//use Instrument 5seconds bar info to calc  CFD 1mins bar Info
		pCreateCFDInstrumentBarInfo->setInterval(m_nInstrumentBarInterval);
		nFunRes = pCreateCFDInstrumentBarInfo->createCFDbarInfoByDB();

	}

	if (0 == nFunRes)
	{
		//create data and save to db ok! then get data from db again
		nFunRes = pCFDStorager->beginGetBarInfo(m_nInstrumentIDFirst, m_nInstrumentIDSecond, m_nCFDBarInterval);
		if (0 == nFunRes)
		{
			nFunRes = _GetCFDBarInfoFromStorager(pCFDStorager, lstCFDBarInfo);
		}
	}
	else
	{
		//create data and save to db error
		LOG_ERROR<<"create CFD Bar info error!"
			<<" "<<"m_nInstrumentIDFirst="<<m_nInstrumentIDFirst
			<<" "<<"m_nInstrumentIDSecond="<<m_nInstrumentIDSecond
			<<" "<<"m_nInterval="<<m_nCFDBarInterval;

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


	if (NULL != pCreateCFDInstrumentBarInfo)
	{
		delete pCreateCFDInstrumentBarInfo;
		pCreateCFDInstrumentBarInfo = NULL;
	}
	if (NULL != pCFDStorager)
	{
		delete pCFDStorager;
		pCFDStorager = NULL;
	}
	return nFunRes;

}



NS_END(TA_Base_App)
















