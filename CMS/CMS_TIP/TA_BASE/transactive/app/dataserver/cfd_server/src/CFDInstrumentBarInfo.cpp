#include "CFDInstrumentBarInfo.h"
#include "BuildCFDMarketData.h"

#include "BarCalculator.h"

#include "CFDServerUtilityFun.h"


#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CCFDInstrumentBarInfo::CCFDInstrumentBarInfo(void)
{
	BOOST_LOG_FUNCTION();
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_pBuildCFDMarketData = new CBuildCFDMarketData();

}

CCFDInstrumentBarInfo::~CCFDInstrumentBarInfo( void )
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}
	if (NULL != m_pBuildCFDMarketData)
	{
		delete m_pBuildCFDMarketData;
		m_pBuildCFDMarketData = NULL;
	}
}


void CCFDInstrumentBarInfo::setBarInfoFirst( const Bar& nBarInfoFirst )
{
	BOOST_LOG_FUNCTION();
	m_nBarInfoFirst = nBarInfoFirst;

}

void CCFDInstrumentBarInfo::setBarInfoSecond( const Bar& nBarInfoSecond )
{
	BOOST_LOG_FUNCTION();
	m_nBarInfoSecond = nBarInfoSecond;

}

void CCFDInstrumentBarInfo::setInterval( int nInterval )
{
	BOOST_LOG_FUNCTION();
	m_nInterval = nInterval;

}

void CCFDInstrumentBarInfo::setCFDInstrumentID( int nCFDInstrumentID )
{
	BOOST_LOG_FUNCTION();
	m_nCFDInstrumentID = nCFDInstrumentID;
}

int CCFDInstrumentBarInfo::buidCFDBarInfo()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	nFunRes = m_pBuildCFDMarketData->buildCFDMarketData(m_nBarInfoFirst, m_nBarInfoSecond, m_CFDBarInfo, m_nInterval);

	return nFunRes;
}



NS_END(TA_Base_App)
















