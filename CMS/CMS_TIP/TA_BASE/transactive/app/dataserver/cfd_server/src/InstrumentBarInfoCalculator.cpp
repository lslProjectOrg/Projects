#include "CFDServerCommonData.h"
#include "InstrumentBarInfoCalculator.h"
#include "MarketData.h"
#include "BarCalculator.h"
#include "CFDServerUtilityFun.h"
#include "DBOperParam.h"
#include "DataArrayBuffer.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_Base_App;

NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////
CInstrumentBarInfoCalculator::CInstrumentBarInfoCalculator(unsigned int nInstrumentID, const CInstrumentBarInfoRequest& instrumentBarInfoRequest)
{	
	BOOST_LOG_FUNCTION();
	m_nInstrumentID = nInstrumentID;
	m_InstrumentBarInfoRequest = instrumentBarInfoRequest;
	m_pMapTimeBarInfo = new MapIntervalBarInfoT();
	m_pUtilityFun = new CCFDServerUtilityFun();

	m_DBOperParam.setValue(m_InstrumentBarInfoRequest.m_nDBType, m_nInstrumentID);
	//D://SaveData//SaveDataBAR
	m_DBOperParam.setValueSqliteDbPathBAR(m_InstrumentBarInfoRequest.m_strSaveDataDirectoryBAR);
	m_DBOperParam.setValueSqliteDbPathTIK(m_InstrumentBarInfoRequest.m_strSaveDataDirectoryTIK);
	m_DBOperParam.logInfo();

	m_pStorager = new CDataArrayBuffer(m_DBOperParam, m_InstrumentBarInfoRequest);
	//BarCalculator second param use false
	m_pBarCalculator = new BarCalculator(m_nInstrumentID, false);
	_InitBarCalculator();
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

	LOG_DEBUG<<"begin delete m_pBarCalculator";
	if (NULL != m_pBarCalculator)
	{
		delete m_pBarCalculator;
		m_pBarCalculator = NULL;
	}
	LOG_DEBUG<<"end delete m_pBarCalculator";
}



int CInstrumentBarInfoCalculator::_InitBarCalculator()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::list<int> lstBarTime;
	std::list<int>::iterator  iterLst;
	int nSeconds = 0;
	
	m_pBarCalculator->onNewBar = boost::bind(&TA_Base_App::CInstrumentBarInfoCalculator::HandleNewBar, this, _1, _2);
	m_pBarCalculator->onBarUpdate = boost::bind(&TA_Base_App::CInstrumentBarInfoCalculator::HandleUpdateBar, this, _1, _2);

	m_InstrumentBarInfoRequest.getLstBarTime(lstBarTime);
	iterLst = lstBarTime.begin();

	while (iterLst != lstBarTime.end())
	{
		nSeconds = (*iterLst);
		m_pBarCalculator->addBar(nSeconds);//seconds

		iterLst++;
	}//while

	lstBarTime.clear();

	return nFunRes;
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

	LOG_INFO<<"Last Save bar m_pMapTimeBarInfo.size="<<m_pMapTimeBarInfo->size();

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
		m_pStorager->storeBarInfo(nInterval, *pBar);

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
		m_pStorager->storeBarInfo(interval, *pBar);

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



int CInstrumentBarInfoCalculator::onMarketDataUpdateForBar(const MarketData& marketData)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	if (NULL != m_pBarCalculator)
	{
		m_pBarCalculator->onMarketDataUpdate(marketData);
	}

	return nFunRes;
}


int CInstrumentBarInfoCalculator::onMarketDataUpdateForTick(const MarketData& marketData)
{
	int nFunRes = 0;
	Tick  newTick;

	if (TA_Base_App::enumMysqlDb == m_DBOperParam.m_nDataSrcDbType)
	{
		return nFunRes;
	}

	newTick.Time = marketData.getTime();

	newTick.BidPx = marketData.getBidPx(1);
	newTick.AskPx = marketData.getAskPx(1);
	newTick.LastPx = marketData.getPrice(MarketData::LAST_TRADED_PRICE);

	newTick.BidVol = marketData.getBidVol(1);
	newTick.AskVol = marketData.getAskVol(1);
	newTick.LastVol = marketData.getVolume(MarketData::LAST_TRADED_VOLUME);

	if (NULL != m_pStorager)
	{
		m_pStorager->storeTickInfo(newTick);
	}

	return nFunRes;
}

int CInstrumentBarInfoCalculator::storeMemoryDataToDB()
{
	int nFunRes = 0;
		
	nFunRes = m_pStorager->storeMemoryDataToDB();

	return nFunRes;
}

NS_END(TA_Base_App) 



