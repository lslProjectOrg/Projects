#include "SyncMarketDataForCFD.h"
#include "CFDServerUtilityFun.h"
#include "CFDInstrumentBarInfo.h"
#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CSyncMarketDataForCFD::CSyncMarketDataForCFD(const CCFDRequest& cfdRequest)
{
	BOOST_LOG_FUNCTION();
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_CFDRequest = cfdRequest;
}

CSyncMarketDataForCFD::~CSyncMarketDataForCFD( void )
{
	BOOST_LOG_FUNCTION();
	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}
}



int CSyncMarketDataForCFD::syncSingleCFDBarInfo(
	const MarketData& nBarInfoFirst, 
	const MarketData& nBarInfoSecond,
	LstCFDBarInfoT& lstCFDbarInfo,
	enNextWorkType& nNextWorkType)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	LstBarInfoT* pLstBarInfoFirst = NULL;
	LstBarInfoT* pLstBarInfoSecond = NULL;

	pLstBarInfoFirst = new LstBarInfoT();
	pLstBarInfoSecond = new LstBarInfoT();
	pLstBarInfoFirst->clear();
	pLstBarInfoSecond->clear();

	_SyncSingleBarInfo(nBarInfoFirst, nBarInfoSecond, *pLstBarInfoFirst, *pLstBarInfoSecond, nNextWorkType);

	_SyncLstCFDBarInfo(*pLstBarInfoFirst, *pLstBarInfoSecond, lstCFDbarInfo);

	if (NULL != pLstBarInfoFirst)
	{
		_ClearBarInfoList(*pLstBarInfoFirst);
		pLstBarInfoFirst->clear();
		delete pLstBarInfoFirst;
		pLstBarInfoFirst = NULL;
	}

	if (NULL != pLstBarInfoSecond)
	{
		_ClearBarInfoList(*pLstBarInfoSecond);
		pLstBarInfoSecond->clear();
		delete pLstBarInfoSecond;
		pLstBarInfoSecond = NULL;
	}

	return nFunRes;
}

int CSyncMarketDataForCFD::clearCFDBarInfoList(LstCFDBarInfoT& lstCFDbarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	LstCFDBarInfoIterT iterLst;
	CCFDInstrumentBarInfo* pData = NULL;

	iterLst = lstCFDbarInfo.begin();
	while (iterLst != lstCFDbarInfo.end())
	{
		pData = (*iterLst);

		delete pData;
		pData = NULL;

		iterLst++;
	}
	lstCFDbarInfo.clear();

	return nFunRes;
}

int CSyncMarketDataForCFD::_SyncSingleBarInfo(
	const MarketData& nBarInfoFirst, 
	const MarketData& nBarInfoSecond, 
	LstBarInfoT& lstBarInfoFirst, 
	LstBarInfoT& lstBarInfoSecond,
	enNextWorkType& nNextWorkType)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	enSynType nSynType = SynType_BEGIN;
	unsigned int nFirstInstrumentTime = 0;
	unsigned int nSecondInstrumentTime = 0;

	nFirstInstrumentTime = nBarInfoFirst.getTime();
	nSecondInstrumentTime = nBarInfoSecond.getTime();

	if (nFirstInstrumentTime == nSecondInstrumentTime)
	{
		nSynType = SynType_EQUAL;
		nNextWorkType = NextWorkType_UseNewFirst_UseNewSecond;
	}	
	else if (nFirstInstrumentTime < nSecondInstrumentTime)
	{
		nSynType = SynType_SMALL;
		nNextWorkType = NextWorkType_UseNewFirst_ReUseSecond;
	}
	else if (nFirstInstrumentTime > nSecondInstrumentTime)
	{
		nSynType = SynType_BIGGER;
		nNextWorkType = NextWorkType_ReUseFirst_UseNewSecond;
	}

	nFunRes = _SyncSingleBarInfoSynType(
		nSynType,
		nBarInfoFirst,
		nBarInfoSecond,
		lstBarInfoFirst,
		lstBarInfoSecond);


	return nFunRes;
}


int CSyncMarketDataForCFD::_SyncSingleBarInfoSynType(
	enSynType nSynType,
	const MarketData& nBarInfoFirst, 
	const MarketData& nBarInfoSecond, 
	LstBarInfoT& lstBarInfoFirst, 
	LstBarInfoT& lstBarInfoSecond)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	switch (nSynType)
	{
	case SynType_EQUAL:
		nFunRes = _SyncSingleBarInfoSynTypeEqual(nSynType, nBarInfoFirst, nBarInfoSecond, lstBarInfoFirst, lstBarInfoSecond);
		break;
	case SynType_SMALL:
		nFunRes = _SyncSingleBarInfoSynTypeSmall(nSynType, nBarInfoFirst, nBarInfoSecond, lstBarInfoFirst, lstBarInfoSecond);		
		break;
	case SynType_BIGGER:
		nFunRes = _SyncSingleBarInfoSynTypeSmall(nSynType, nBarInfoSecond, nBarInfoFirst, lstBarInfoSecond, lstBarInfoFirst);		
		break;
	default:
		nFunRes = -1;
		break;
	}

	return nFunRes;
}


