#include "CreateCFDInstrumentBarInfo.h"

#include "BarCalculator.h"

#include "CFDServerUtilityFun.h"
#include "CFDInstrumentBarInfo.h"
#include "SyncMarketDataForCFD.h"
#include "CFDInstrumentBarInfoCalculator.h"



#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CCreateCFDInstrumentBarInfo::CCreateCFDInstrumentBarInfo(unsigned int nInstrumentIDFirst, unsigned int nInstrumentIDSecond)
{
	BOOST_LOG_FUNCTION();
	m_nInstrumentIDFirest = nInstrumentIDFirst;
	m_nInstrumentIDSecond = nInstrumentIDSecond;
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_CSyncMarketDataForCFD = new CSyncMarketDataForCFD();
	m_pCFDBarInfoCalculator = new CCFDInstrumentBarInfoCalculator(m_nInstrumentIDFirest, m_nInstrumentIDSecond);
	
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

void CCreateCFDInstrumentBarInfo::doOneTest()
{
	BOOST_LOG_FUNCTION();
	CSyncMarketDataForCFD::LstCFDBarInfoT   lstCFDBarInfoTmp;
	int interval = 5;//second
	int nFunRes = 0;
	CSyncMarketDataForCFD::enNextWorkType nNextWorkType;

	//prepare data
	Bar BarInfo_First;
	Bar BarInfo_Second;
	std::list<Bar>  lstBarFirst;
	std::list<Bar>::iterator  iterLstFirst;
	std::list<Bar>  lstBarSecond;
	std::list<Bar>::iterator  iterLstSecond;

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


	//get CFD bar
	m_CSyncMarketDataForCFD->setCFDInstrumentIDFirst(m_nInstrumentIDFirest);
	m_CSyncMarketDataForCFD->setCFDInstrumentIDSecond(m_nInstrumentIDSecond);
	m_CSyncMarketDataForCFD->setInterval(interval);


	iterLstFirst = lstBarFirst.begin();
	iterLstSecond = lstBarSecond.begin();

	//15705914359
	while 
		(
			(iterLstFirst != lstBarFirst.end())
			|| (iterLstSecond != lstBarSecond.end())
		)
	{
		if (iterLstFirst != lstBarFirst.end())
		{
			BarInfo_First = (*iterLstFirst);
		}
		if (iterLstSecond != lstBarSecond.end())
		{
			BarInfo_Second = (*iterLstSecond);
		}

		m_CSyncMarketDataForCFD->syncSingleCFDBarInfo(BarInfo_First, BarInfo_Second, lstCFDBarInfoTmp, nNextWorkType);
		UpdateCFDMarketData(lstCFDBarInfoTmp);
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
				BarInfo_First.Time = BarInfo_Second.Time;
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
				BarInfo_Second.Time = BarInfo_First.Time;
			}
		}

	}//while
}

int CCreateCFDInstrumentBarInfo::UpdateCFDMarketData(CSyncMarketDataForCFD::LstCFDBarInfoT&  lstCFDBarInfo)
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



NS_END(TA_Base_App)
















