#include "MarketDataDispatcher.h"
#include <QtSql/QSqlDatabase>

#include "InstrumentBarInfoCalculator.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

extern boost::mutex g_mutex_Database_Mysql;
extern bool g_bool_NeedToCheckAndInditMysqlDb;
extern QSqlDatabase*	g_SqlDataBase_Mysql;

NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////
CMarketDataDispatcher::CMarketDataDispatcher(void)
{	
	BOOST_LOG_FUNCTION();
	m_mapInstrumentIDBarInfoCalc = new MapInstrumentIDBarInfoCalcT();
	
}

CMarketDataDispatcher::~CMarketDataDispatcher(void)
{
	BOOST_LOG_FUNCTION();
	
	if (NULL != m_mapInstrumentIDBarInfoCalc)
	{
		_ClearDataInMap(m_mapInstrumentIDBarInfoCalc);
		m_mapInstrumentIDBarInfoCalc->clear();
		delete 	m_mapInstrumentIDBarInfoCalc;
		m_mapInstrumentIDBarInfoCalc = NULL;
	}

	if (enumMysqlDb ==  m_InstrumentBarInfoRequest.m_nDBType)
	{
		boost::mutex::scoped_lock lock(g_mutex_Database_Mysql);
		if (NULL != g_SqlDataBase_Mysql)
		{
			delete g_SqlDataBase_Mysql;
			g_SqlDataBase_Mysql = NULL;

			LOG_INFO<<"delete Database QMYSQL";
		}
		g_bool_NeedToCheckAndInditMysqlDb = true;

	}

}

int CMarketDataDispatcher::_ClearDataInMap(MapInstrumentIDBarInfoCalcT*  pMapInstrumentIDBarInfoCalc)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	MapInstrumentIDBarInfoCalcIterT  iterMap;
	CInstrumentBarInfoCalculator* pBarInfoCalc = NULL;

	iterMap = m_mapInstrumentIDBarInfoCalc->begin();
	while(iterMap != m_mapInstrumentIDBarInfoCalc->end())
	{
		pBarInfoCalc = (iterMap->second);

		delete pBarInfoCalc;
		pBarInfoCalc = NULL;

		iterMap++;
	}
	m_mapInstrumentIDBarInfoCalc->clear();

	return nFunRes;
}

int CMarketDataDispatcher::dispatcherMarketData(const MarketData& marketData)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	unsigned int nSecurityIDValue = 0;
	MapInstrumentIDBarInfoCalcIterT  iterMap;
	CInstrumentBarInfoCalculator* pBarInfoCalc = NULL;

	nSecurityIDValue = marketData.getSecurityID();

	iterMap = m_mapInstrumentIDBarInfoCalc->begin();
	iterMap = m_mapInstrumentIDBarInfoCalc->find(nSecurityIDValue);
	if (iterMap != m_mapInstrumentIDBarInfoCalc->end())
	{
		//find ok
		pBarInfoCalc = (iterMap->second);
		nFunRes = pBarInfoCalc->onMarketDataUpdateForTick(marketData);
		nFunRes = pBarInfoCalc->onMarketDataUpdateForBar(marketData);
	}
	else
	{
		//not find
		unsigned int nInstrumentID = nSecurityIDValue;		
		pBarInfoCalc = new CInstrumentBarInfoCalculator(nInstrumentID, m_InstrumentBarInfoRequest);
		m_mapInstrumentIDBarInfoCalc->insert(MapInstrumentIDBarInfoCalcValueTypeT(nInstrumentID, pBarInfoCalc));
		nFunRes = pBarInfoCalc->onMarketDataUpdateForTick(marketData);
		nFunRes = pBarInfoCalc->onMarketDataUpdateForBar(marketData);
	}

	return nFunRes;
}

void CMarketDataDispatcher::setInstrumentBarInfoRequest( const CInstrumentBarInfoRequest& instrumentBarInfoRequest )
{
	BOOST_LOG_FUNCTION();
	m_InstrumentBarInfoRequest = instrumentBarInfoRequest;
}


NS_END(TA_Base_App) 



