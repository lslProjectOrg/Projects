#include "CFDInstrumentBarInfo.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CCFDInstrumentBarInfo::CCFDInstrumentBarInfo(void)
{
	BOOST_LOG_FUNCTION();
}

CCFDInstrumentBarInfo::~CCFDInstrumentBarInfo(void)
{
	BOOST_LOG_FUNCTION();
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

void CCFDInstrumentBarInfo::setCFDInstrumentIDFirst(unsigned int nCFDInstrumentIDFirst)
{
	BOOST_LOG_FUNCTION();

	m_nCFDInstrumentIDFirst = nCFDInstrumentIDFirst;
}


void CCFDInstrumentBarInfo::setCFDInstrumentIDSecond(unsigned int nCFDInstrumentIDSecond)
{
	BOOST_LOG_FUNCTION();

	m_nCFDInstrumentIDSecond = nCFDInstrumentIDSecond;
}


int CCFDInstrumentBarInfo::buidCFDBarInfo()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	if (m_nBarInfoFirst.Time != m_nBarInfoSecond.Time)
	{
		nFunRes = -1;
		return nFunRes;
	}

	m_CFDBarInfo.Time = m_nBarInfoFirst.Time;
	m_CFDBarInfo.Open = m_nBarInfoFirst.Open - m_nBarInfoSecond.Open;
	m_CFDBarInfo.Close = m_nBarInfoFirst.Close - m_nBarInfoSecond.Close;
	m_CFDBarInfo.High = m_nBarInfoFirst.High - m_nBarInfoSecond.High;
	m_CFDBarInfo.Low = m_nBarInfoFirst.Low - m_nBarInfoSecond.Low;
	m_CFDBarInfo.Volume = m_nBarInfoFirst.Volume - m_nBarInfoSecond.Volume;


	//TODO.
	//bid  buy  
	//ask  sell
	/*
	Jake: A B
	xA - yB
	bidPx = xABid - yBAsk
	askPx = xAAsk - yBBid
	price = (bidPx + askPx) / 2
	price = (xAska + xAskb - yAska - yBidb) / 2
	*/
	return nFunRes;
}



void CCFDInstrumentBarInfo::logInfo()
{
	BOOST_LOG_FUNCTION();
	std::string strInfo;
	std::string strBarTime;
	Bar* pBarInfo = NULL;

	pBarInfo = &m_CFDBarInfo;
	strInfo = "CFDInstrumentBarInfo: ";

	//strBarTime = dataTimeToStr(pBarInfo->Time);
	LOG_DEBUG<<strInfo<<"interval="<<m_nInterval<<" "<<"pBarInfo->Time="<<pBarInfo->Time;
	//LOG_DEBUG<<strInfo<<"interval="<<interval<<" "<<"strBarTime="<<strBarTime;
	LOG_DEBUG<<strInfo<<"interval="<<m_nInterval<<" "<<"pBarInfo->Open="<<pBarInfo->Open;
	LOG_DEBUG<<strInfo<<"interval="<<m_nInterval<<" "<<"pBarInfo->Close="<<pBarInfo->Close;
	LOG_DEBUG<<strInfo<<"interval="<<m_nInterval<<" "<<"pBarInfo->High="<<pBarInfo->High;
	LOG_DEBUG<<strInfo<<"interval="<<m_nInterval<<" "<<"pBarInfo->Low="<<pBarInfo->Low;
	LOG_DEBUG<<strInfo<<"interval="<<m_nInterval<<" "<<"pBarInfo->Volume="<<pBarInfo->Volume;

	return;
}

NS_END(TA_Base_App)
















