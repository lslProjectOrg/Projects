#include "CreateCFDInstrumentBarInfo.h"

#include "BarCalculator.h"

#include "CFDServerUtilityFun.h"
#include "CFDInstrumentBarInfo.h"
#include "SyncMarketDataForCFD.h"
#include "CFDInstrumentBarInfoCalculator.h"
#include "InstrumentBarInfoStorager.h"



#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CCreateCFDInstrumentBarInfo::CCreateCFDInstrumentBarInfo(unsigned int nInstrumentIDFirst, unsigned int nInstrumentIDSecond)
{
	BOOST_LOG_FUNCTION();
	m_nInstrumentIDFirst = nInstrumentIDFirst;
	m_nInstrumentIDSecond = nInstrumentIDSecond;
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_CSyncMarketDataForCFD = new CSyncMarketDataForCFD();
	m_pCFDBarInfoCalculator = new CCFDInstrumentBarInfoCalculator(m_nInstrumentIDFirst, m_nInstrumentIDSecond);
	
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


int CCreateCFDInstrumentBarInfo::_UpdateCFDMarketData(CSyncMarketDataForCFD::LstCFDBarInfoT&  lstCFDBarInfo)
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

void CCreateCFDInstrumentBarInfo::setInterval(int nInterval)
{
	BOOST_LOG_FUNCTION();

	m_nInterval = nInterval;// = 5;//second
}

void CCreateCFDInstrumentBarInfo::createCFDbarInfoByLst(LstBarInfoT& lstBarFirst, LstBarInfoT& lstBarSecond)
{
	BOOST_LOG_FUNCTION();
	CSyncMarketDataForCFD::LstCFDBarInfoT   lstCFDBarInfoTmp;
	CSyncMarketDataForCFD::enNextWorkType nNextWorkType;
	int interval = 5;//second
	int nFunRes = 0;

	//prepare data
	Bar BarInfoFirst;
	Bar BarInfoSecond;
	LstBarInfoIterT  iterLstFirst;
	LstBarInfoIterT  iterLstSecond;

	//get CFD bar
	m_CSyncMarketDataForCFD->setCFDInstrumentIDFirst(m_nInstrumentIDFirst);
	m_CSyncMarketDataForCFD->setCFDInstrumentIDSecond(m_nInstrumentIDSecond);
	m_CSyncMarketDataForCFD->setInterval(interval);


	iterLstFirst = lstBarFirst.begin();
	iterLstSecond = lstBarSecond.begin();

	while 
		(
		(iterLstFirst != lstBarFirst.end())
		|| (iterLstSecond != lstBarSecond.end())
		)
	{
		if (iterLstFirst != lstBarFirst.end())
		{
			BarInfoFirst = (*iterLstFirst);
		}
		if (iterLstSecond != lstBarSecond.end())
		{
			BarInfoSecond = (*iterLstSecond);
		}

		m_CSyncMarketDataForCFD->syncSingleCFDBarInfo(BarInfoFirst, BarInfoSecond, lstCFDBarInfoTmp, nNextWorkType);
		_UpdateCFDMarketData(lstCFDBarInfoTmp);
		m_CSyncMarketDataForCFD->clearCFDBarInfoList(lstCFDBarInfoTmp);

		if (nNextWorkType == CSyncMarketDataForCFD::NextWorkType_UseNewFirst_UseNewSecond)
		{
			if (iterLstFirst != lstBarFirst.end())
			{
				iterLstFirst++;
			}
			if (iterLstSecond != lstBarSecond.end())
			{
				iterLstSecond++;
			}
		}
		else if (nNextWorkType == CSyncMarketDataForCFD::NextWorkType_UseNewFirst_ReUseSecond)
		{
			if (iterLstFirst != lstBarFirst.end())
			{
				iterLstFirst++;
			}
			else
			{
				//last data
				BarInfoFirst.Time = BarInfoSecond.Time;
			}
		}
		else if (nNextWorkType == CSyncMarketDataForCFD::NextWorkType_ReUseFirst_UseNewSecond)
		{
			if (iterLstSecond != lstBarSecond.end())
			{
				iterLstSecond++;
			}
			else
			{
				//last data
				BarInfoSecond.Time = BarInfoFirst.Time;
			}
		}

	}//while
}




int CCreateCFDInstrumentBarInfo::_InitInstrumentDataSource(unsigned int nInstrumentID, int nInterval, CInstrumentBarInfoStorager** ppStorager)
{
	int nFunRes = 0;

	if (NULL == ppStorager)
	{
		nFunRes = -1;
		return nFunRes;			 
	}
	if (NULL != *ppStorager)
	{
		_UnInitInstrumentDataSource(nInstrumentID, nInterval, ppStorager);
		(*ppStorager) = NULL;
	}
	(*ppStorager) = new CInstrumentBarInfoStorager(nInstrumentID);
	nFunRes = (*ppStorager)->beginGetBarInfo(nInstrumentID, nInterval);
	if (0 != nFunRes)
	{
		_UnInitInstrumentDataSource(nInstrumentID, nInterval, ppStorager);
		(*ppStorager) = NULL;
		nFunRes = -1;
		return nFunRes;
	}
	return nFunRes;
}
int CCreateCFDInstrumentBarInfo::_UnInitInstrumentDataSource(unsigned int nInstrumentID, int nInterval, CInstrumentBarInfoStorager** ppStorager)
{
	int nFunRes = 0;

	if (NULL == ppStorager)
	{
		nFunRes = -1;
		return nFunRes;			 
	}
	if (NULL != *ppStorager)
	{
		delete (*ppStorager);
		(*ppStorager) = NULL;
	}
	return nFunRes;
}

int CCreateCFDInstrumentBarInfo::_GetNextBarInfo( CInstrumentBarInfoStorager* pStorager, bool& bIsLstEnd, Bar& BarInfoGet)
{
	int nFunRes = 0;

	if (NULL == pStorager)
	{
		nFunRes = -1;
		bIsLstEnd = true;
		return nFunRes;			 
	}

	nFunRes = pStorager->getNextBarInfo(BarInfoGet);
	if (0 != nFunRes)
	{
		bIsLstEnd = true;
	}
	return nFunRes;
}

int CCreateCFDInstrumentBarInfo::createCFDbarInfoByDB()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CSyncMarketDataForCFD::LstCFDBarInfoT   lstCFDBarInfoTmp;
	CSyncMarketDataForCFD::enNextWorkType nNextWorkType;
	
	CInstrumentBarInfoStorager*  pStoragerFirst = NULL;
	CInstrumentBarInfoStorager*  pStoragerSecond = NULL;
	bool bIsLstFirstEnd = false;
	bool bIsLstSecondEnd = false;
	
	Bar BarInfoFirst;
	Bar BarInfoSecond;
	Bar BarInfoFirstGetTmp;
	Bar BarInfoSecondGetTmp;

	nFunRes = _InitInstrumentDataSource(m_nInstrumentIDFirst, m_nInterval, &pStoragerFirst);
	if (0 != nFunRes)
	{
		_UnInitInstrumentDataSource(m_nInstrumentIDFirst, m_nInterval, &pStoragerFirst);
		pStoragerFirst = NULL;
		nFunRes = -1;
		return nFunRes;
	}

	nFunRes = _InitInstrumentDataSource(m_nInstrumentIDSecond, m_nInterval, &pStoragerSecond);
	if (0 != nFunRes)
	{
		_UnInitInstrumentDataSource(m_nInstrumentIDFirst, m_nInterval, &pStoragerFirst);
		pStoragerFirst = NULL;
		_UnInitInstrumentDataSource(m_nInstrumentIDSecond, m_nInterval, &pStoragerSecond);
		pStoragerSecond = NULL;
		nFunRes = -1;
		return nFunRes;
	}

	//get CFD bar
	m_CSyncMarketDataForCFD->setCFDInstrumentIDFirst(m_nInstrumentIDFirst);
	m_CSyncMarketDataForCFD->setCFDInstrumentIDSecond(m_nInstrumentIDSecond);
	m_CSyncMarketDataForCFD->setInterval(m_nInterval);
	bIsLstFirstEnd = false;
	bIsLstSecondEnd = false;

	_GetNextBarInfo(pStoragerFirst, bIsLstFirstEnd, BarInfoFirstGetTmp);
	_GetNextBarInfo(pStoragerSecond, bIsLstSecondEnd, BarInfoSecondGetTmp);

	while 
		(
		(true != bIsLstFirstEnd)
		|| (true != bIsLstSecondEnd)
		)
	{
		if (true != bIsLstFirstEnd)
		{
			BarInfoFirst = BarInfoFirstGetTmp;
		}
		if (true != bIsLstSecondEnd)
		{
			BarInfoSecond = BarInfoSecondGetTmp;
		}

		m_CSyncMarketDataForCFD->syncSingleCFDBarInfo(BarInfoFirst, BarInfoSecond, lstCFDBarInfoTmp, nNextWorkType);
		_UpdateCFDMarketData(lstCFDBarInfoTmp);
		m_CSyncMarketDataForCFD->clearCFDBarInfoList(lstCFDBarInfoTmp);

		if (CSyncMarketDataForCFD::NextWorkType_UseNewFirst_UseNewSecond == nNextWorkType)
		{
			if (true != bIsLstFirstEnd)
			{
				_GetNextBarInfo(pStoragerFirst, bIsLstFirstEnd, BarInfoFirstGetTmp);
			}
			if (true != bIsLstSecondEnd)
			{
				_GetNextBarInfo(pStoragerSecond, bIsLstSecondEnd, BarInfoSecondGetTmp);	
			}
		}
		else if (CSyncMarketDataForCFD::NextWorkType_UseNewFirst_ReUseSecond == nNextWorkType)
		{
			if (true != bIsLstFirstEnd)
			{
				_GetNextBarInfo(pStoragerFirst, bIsLstFirstEnd, BarInfoFirstGetTmp);
			}
			else
			{
				//last data
				BarInfoFirst.Time = BarInfoSecond.Time;
			}
		}
		else if (CSyncMarketDataForCFD::NextWorkType_ReUseFirst_UseNewSecond == nNextWorkType)
		{
			if (true != bIsLstSecondEnd)
			{
				_GetNextBarInfo(pStoragerSecond, bIsLstSecondEnd, BarInfoSecondGetTmp);	
			}
			else
			{
				//last data
				BarInfoSecond.Time = BarInfoFirst.Time;
			}
		}

	}//while


	_UnInitInstrumentDataSource(m_nInstrumentIDFirst, m_nInterval, &pStoragerFirst);
	pStoragerFirst = NULL;
	_UnInitInstrumentDataSource(m_nInstrumentIDSecond, m_nInterval, &pStoragerSecond);
	pStoragerSecond = NULL;
	return nFunRes;
}

