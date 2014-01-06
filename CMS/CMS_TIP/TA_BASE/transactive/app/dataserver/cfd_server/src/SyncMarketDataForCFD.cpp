#include "SyncMarketDataForCFD.h"
#include "CFDServerUtilityFun.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CSyncMarketDataForCFD::CSyncMarketDataForCFD(void)
{
	BOOST_LOG_FUNCTION();
	m_nInterval = 0;// = 5;//second
	m_nCFDInstrumentID = 0;// = 3620*3521;
	m_pUtilityFun = new CCFDServerUtilityFun();
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
	//////////////////////////////////////////////////////////////////////////

	_SyncSingleBarInfo(nBarInfoFirst, nBarInfoSecond, *pLstBarInfoFirst, *pLstBarInfoSecond);

	_SyncLstCFDBarInfo(*pLstBarInfoFirst, *pLstBarInfoSecond, lstCFDbarInfo);

	//////////////////////////////////////////////////////////////////////////
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
	Bar* pNewBar = NULL;
	//TODO.
	if (nBarInfoFirst.Time == nBarInfoSecond.Time)
	{
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
	}

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

		pCFDInstrumentBarInfoTmp->setCFDInstrumentID(m_nCFDInstrumentID);
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

void CSyncMarketDataForCFD::setCFDInstrumentID(int nCFDInstrumentID)
{
	BOOST_LOG_FUNCTION();

	m_nCFDInstrumentID = nCFDInstrumentID;// = 3620*3521;
}






NS_END(TA_Base_App)
















