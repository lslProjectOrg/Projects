#include "DataArrayBuffer.h"

#include "DBOperParam.h"
#include "InstrumentBarInfoRequest.h"
#include "SqliteDbOper.h"
#include "MysqlDbOper.h"
#include "InstrumentBarInfo.h"
#include "InstrumentTickInfo.h"

#include <boost/algorithm/string.hpp>
#include <boost/math/special_functions/fpclassify.hpp> 

#include "BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_Base_App;


NS_BEGIN(TA_Base_App)

static const int DEF_VALUE_INT_InsertBatchSize = 10000;


CDataArrayBuffer::CDataArrayBuffer( const CDBOperParam& dbOperParam, const CInstrumentBarInfoRequest& barInfoRequest )
{
	BOOST_LOG_FUNCTION();

	m_nInsertBatchSize = DEF_VALUE_INT_InsertBatchSize;
	m_nInsertBatchIndex = 0;

	m_pDBOperParam = NULL;
	m_pBarInfoRequest = NULL;
	m_pDbOper = NULL;
	m_pMapIntervalBarLst = NULL;
	m_pLstInstrumentTickInfo = NULL;

	m_pDBOperParam = new CDBOperParam();
	m_pBarInfoRequest = new CInstrumentBarInfoRequest();

	*m_pDBOperParam = dbOperParam;
	*m_pBarInfoRequest = barInfoRequest;


	_InitMapIntervalBarLst();//m_pMapIntervalBarLst
	_InitLstInstrumentTickInfo();

	if (TA_Base_App::enumSqliteDb == m_pDBOperParam->m_nDataSrcDbType)
	{
		m_pDbOper = new CSqliteDbOper(*m_pDBOperParam);
	}
	else
	{
		m_pDbOper = new CMysqlDbOper(*m_pDBOperParam);
	}

	_InitDBTables();

}

CDataArrayBuffer::~CDataArrayBuffer(void)
{
	BOOST_LOG_FUNCTION();


	_UnInitMapIntervalBarLst();
	_UnInitLstInstrumentTickInfo();

	if (NULL != m_pDbOper)
	{
		delete m_pDbOper;
		m_pDbOper = NULL;
	}

	if (NULL != m_pDBOperParam)
	{
		delete m_pDBOperParam;
		m_pDBOperParam = NULL;
	}

}

int CDataArrayBuffer::storeBarInfo( int interval, Bar& barInfo )
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	MapIntervalBarLstIterT iterMap;
	CInstrumentBarInfo* pInstrumentBarInfo = NULL;
	LstInstrumentBarInfoT* pList = NULL;


	if ((boost::math::isnan)(barInfo.Open)
		||(boost::math::isnan)(barInfo.Close)
		||(boost::math::isnan)(barInfo.High)
		||(boost::math::isnan)(barInfo.Low)) 
	{
		//x is NaN
		return nFunRes;
	} 


	pInstrumentBarInfo = new CInstrumentBarInfo(interval, barInfo);

	iterMap = m_pMapIntervalBarLst->find(interval);
	if (iterMap != m_pMapIntervalBarLst->end())
	{
		pList = iterMap->second;
		pList->push_back(pInstrumentBarInfo);
		m_nInsertBatchIndex++;
	}
	else
	{
		LOG_ERROR<<"error in m_pDBOperParam.m_nInstrumentID="<<m_pDBOperParam->m_nInstrumentID
			<<" "<<"not find interval="<<interval<<" "<<"in m_pMapIntervalBarLst"
			<<" "<<"m_pMapIntervalBarLst.size="<<m_pMapIntervalBarLst->size();
		delete pInstrumentBarInfo;
		pInstrumentBarInfo = NULL;
	}

	_StoreData();

	return nFunRes;
}