void CCreateCFDInstrumentBarInfo::doOneTest()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	//prepare data
	Bar BarInfo_First;
	Bar BarInfo_Second;
	std::list<Bar>  lstBarFirst;
	std::list<Bar>  lstBarSecond;

	BarInfo_First.Time = 1387502100;
	BarInfo_First.Open = 2346;
	BarInfo_First.Close = 2345;
	BarInfo_First.High = 2347;
	BarInfo_First.Low = 2344;
	BarInfo_First.Volume = 0;

	BarInfo_Second.Time = 1387502110;
	BarInfo_Second.Open = 2336;
	BarInfo_Second.Close = 2335;
	BarInfo_Second.High = 2337;
	BarInfo_Second.Low = 2334;
	BarInfo_Second.Volume = 0;

	//type1 small
	BarInfo_First.Time = 1387502100;
	BarInfo_Second.Time = 1387502110;
	lstBarFirst.push_back(BarInfo_First);
	lstBarSecond.push_back(BarInfo_Second);	

	//type2 bigger
	BarInfo_First.Time = 1387502115;
	BarInfo_Second.Time = 1387502112;
	lstBarFirst.push_back(BarInfo_First);
	lstBarSecond.push_back(BarInfo_Second);	

	//type3 equal
	BarInfo_First.Time = 1387502120;
	BarInfo_Second.Time = 1387502120;
	lstBarFirst.push_back(BarInfo_First);
	lstBarSecond.push_back(BarInfo_Second);

	//type3 equal
	BarInfo_First.Time = 1387502130;
	BarInfo_Second.Time = 1387502130;
	lstBarFirst.push_back(BarInfo_First);
	lstBarSecond.push_back(BarInfo_Second);	

	//type3 equal
	BarInfo_First.Time = 1387502135;
	BarInfo_Second.Time = 1387502140;
	lstBarFirst.push_back(BarInfo_First);
	lstBarSecond.push_back(BarInfo_Second);	

	createCFDbarInfoByLst(lstBarFirst, lstBarSecond);
	lstBarFirst.clear();
	lstBarSecond.clear();


}

NS_END(TA_Base_App)
















