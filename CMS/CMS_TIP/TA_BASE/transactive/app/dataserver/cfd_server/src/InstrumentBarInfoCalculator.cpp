#include "InstrumentBarInfoCalculator.h"

#include "core/utilities/src/UtilitiesCommonData.h"

#include "MarketData.h"
#include "BarCalculator.h"
#include "InsteumentBarInfoStorager.h"
#include "CFDServerUtilityFun.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_Base_Core;

NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////
CInstrumentBarInfoCalculator::CInstrumentBarInfoCalculator(unsigned int nInstrumentID)
{	
	BOOST_LOG_FUNCTION();
	m_nInstrumentID = nInstrumentID;
	m_pMapTimeBarInfo = new MapIntervalBarInfoT();
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_pStorager = new CInstrumentBarInfoStorager(m_nInstrumentID);

	m_pBarCalculator = new BarCalculator(m_nInstrumentID);

	m_pBarCalculator->onNewBar = boost::bind(&TA_Base_App::CInstrumentBarInfoCalculator::HandleNewBar, this, _1, _2);
	m_pBarCalculator->onBarUpdate = boost::bind(&TA_Base_App::CInstrumentBarInfoCalculator::HandleUpdateBar, this, _1, _2);
	
	//TODO.
	m_pBarCalculator->addBar(TIME_BASE_S_5S);//seconds
	m_pBarCalculator->addBar(TIME_BASE_S_1MIN);//seconds
	m_pBarCalculator->addBar(TIME_BASE_S_5MIN);//seconds
	m_pBarCalculator->addBar(TIME_BASE_S_30MIN);//seconds
	m_pBarCalculator->addBar(TIME_BASE_S_1HOUR);//seconds
	m_pBarCalculator->addBar(TIME_BASE_S_1DAY);//seconds


	//5s
	//1m
	//5m
	//30m
	//60
	//1d

}

CInstrumentBarInfoCalculator::~CInstrumentBarInfoCalculator(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pMapTimeBarInfo)
	{
		_ClearDataInMap(m_pMapTimeBarInfo);
		m_pMapTimeBarInfo->clear();
		delete m_pMapTimeBarInfo;
		m_pMapTimeBarInfo = NULL;
	}

	LOG_DEBUG<<"begin delete m_pBarCalculator";
	if (NULL != m_pBarCalculator)
	{
		delete m_pBarCalculator;
		m_pBarCalculator = NULL;
	}
	LOG_DEBUG<<"end delete m_pBarCalculator";


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

}


int CInstrumentBarInfoCalculator::_ClearDataInMap(MapIntervalBarInfoT*  pMapTimeBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::string strLogInfo;
	MapIntervalBarInfoIterT  iterMap;
	int nInterval = 0;
	Bar* pBar = NULL;
	std::ostringstream sreaamTmp;

	LOG_DEBUG<<"Last Save bar m_pMapTimeBarInfo.size="<<m_pMapTimeBarInfo->size();

	//save last Data
	iterMap = m_pMapTimeBarInfo->begin();
	while(iterMap != m_pMapTimeBarInfo->end())
	{
		nInterval = (iterMap->first);
		pBar = (iterMap->second);
		//log bar
		sreaamTmp.str("");
		sreaamTmp<<"last save bar InstrumentID="<<m_nInstrumentID<<" ";
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

void CInstrumentBarInfoCalculator::HandleNewBar(int interval, const Bar &bar)
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
		sreaamTmp<<"save bar InstrumentID="<<m_nInstrumentID<<" ";
		strLogInfo=sreaamTmp.str();
		m_pUtilityFun->logBarInfo(strLogInfo, interval, pBar);
		//save to db
		m_pStorager->storeBarInfo(interval, pBar);

		//reset bar
		pBar->update(0, 0, 0);
		(*pBar) = bar;

		//log bar
		sreaamTmp.str("");
		sreaamTmp<<"reuse bar InstrumentID="<<m_nInstrumentID<<" ";
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
		sreaamTmp<<"new bar for InstrumentID="<<m_nInstrumentID<<" ";
		strLogInfo=sreaamTmp.str();
		m_pUtilityFun->logBarInfo(strLogInfo, interval, pBar);
	}

}



void CInstrumentBarInfoCalculator::HandleUpdateBar(int interval, const Bar &bar)
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
		strLogInfo="UpdateBar bar";
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



int CInstrumentBarInfoCalculator::updateMarketData(const MarketData& marketData)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	if (NULL != m_pBarCalculator)
	{
		m_pBarCalculator->onMarketDataUpdate(marketData);
	}

	return nFunRes;
}



NS_END(TA_Base_App) 



