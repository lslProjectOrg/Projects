#include "SyncMarketDataForCFD.h"
#include "CFDServerUtilityFun.h"
#include "CFDInstrumentBarInfo.h"
#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CSyncMarketDataForCFD::CSyncMarketDataForCFD(void)
{
	BOOST_LOG_FUNCTION();
	m_nInterval = 0;// = 5;//second
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_nCFDInstrumentIDFirst = 0;//
	m_nCFDInstrumentIDSecond = 0;//
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



int CSyncMarketDataForCFD::syncSingleCFDBarInfo(const Bar& nBarInfoFirst, const Bar& nBarInfoSecond, LstCFDBarInfoT& lstCFDbarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	LstBarInfoT* pLstBarInfoFirst = NULL;
	LstBarInfoT* pLstBarInfoSecond = NULL;

	pLstBarInfoFirst = new LstBarInfoT();
	pLstBarInfoSecond = new LstBarInfoT();
	pLstBarInfoFirst->clear();
	pLstBarInfoSecond->clear();

	_SyncSingleBarInfo(nBarInfoFirst, nBarInfoSecond, *pLstBarInfoFirst, *pLstBarInfoSecond);

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

int CSyncMarketDataForCFD::_SyncSingleBarInfo(const Bar& nBarInfoFirst, const Bar& nBarInfoSecond, LstBarInfoT& lstBarInfoFirst, LstBarInfoT& lstBarInfoSecond)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	enSynType nSynType = SynType_BEGIN;
	//TODO.


	if (nBarInfoFirst.Time == nBarInfoSecond.Time)
	{
		nSynType = SynType_EQUAL;
	}	
	else if (nBarInfoFirst.Time < nBarInfoSecond.Time)
	{
		nSynType = SynType_SMALL;
	}
	else if (nBarInfoFirst.Time > nBarInfoSecond.Time)
	{
		nSynType = SynType_BIGGER;
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
	const Bar& nBarInfoFirst, 
	const Bar& nBarInfoSecond, 
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
	const Bar& nBarInfoFirst, 
	const Bar& nBarInfoSecond, 
	LstBarInfoT& lstBarInfoFirst, 
	LstBarInfoT& lstBarInfoSecond)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	Bar* pNewBar = NULL;
	
	if (!(nBarInfoFirst.Time == nBarInfoSecond.Time))
	{
		nFunRes = -1;
		return nFunRes;
	}

	pNewBar = NULL;
	pNewBar = new Bar();
	*pNewBar = nBarInfoFirst;
	lstBarInfoFirst.push_back(pNewBar);
	pNewBar = NULL;

	pNewBar = NULL;
	pNewBar = new Bar();
	*pNewBar = nBarInfoSecond;
	lstBarInfoSecond.push_back(pNewBar);
	pNewBar = NULL;

	return nFunRes;
}



int CSyncMarketDataForCFD::_SyncSingleBarInfoSynTypeSmall(
	enSynType nSynType,
	const Bar& nBarInfoFirst, 
	const Bar& nBarInfoSecond, 
	LstBarInfoT& lstBarInfoFirst, 
	LstBarInfoT& lstBarInfoSecond)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	Bar* pNewBar = NULL;

	if (!(nBarInfoFirst.Time < nBarInfoSecond.Time))
	{
		nFunRes = -1;
		return nFunRes;
	}

	//syn small
	pNewBar = NULL;
	pNewBar = new Bar();
	*pNewBar = nBarInfoFirst;
	lstBarInfoFirst.push_back(pNewBar);
	pNewBar = NULL;

	
	pNewBar = NULL;
	pNewBar = new Bar();
	*pNewBar = nBarInfoFirst;
	pNewBar->Time = nBarInfoSecond.Time;
	lstBarInfoFirst.push_back(pNewBar);
	pNewBar = NULL;

	//syn biger
	pNewBar = NULL;
	pNewBar = new Bar();
	*pNewBar = nBarInfoSecond;
	pNewBar->Time = nBarInfoFirst.Time;
	lstBarInfoSecond.push_back(pNewBar);
	pNewBar = NULL;


	//syn biger
	pNewBar = NULL;
	pNewBar = new Bar();
	*pNewBar = nBarInfoSecond;
	lstBarInfoSecond.push_back(pNewBar);
	pNewBar = NULL;

	return nFunRes;
}


int CSyncMarketDataForCFD::_ClearBarInfoList(LstBarInfoT& lstBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	LstBarInfoIterT iterLst;
	Bar* pData = NULL;

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
	Bar* pBarFirst = NULL;
	Bar* pBarSecond = NULL;
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

		pCFDInstrumentBarInfoTmp =new CCFDInstrumentBarInfo();

		pCFDInstrumentBarInfoTmp->setCFDInstrumentIDFirst(m_nCFDInstrumentIDFirst);
		pCFDInstrumentBarInfoTmp->setCFDInstrumentIDSecond(m_nCFDInstrumentIDSecond);
		pCFDInstrumentBarInfoTmp->setInterval(m_nInterval);
		pCFDInstrumentBarInfoTmp->setBarInfoFirst(*pBarFirst);
		pCFDInstrumentBarInfoTmp->setBarInfoSecond(*pBarSecond);
		nFunRes = pCFDInstrumentBarInfoTmp->buidCFDBarInfo();
		if (0 != nFunRes)
		{
			LOG_ERROR<<"buildCFDMarketData DataFirst.Time != DataSecond.Time"
				<<" ["<<"DataFirst.Time="<<m_pUtilityFun->dataTimeToStr(pBarFirst->Time)<<"]"
				<<" ["<<"DataSecond.Time="<<m_pUtilityFun->dataTimeToStr(pBarSecond->Time)<<"]"; 
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

void CSyncMarketDataForCFD::setInterval(int nInterval)
{
	BOOST_LOG_FUNCTION();

	m_nInterval = nInterval;// = 5;//second
}

void CSyncMarketDataForCFD::setCFDInstrumentIDFirst(unsigned int nCFDInstrumentIDFirst)
{
	BOOST_LOG_FUNCTION();

	m_nCFDInstrumentIDFirst = nCFDInstrumentIDFirst;
}


void CSyncMarketDataForCFD::setCFDInstrumentIDSecond(unsigned int nCFDInstrumentIDSecond)
{
	BOOST_LOG_FUNCTION();

	m_nCFDInstrumentIDSecond = nCFDInstrumentIDSecond;
}




NS_END(TA_Base_App)
