int CDataArrayBuffer::storeTickInfo( Tick& tickInfo )
{
	int nFunRes = 0;
	CInstrumentTickInfo* pInstrumentTickInfo = NULL;

	if ((boost::math::isnan)(tickInfo.BidPx)
		||(boost::math::isnan)(tickInfo.AskPx)
		||(boost::math::isnan)(tickInfo.LastPx)) 
	{
		//x is NaN
		return nFunRes;
	} 

	pInstrumentTickInfo = new CInstrumentTickInfo(tickInfo);
	m_pLstInstrumentTickInfo->push_back(pInstrumentTickInfo);
	m_nInsertBatchIndex++;

	pInstrumentTickInfo = NULL;

	_StoreData();
	return nFunRes;
}

int CDataArrayBuffer::_StoreData()
{
	int nFunRes = 0;

	if (m_nInsertBatchIndex < m_nInsertBatchSize)
	{
		return nFunRes;
	}

	_StoreDataArrayBar(m_pMapIntervalBarLst);
	_StoreDataArrayTick(m_pLstInstrumentTickInfo);
	m_nInsertBatchIndex = 0;

	return nFunRes;
}




int CDataArrayBuffer::_InitMapIntervalBarLst()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::list<int> lstBarTime;
	std::list<int>::iterator  iterLst;
	int nInterval = 0;
	LstInstrumentBarInfoT* pLstInstrumentBarInfo = NULL;

	_UnInitMapIntervalBarLst();

	m_pMapIntervalBarLst = new MapIntervalBarLstT();
	m_pMapIntervalBarLst->clear();

	m_pBarInfoRequest->getLstBarTime(lstBarTime);
	iterLst = lstBarTime.begin();

	while (iterLst != lstBarTime.end())
	{
		nInterval = (*iterLst);
		pLstInstrumentBarInfo = new LstInstrumentBarInfoT();

		LOG_INFO<<"nInstrumentID="<<m_pDBOperParam->m_nInstrumentID<<" "<<"pMapIntervalBarLstT add nInterval="<<nInterval;
		
		m_pMapIntervalBarLst->insert(MapIntervalBarLstValueTypeT(nInterval, pLstInstrumentBarInfo));
		pLstInstrumentBarInfo = NULL;

		iterLst++;
	}//while

	lstBarTime.clear();

	return nFunRes;
}


int CDataArrayBuffer::_InitDBTables()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::list<int> lstBarTime;

	m_pBarInfoRequest->getLstBarTime(lstBarTime);
	m_pDbOper->checkAndInitDBTable(lstBarTime);

	lstBarTime.clear();

	return nFunRes;
}




int CDataArrayBuffer::_StoreDataArrayBar(MapIntervalBarLstT*  pMapIntervalBarLst)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	MapIntervalBarLstIterT iterMap;
	int nInterval = 0;
	LstInstrumentBarInfoT* pLstInstrumentBarInfo = NULL;
	int nDataCount = 0;

	nDataCount = 0;

	iterMap = pMapIntervalBarLst->begin();
	while (iterMap != pMapIntervalBarLst->end())
	{
		nInterval = (iterMap->first);
		pLstInstrumentBarInfo = (iterMap->second);
		nDataCount += pLstInstrumentBarInfo->size();
		iterMap++;
	}//while

	if (nDataCount <= 0)
	{
		nFunRes = 0;
		return nFunRes;
	}

	iterMap = pMapIntervalBarLst->begin();
	while (iterMap != pMapIntervalBarLst->end())
	{
		nInterval = (iterMap->first);
		pLstInstrumentBarInfo = (iterMap->second);
		nFunRes = m_pDbOper->transactionInsertDataBar(nInterval, pLstInstrumentBarInfo);
		_ClearDataArrayBar(pLstInstrumentBarInfo);//just clean data
		iterMap++;
	}//while

	return nFunRes;
}



int CDataArrayBuffer::_StoreDataArrayTick(LstInstrumentTickInfoT* pLstInstrumentTickInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	int nDataCount = 0;

	if (pLstInstrumentTickInfo->empty())
	{
		nDataCount = 0;
		return nFunRes;
	}

	//save data

	nFunRes = m_pDbOper->transactionInsertDataTick(pLstInstrumentTickInfo);
	_ClearDataArrayTick(pLstInstrumentTickInfo);//just clean data

	return nFunRes;
}


