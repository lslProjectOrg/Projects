#include "CreateCFDInstrumentBarInfo.h"

#include "BarCalculator.h"

#include "CFDServerUtilityFun.h"
#include "CFDInstrumentBarInfo.h"
#include "SyncMarketDataForCFD.h"
#include "CFDInstrumentBarInfoCalculator.h"



#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CCreateCFDInstrumentBarInfo::CCreateCFDInstrumentBarInfo(unsigned int nInstrumentIDFirst, unsigned int nInstrumentIDSecond)
{
	BOOST_LOG_FUNCTION();
	m_nInstrumentIDFirest = nInstrumentIDFirst;
	m_nInstrumentIDSecond = nInstrumentIDSecond;
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_CSyncMarketDataForCFD = new CSyncMarketDataForCFD();
	m_pCFDBarInfoCalculator = new CCFDInstrumentBarInfoCalculator(m_nInstrumentIDFirest, m_nInstrumentIDSecond);
	
}

CCreateCFDInstrumentBarInfo::~CCreateCFDInstrumentBarInfo( void )
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}

	if (NULL != m_CSyncMarketDataForCFD)
	{
		delete m_CSyncMarketDataForCFD;
		m_CSyncMarketDataForCFD = NULL;
	}

	if (NULL != m_pCFDBarInfoCalculator)
	{
		delete m_pCFDBarInfoCalculator;
		m_pCFDBarInfoCalculator = NULL;
	}

}

void CCreateCFDInstrumentBarInfo::doOneTest()
{
	BOOST_LOG_FUNCTION();
	CSyncMarketDataForCFD::LstCFDBarInfoT   lstCFDBarInfoTmp;
	int interval = 5;//second
	int nFunRes = 0;

	Bar BarInfo_3620;
	Bar BarInfo_3621;


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

	m_CSyncMarketDataForCFD->setCFDInstrumentIDFirst(m_nInstrumentIDFirest);
	m_CSyncMarketDataForCFD->setCFDInstrumentIDSecond(m_nInstrumentIDSecond);
	m_CSyncMarketDataForCFD->setInterval(interval);
	m_CSyncMarketDataForCFD->syncSingleCFDBarInfo(BarInfo_3620, BarInfo_3621, lstCFDBarInfoTmp);

	UpdateCFDMarketData(lstCFDBarInfoTmp);

	m_CSyncMarketDataForCFD->clearCFDBarInfoList(lstCFDBarInfoTmp);
}

int CCreateCFDInstrumentBarInfo::UpdateCFDMarketData(CSyncMarketDataForCFD::LstCFDBarInfoT&  lstCFDBarInfo)
{	
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CSyncMarketDataForCFD::LstCFDBarInfoIterT  iterLst;
	CCFDInstrumentBarInfo* pCFDBarInfo = NULL;

	iterLst = lstCFDBarInfo.begin();
	while (iterLst != lstCFDBarInfo.end())
	{
		pCFDBarInfo = (*iterLst);

		m_pCFDBarInfoCalculator->updateMarketData(pCFDBarInfo);

		iterLst++;
	}

	

	return nFunRes;
}



NS_END(TA_Base_App)
















