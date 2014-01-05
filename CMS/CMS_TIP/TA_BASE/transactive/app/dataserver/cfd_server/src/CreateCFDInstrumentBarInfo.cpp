#include "CreateCFDInstrumentBarInfo.h"

#include "BarCalculator.h"

#include "CFDServerUtilityFun.h"
#include "CFDInstrumentBarInfo.h"



#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CCreateCFDInstrumentBarInfo::CCreateCFDInstrumentBarInfo(void)
{
	BOOST_LOG_FUNCTION();
	m_pUtilityFun = new CCFDServerUtilityFun();
}

CCreateCFDInstrumentBarInfo::~CCreateCFDInstrumentBarInfo( void )
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}

}

void CCreateCFDInstrumentBarInfo::doOneTest()
{
	BOOST_LOG_FUNCTION();
	CCFDInstrumentBarInfo* pCFDInstrumentBarInfoTmp = NULL;
	int nCFDInstrumentID = 3620*3521;
	int interval = 5;//second

	Bar BarInfo_3620;
	Bar BarInfo_3621;

	pCFDInstrumentBarInfoTmp =new CCFDInstrumentBarInfo();

	BarInfo_3620.Time = 1387502103;
	BarInfo_3620.Open = 2346;
	BarInfo_3620.Close = 2345;
	BarInfo_3620.High = 2347;
	BarInfo_3620.Low = 2344;
	BarInfo_3620.Volume = 0;

	BarInfo_3621.Time = 1387502103;
	BarInfo_3621.Open = 2336;
	BarInfo_3621.Close = 2335;
	BarInfo_3621.High = 2337;
	BarInfo_3621.Low = 2334;
	BarInfo_3621.Volume = 0;


	pCFDInstrumentBarInfoTmp->setCFDInstrumentID(nCFDInstrumentID);
	pCFDInstrumentBarInfoTmp->setInterval(interval);
	pCFDInstrumentBarInfoTmp->setBarInfoFirst(BarInfo_3620);
	pCFDInstrumentBarInfoTmp->setBarInfoFirst(BarInfo_3621);
	pCFDInstrumentBarInfoTmp->buidCFDBarInfo();


	if (NULL != pCFDInstrumentBarInfoTmp)
	{
		delete pCFDInstrumentBarInfoTmp;
		pCFDInstrumentBarInfoTmp = NULL;
	}

}



NS_END(TA_Base_App)
