void CDataArrayBuffer::_ClearDataArrayBar(LstInstrumentBarInfoT* pLstInstrumentBarInfo)
{
	BOOST_LOG_FUNCTION();

	LstInstrumentBarInfoIterT iterLst;
	CInstrumentBarInfo* pData = NULL;

	iterLst = pLstInstrumentBarInfo->begin();
	while (iterLst != pLstInstrumentBarInfo->end())
	{
		pData = (*iterLst);

		delete pData;
		pData = NULL;

		iterLst++;
	}
	pLstInstrumentBarInfo->clear();

}


void CDataArrayBuffer::_ClearDataArrayTick(LstInstrumentTickInfoT* pLstInstrumentTickInfo)
{
	BOOST_LOG_FUNCTION();

	LstInstrumentTickInfoIterT iterLst;
	CInstrumentTickInfo* pData = NULL;

	iterLst = pLstInstrumentTickInfo->begin();
	while (iterLst != pLstInstrumentTickInfo->end())
	{
		pData = (*iterLst);

		delete pData;
		pData = NULL;

		iterLst++;
	}
	pLstInstrumentTickInfo->clear();

}


void CDataArrayBuffer::_UnInitMapIntervalBarLst()
{
	BOOST_LOG_FUNCTION();

	MapIntervalBarLstIterT iterMap;
	LstInstrumentBarInfoT* pLstInstrumentBarInfo = NULL;
	int nInterval;

	if (NULL == m_pMapIntervalBarLst)
	{
		return;
	}

	iterMap = m_pMapIntervalBarLst->begin();
	while (iterMap != m_pMapIntervalBarLst->end())
	{
		nInterval = (iterMap->first);

		pLstInstrumentBarInfo = (iterMap->second);

		//save last data
		m_pDbOper->transactionInsertDataBar(nInterval, pLstInstrumentBarInfo);

		_ClearDataArrayBar(pLstInstrumentBarInfo);

		pLstInstrumentBarInfo->clear();
		delete pLstInstrumentBarInfo;
		pLstInstrumentBarInfo = NULL;

		iterMap++;
	}
	m_pMapIntervalBarLst->clear();


	if (NULL != m_pMapIntervalBarLst)
	{
		delete m_pMapIntervalBarLst;
		m_pMapIntervalBarLst = NULL;
	}
}


int CDataArrayBuffer::_InitLstInstrumentTickInfo()
{
	int nFunRes = 0;

	_UnInitLstInstrumentTickInfo();
	m_pLstInstrumentTickInfo = new LstInstrumentTickInfoT();
	m_pLstInstrumentTickInfo->clear();

	return nFunRes;
}
void CDataArrayBuffer::_UnInitLstInstrumentTickInfo()
{

	LstInstrumentTickInfoIterT iterList;
	CInstrumentTickInfo* pTickInfo = NULL;

	if (NULL == m_pLstInstrumentTickInfo)
	{
		return;
	}

	//last save data
	m_pDbOper->transactionInsertDataTick(m_pLstInstrumentTickInfo);

	//free data
	iterList = m_pLstInstrumentTickInfo->begin();
	while (iterList != m_pLstInstrumentTickInfo->end())
	{
		pTickInfo = *iterList;

		delete pTickInfo;
		pTickInfo = NULL;

		iterList++;
	}

	if (NULL != m_pLstInstrumentTickInfo)
	{
		delete m_pLstInstrumentTickInfo;
		m_pLstInstrumentTickInfo = NULL;
	}

	
}

int CDataArrayBuffer::storeMemoryDataToDB()
{
	int nFunRes = 0;
	_StoreDataArrayBar(m_pMapIntervalBarLst);
	_StoreDataArrayTick(m_pLstInstrumentTickInfo);
	m_nInsertBatchIndex = 0;

	return nFunRes;
}

NS_END(TA_Base_App) 



