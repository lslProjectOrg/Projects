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
	m_CSyncMarketDataForCFD = new CSyncMarketDataForCFD(m_CFDRequest);
	m_pCFDBarInfoCalculator = new CCFDInstrumentBarInfoCalculator(m_CFDRequest);
	
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
	m_CSyncMarketDataForCFD->setCFDRequest(m_CFDRequest);

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
				//BarInfoFirst.Time = BarInfoSecond.Time;
				break;
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
				//BarInfoSecond.Time = BarInfoFirst.Time;
				break;
			}
		}

	}//while
}

std::string CCreateCFDInstrumentBarInfo::_GetInstrumentPriceFileName(unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();
	std::string strInstrumentPriceFileName;
	std::ostringstream streamTmp;

	streamTmp<<m_CFDRequest.m_strPathHistoryMarketDataInstrument<<"//"<<nInstrumentID<<".csv";//3320.csv

	strInstrumentPriceFileName = streamTmp.str();

	return strInstrumentPriceFileName;
}


int CCreateCFDInstrumentBarInfo::_InitInstrumentDataSource(unsigned int nInstrumentID, std::ifstream** ppStorager)
{
	BOOST_LOG_FUNCTION();
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
	BOOST_LOG_FUNCTION();

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
	BOOST_LOG_FUNCTION();

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

	strFirstInstrumentPriceFileName = _GetInstrumentPriceFileName(m_CFDRequest.m_nCFDInstrumentIDFirst);
	strSecondInstrumentPriceFileName = _GetInstrumentPriceFileName(m_CFDRequest.m_nCFDInstrumentIDSecond);


	nFunRes = _InitInstrumentDataSource(m_CFDRequest.m_nCFDInstrumentIDFirst, &pFirstInstrumentPriceFile);
	if (0 != nFunRes)
	{
		_UnInitInstrumentDataSource(m_CFDRequest.m_nCFDInstrumentIDFirst, &pFirstInstrumentPriceFile);
		pFirstInstrumentPriceFile = NULL;
		nFunRes = -1;
		return nFunRes;
	}

	nFunRes = _InitInstrumentDataSource(m_CFDRequest.m_nCFDInstrumentIDSecond, &pSecondInstrumentPriceFile);
	if (0 != nFunRes)
	{
		_UnInitInstrumentDataSource(m_CFDRequest.m_nCFDInstrumentIDFirst, &pFirstInstrumentPriceFile);
		pFirstInstrumentPriceFile = NULL;
		_UnInitInstrumentDataSource(m_CFDRequest.m_nCFDInstrumentIDSecond, &pSecondInstrumentPriceFile);
		pSecondInstrumentPriceFile = NULL;
		nFunRes = -1;
		return nFunRes;
	}

	//get CFD bar
	m_CSyncMarketDataForCFD->setCFDRequest(m_CFDRequest);
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
				//BarInfoFirst.Time = BarInfoSecond.Time;
				break;
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
				//BarInfoSecond.Time = BarInfoFirst.Time;
				break;
			}
		}

	}//while

	_UnInitInstrumentDataSource(m_CFDRequest.m_nCFDInstrumentIDFirst, &pFirstInstrumentPriceFile);
	pFirstInstrumentPriceFile = NULL;
	_UnInitInstrumentDataSource(m_CFDRequest.m_nCFDInstrumentIDSecond, &pSecondInstrumentPriceFile);
	pSecondInstrumentPriceFile = NULL;
	return nFunRes;
}

NS_END(TA_Base_App)
















