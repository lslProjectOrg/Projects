#include "CFDInstrumentBarInfoCalculator.h"

#include "core/utilities/src/UtilitiesCommonData.h"

#include "MarketData.h"
#include "BarCalculator.h"
#include "CFDInstrumentBarInfoStorager.h"
#include "CFDServerUtilityFun.h"
#include "CFDInstrumentBarInfo.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_Base_Core;

NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////
CCFDInstrumentBarInfoCalculator::CCFDInstrumentBarInfoCalculator(unsigned int nInstrumentIDFirst, unsigned int nInstrumentIDSecond)
{	
	BOOST_LOG_FUNCTION();
	m_nInstrumentIDFirest = nInstrumentIDFirst;
	m_nInstrumentIDSecond = nInstrumentIDSecond;
	m_pMapTimeBarInfo = new MapIntervalBarInfoT();
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_pStorager = new CCFDInstrumentBarInfoStorager(m_nInstrumentIDFirest, m_nInstrumentIDSecond);

	unsigned int m_nInstrumentIDForBarCalculator = m_nInstrumentIDFirest * m_nInstrumentIDSecond;
	m_pBarCalculator = new BarCalculator(m_nInstrumentIDForBarCalculator);

	m_pBarCalculator->onNewBar = boost::bind(&TA_Base_App::CCFDInstrumentBarInfoCalculator::HandleNewBar, this, _1, _2);
	m_pBarCalculator->onBarUpdate = boost::bind(&TA_Base_App::CCFDInstrumentBarInfoCalculator::HandleUpdateBar, this, _1, _2);
	
	//TODO.
	m_pBarCalculator->addBar(TIME_BASE_S_1MIN);//seconds

}

CCFDInstrumentBarInfoCalculator::~CCFDInstrumentBarInfoCalculator(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pMapTimeBarInfo)
	{
		_ClearDataInMap(m_pMapTimeBarInfo);
		m_pMapTimeBarInfo->clear();
		delete m_pMapTimeBarInfo;
		m_pMapTimeBarInfo = NULL;
	}


	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}

	if (NULL != m_pStorager)
	{
		delete m_pStorager;
		m_pStorager = NULL;
	}

	LOG_DEBUG<<"begin delete cfd m_pBarCalculator";
	if (NULL != m_pBarCalculator)
	{
		delete m_pBarCalculator;
		m_pBarCalculator = NULL;
	}
	LOG_DEBUG<<"end delete cfd m_pBarCalculator";



}


int CCFDInstrumentBarInfoCalculator::_ClearDataInMap(MapIntervalBarInfoT*  pMapTimeBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::string strLogInfo;
	MapIntervalBarInfoIterT  iterMap;
	int nInterval = 0;
	Bar* pBar = NULL;
	std::ostringstream sreaamTmp;

	LOG_DEBUG<<"Last Save cfd bar m_pMapTimeBarInfo.size="<<m_pMapTimeBarInfo->size();

	//save last Data
	iterMap = m_pMapTimeBarInfo->begin();
	while(iterMap != m_pMapTimeBarInfo->end())
	{
		nInterval = (iterMap->first);
		pBar = (iterMap->second);
		//log bar
		sreaamTmp.str("");
		sreaamTmp<<"last save cfd bar m_nInstrumentIDFirest="<<m_nInstrumentIDFirest<<"m_nInstrumentIDSecond="<<m_nInstrumentIDSecond<<" ";
		strLogInfo = sreaamTmp.str();
		m_pUtilityFun->logBarInfo(strLogInfo, nInterval, pBar);
		//save to db
		m_pStorager->storeBarInfo(nInterval, pBar);

		delete pBar;
		pBar = NULL;

		iterMap++;
	}
	m_pMapTimeBarInfo->clear();

	return nFunRes;
}

void CCFDInstrumentBarInfoCalculator::HandleNewBar(int interval, const Bar &bar)
{
	BOOST_LOG_FUNCTION();

	std::string strLogInfo;
	std::ostringstream sreaamTmp;

	MapIntervalBarInfoIterT  iterMap;
	Bar* pBar = NULL;
	iterMap = m_pMapTimeBarInfo->find(interval);
	if (iterMap != m_pMapTimeBarInfo->end())
	{
		//find ok
		pBar = (iterMap->second);
		//log bar
		sreaamTmp.str("");
		sreaamTmp<<"save cfd bar m_nInstrumentIDFirest="<<m_nInstrumentIDFirest<<"m_nInstrumentIDSecond="<<m_nInstrumentIDSecond<<" ";
		strLogInfo=sreaamTmp.str();
		m_pUtilityFun->logBarInfo(strLogInfo, interval, pBar);
		//save to db
		m_pStorager->storeBarInfo(interval, pBar);

		//reset bar
		pBar->update(0, 0, 0);
		(*pBar) = bar;

		//log bar
		sreaamTmp.str("");
		sreaamTmp<<"reuse cfd bar m_nInstrumentIDFirest="<<m_nInstrumentIDFirest<<"m_nInstrumentIDSecond="<<m_nInstrumentIDSecond<<" ";
		strLogInfo=sreaamTmp.str();
		m_pUtilityFun->logBarInfo(strLogInfo, interval, pBar);
	}
	else
	{
		//not find
		pBar = new Bar();
		m_pMapTimeBarInfo->insert(MapIntervalBarInfoValueTypeT(interval, pBar));
		pBar->update(0, 0, 0);
		(*pBar) = bar;

		sreaamTmp.str("");
		sreaamTmp<<"new cfd bar m_nInstrumentIDFirest="<<m_nInstrumentIDFirest<<"m_nInstrumentIDSecond="<<m_nInstrumentIDSecond<<" ";
		strLogInfo=sreaamTmp.str();
		m_pUtilityFun->logBarInfo(strLogInfo, interval, pBar);
	}

}



void CCFDInstrumentBarInfoCalculator::HandleUpdateBar(int interval, const Bar &bar)
{
	std::string strLogInfo;

	MapIntervalBarInfoIterT  iterMap;
	Bar* pBar = NULL;
	iterMap = m_pMapTimeBarInfo->find(interval);
	if (iterMap != m_pMapTimeBarInfo->end())
	{
		//find ok
		pBar = (iterMap->second);
		//log bar
		strLogInfo="UpdateBar cfd bar";
		m_pUtilityFun->logBarInfo(strLogInfo, interval, pBar);
		//reset bar
		(*pBar) = bar;
	}
	else
	{
		//find error
		LOG_ERROR<<"HandleNewBar must call first"<<" "<<"interval="<<interval;
	}
}

int CCFDInstrumentBarInfoCalculator::updateMarketData( CCFDInstrumentBarInfo* pCFDBarInfo )
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	if (NULL == pCFDBarInfo)
	{
		nFunRes = -1;
		return nFunRes;
	}

	//use pCFDBarInfo->m_CFDBarInfo.High  TODO.
	if (NULL != m_pBarCalculator)
	{
		m_pBarCalculator->onMarketDataUpdate(
			pCFDBarInfo->m_CFDBarInfo.Time, 
			pCFDBarInfo->m_CFDBarInfo.High, 
			pCFDBarInfo->m_CFDBarInfo.Volume);
	}

	return nFunRes;
}



NS_END(TA_Base_App) 



