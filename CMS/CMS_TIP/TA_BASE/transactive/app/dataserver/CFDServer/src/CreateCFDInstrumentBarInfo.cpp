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


CCreateCFDInstrumentBarInfo::CCreateCFDInstrumentBarInfo(const CCFDRequest& cfdRequest)
{
	BOOST_LOG_FUNCTION();
	m_CFDRequest = cfdRequest;
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_CSyncMarketDataForCFD = new CSyncMarketDataForCFD();
	m_pCFDBarInfoCalculator = new CCFDInstrumentBarInfoCalculator(cfdRequest);
	
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



void CCreateCFDInstrumentBarInfo::createCFDbarInfoByLst(LstMarketDataT& lstBarFirst, LstMarketDataT& lstBarSecond)
{
	BOOST_LOG_FUNCTION();
	CSyncMarketDataForCFD::LstCFDBarInfoT   lstCFDBarInfoTmp;
	CSyncMarketDataForCFD::enNextWorkType nNextWorkType;
	int interval = 5;//second
	int nFunRes = 0;

	//prepare data
	MarketData BarInfoFirst;
	MarketData BarInfoSecond;
	LstMarketDataIterT  iterLstFirst;
	LstMarketDataIterT  iterLstSecond;

	//get CFD bar
	m_CSyncMarketDataForCFD->setCFDInstrumentIDFirst(m_nInstrumentIDFirst);
	m_CSyncMarketDataForCFD->setCFDInstrumentIDSecond(m_nInstrumentIDSecond);

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

std::string CCreateCFDInstrumentBarInfo::_GetInstrumentPriceFileName(unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION;
	std::string strInstrumentPriceFileName;
	std::ostringstream streamTmp;

	streamTmp<<nInstrumentID<<".csv";//3320.csv

	strInstrumentPriceFileName = streamTmp.str();

	return strInstrumentPriceFileName;
}


int CCreateCFDInstrumentBarInfo::_InitInstrumentDataSource(unsigned int nInstrumentID, std::ifstream** ppStorager)
{
	BOOST_LOG_FUNCTION;
	int nFunRes = 0;
	std::string strInstrumentPriceFileName;

	strInstrumentPriceFileName = _GetInstrumentPriceFileName(nInstrumentID);

	if (NULL == ppStorager)
	{
		nFunRes = -1;
		return nFunRes;			 
	}
	if (NULL != *ppStorager)
	{
		_UnInitInstrumentDataSource(nInstrumentID, ppStorager);
		(*ppStorager) = NULL;
	}
	(*ppStorager) = new std::ifstream(strInstrumentPriceFileName.c_str());
	if (!(*ppStorager)->is_open())
	{
		_UnInitInstrumentDataSource(nInstrumentID, ppStorager);
		nFunRes = -1;
	}
	return nFunRes;
}
int CCreateCFDInstrumentBarInfo::_UnInitInstrumentDataSource(unsigned int nInstrumentID, std::ifstream** ppStorager)
{
	int nFunRes = 0;

	if (NULL == ppStorager)
	{
		nFunRes = -1;
		return nFunRes;			 
	}
	if (NULL != *ppStorager)
	{
		(*ppStorager)->close();
		delete (*ppStorager);
		(*ppStorager) = NULL;
	}
	return nFunRes;
}

int CCreateCFDInstrumentBarInfo::_GetNextBarInfo( std::ifstream* pStorager, bool& bIsLstEnd, MarketData& BarInfoGet)
{
	int nFunRes = 0;

	if (NULL == pStorager)
	{
		nFunRes = -1;
		bIsLstEnd = true;
		return nFunRes;			 
	}

	if (!pStorager->eof())
	{
		(*pStorager) >> BarInfoGet;
	}
	else
	{
		bIsLstEnd = true;
	}
	return nFunRes;
}

int CCreateCFDInstrumentBarInfo::createCFDbarInfoByMarketData()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CSyncMarketDataForCFD::LstCFDBarInfoT   lstCFDBarInfoTmp;
	CSyncMarketDataForCFD::enNextWorkType nNextWorkType;

	std::ifstream* pFirstInstrumentPriceFile = NULL;
	std::ifstream* pSecondInstrumentPriceFile = NULL;
	std::string strFirstInstrumentPriceFileName;
	std::string strSecondInstrumentPriceFileName;

	bool bIsLstFirstEnd = false;
	bool bIsLstSecondEnd = false;

	MarketData BarInfoFirst;
	MarketData BarInfoSecond;
	MarketData BarInfoFirstGetTmp;
	MarketData BarInfoSecondGetTmp;

	strFirstInstrumentPriceFileName = _GetInstrumentPriceFileName(m_nInstrumentIDFirst);
	strSecondInstrumentPriceFileName = _GetInstrumentPriceFileName(m_nInstrumentIDSecond);


	nFunRes = _InitInstrumentDataSource(m_nInstrumentIDFirst, &pFirstInstrumentPriceFile);
	if (0 != nFunRes)
	{
		_UnInitInstrumentDataSource(m_nInstrumentIDFirst, &pFirstInstrumentPriceFile);
		pFirstInstrumentPriceFile = NULL;
		nFunRes = -1;
		return nFunRes;
	}

	nFunRes = _InitInstrumentDataSource(m_nInstrumentIDSecond, m_nInterval, &pSecondInstrumentPriceFile);
	if (0 != nFunRes)
	{
		_UnInitInstrumentDataSource(m_nInstrumentIDFirst, m_nInterval, &pFirstInstrumentPriceFile);
		pFirstInstrumentPriceFile = NULL;
		_UnInitInstrumentDataSource(m_nInstrumentIDSecond, m_nInterval, &pSecondInstrumentPriceFile);
		pSecondInstrumentPriceFile = NULL;
		nFunRes = -1;
		return nFunRes;
	}

	//get CFD bar
	m_CSyncMarketDataForCFD->setCFDInstrumentIDFirst(m_nInstrumentIDFirst);
	m_CSyncMarketDataForCFD->setCFDInstrumentIDSecond(m_nInstrumentIDSecond);
	bIsLstFirstEnd = false;
	bIsLstSecondEnd = false;

	_GetNextBarInfo(pFirstInstrumentPriceFile, bIsLstFirstEnd, BarInfoFirstGetTmp);
	_GetNextBarInfo(pSecondInstrumentPriceFile, bIsLstSecondEnd, BarInfoSecondGetTmp);

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
				_GetNextBarInfo(pFirstInstrumentPriceFile, bIsLstFirstEnd, BarInfoFirstGetTmp);
			}
			if (true != bIsLstSecondEnd)
			{
				_GetNextBarInfo(pSecondInstrumentPriceFile, bIsLstSecondEnd, BarInfoSecondGetTmp);
			}
		}
		else if (CSyncMarketDataForCFD::NextWorkType_UseNewFirst_ReUseSecond == nNextWorkType)
		{
			if (true != bIsLstFirstEnd)
			{
				_GetNextBarInfo(pFirstInstrumentPriceFile, bIsLstFirstEnd, BarInfoFirstGetTmp);
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
				_GetNextBarInfo(pSecondInstrumentPriceFile, bIsLstSecondEnd, BarInfoSecondGetTmp);
			}
			else
			{
				//last data
				BarInfoSecond.Time = BarInfoFirst.Time;
			}
		}

	}//while

	_UnInitInstrumentDataSource(m_nInstrumentIDFirst, m_nInterval, &pFirstInstrumentPriceFile);
	pFirstInstrumentPriceFile = NULL;
	_UnInitInstrumentDataSource(m_nInstrumentIDSecond, m_nInterval, &pSecondInstrumentPriceFile);
	pSecondInstrumentPriceFile = NULL;
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
