int CSyncMarketDataForCFD::_SyncSingleBarInfoSynTypeEqual(
	enSynType nSynType,
	const MarketData& nBarInfoFirst, 
	const MarketData& nBarInfoSecond, 
	LstBarInfoT& lstBarInfoFirst, 
	LstBarInfoT& lstBarInfoSecond)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	MarketData* pNewBar = NULL;
	unsigned int nFirstInstrumentTime = 0;
	unsigned int nSecondInstrumentTime = 0;

	nFirstInstrumentTime = nBarInfoFirst.getTime();
	nSecondInstrumentTime = nBarInfoSecond.getTime();
	
	if (!(nFirstInstrumentTime == nSecondInstrumentTime))
	{
		nFunRes = -1;
		return nFunRes;
	}

	pNewBar = NULL;
	pNewBar = new MarketData();
	*pNewBar = nBarInfoFirst;
	lstBarInfoFirst.push_back(pNewBar);
	pNewBar = NULL;

	pNewBar = NULL;
	pNewBar = new MarketData();
	*pNewBar = nBarInfoSecond;
	lstBarInfoSecond.push_back(pNewBar);
	pNewBar = NULL;

	return nFunRes;
}



int CSyncMarketDataForCFD::_SyncSingleBarInfoSynTypeSmall(
	enSynType nSynType,
	const MarketData& nBarInfoFirst, 
	const MarketData& nBarInfoSecond, 
	LstBarInfoT& lstBarInfoFirst, 
	LstBarInfoT& lstBarInfoSecond)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	MarketData* pNewBar = NULL;
	unsigned int nFirstInstrumentTime = 0;
	unsigned int nSecondInstrumentTime = 0;

	nFirstInstrumentTime = nBarInfoFirst.getTime();
	nSecondInstrumentTime = nBarInfoSecond.getTime();

	if (!(nFirstInstrumentTime < nSecondInstrumentTime))
	{
		nFunRes = -1;
		return nFunRes;
	}

	//syn small
	pNewBar = NULL;
	pNewBar = new MarketData();
	*pNewBar = nBarInfoFirst;
	lstBarInfoFirst.push_back(pNewBar);
	pNewBar = NULL;

	//syn biger
	pNewBar = NULL;
	pNewBar = new MarketData();
	*pNewBar = nBarInfoSecond;//copy price
	//pNewBar->Time = nBarInfoFirst.Time;
	lstBarInfoSecond.push_back(pNewBar);
	pNewBar = NULL;

	return nFunRes;
}


int CSyncMarketDataForCFD::_ClearBarInfoList(LstBarInfoT& lstBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	LstBarInfoIterT iterLst;
	MarketData* pData = NULL;

	iterLst = lstBarInfo.begin();
	while (iterLst != lstBarInfo.end())
	{
		pData = (*iterLst);

		delete pData;
		pData = NULL;

		iterLst++;
	}
	lstBarInfo.clear();
	return nFunRes;
}

int CSyncMarketDataForCFD::_SyncLstCFDBarInfo(LstBarInfoT& lstBarInfoFirst, LstBarInfoT& lstBarInfoSecond, LstCFDBarInfoT& lstCFDBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	LstBarInfoIterT iterLstFirst;
	LstBarInfoIterT iterLstSecond;
	MarketData* pBarFirst = NULL;
	MarketData* pBarSecond = NULL;
	CCFDInstrumentBarInfo* pCFDInstrumentBarInfoTmp = NULL;

	if (lstBarInfoFirst.size() != lstBarInfoSecond.size())
	{
		LOG_ERROR<<"_SyncLstCFDBarInfo size error!"
			<<" "<<"lstBarInfoFirst.size()="<<lstBarInfoFirst.size()
			<<" "<<"lstBarInfoSecond.size()="<<lstBarInfoSecond.size();
		nFunRes = -1;
		return nFunRes;
	}

	iterLstFirst = lstBarInfoFirst.begin();
	iterLstSecond = lstBarInfoSecond.begin();

	while (
		(iterLstFirst != lstBarInfoFirst.end())
		&& (iterLstSecond != lstBarInfoSecond.end())
		)
	{
		pBarFirst = NULL;
		pBarSecond = NULL;
		pBarFirst = (*iterLstFirst);
		pBarSecond = (*iterLstSecond);

		pCFDInstrumentBarInfoTmp =new CCFDInstrumentBarInfo(m_CFDRequest);

		pCFDInstrumentBarInfoTmp->setCFDRequest(m_CFDRequest);
		pCFDInstrumentBarInfoTmp->setBarInfoFirst(*pBarFirst);
		pCFDInstrumentBarInfoTmp->setBarInfoSecond(*pBarSecond);
		nFunRes = pCFDInstrumentBarInfoTmp->buidCFDBarInfo();
		if (0 != nFunRes)
		{
			LOG_ERROR<<"buildCFDMarketData DataFirst.Time != DataSecond.Time"
				<<" ["<<"DataFirst.Time="<<m_pUtilityFun->dataTimeToStr(pBarFirst->getTime())<<"]"
				<<" ["<<"DataSecond.Time="<<m_pUtilityFun->dataTimeToStr(pBarSecond->getTime())<<"]"; 
		}
		else
		{
			pCFDInstrumentBarInfoTmp->logInfo();
		}


		lstCFDBarInfo.push_back(pCFDInstrumentBarInfoTmp);
		pCFDInstrumentBarInfoTmp = NULL;

		iterLstFirst++;
		iterLstSecond++;
	}//while

	return nFunRes;
}


void CSyncMarketDataForCFD::setCFDRequest(const CCFDRequest& cfdRequest)
{
	BOOST_LOG_FUNCTION();

	m_CFDRequest = cfdRequest;
}






NS_END(TA_Base_App)
















