#include "InstrumentTickInfo.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)

CInstrumentTickInfo::CInstrumentTickInfo( CMyTick& tickInfo )
{
	BOOST_LOG_FUNCTION();
	m_TickInfo = tickInfo;
}

CInstrumentTickInfo::~CInstrumentTickInfo(void)
{
	BOOST_LOG_FUNCTION();
}


CInstrumentTickInfo& CInstrumentTickInfo::operator=(const CInstrumentTickInfo& instrumentTickInfo )
{
	BOOST_LOG_FUNCTION();
	m_TickInfo = instrumentTickInfo.m_TickInfo;

	return *this;
}




void CInstrumentTickInfo::logInfo()
{
	BOOST_LOG_FUNCTION();

	//	strTimeValue = dataTimeToStr(nTimeValue);

	LOG_DEBUG<<"Tick"<<"->"<<"nTimeValue="<<m_TickInfo.Time;
	//LOG_DEBUG<<"Tick"<<"->"<<"strTimeValue="<<strTimeValue;
	LOG_DEBUG<<"Tick"<<"->"<<"BidPx="<<m_TickInfo.BidPx;
	LOG_DEBUG<<"Tick"<<"->"<<"AskPx="<<m_TickInfo.AskPx;
	LOG_DEBUG<<"Tick"<<"->"<<"LastPx="<<m_TickInfo.LastPx;
	LOG_DEBUG<<"Tick"<<"->"<<"BidVol="<<m_TickInfo.BidVol;
	LOG_DEBUG<<"Tick"<<"->"<<"AskVol="<<m_TickInfo.AskVol;
	LOG_DEBUG<<"Tick"<<"->"<<"LastVol="<<m_TickInfo.LastVol;

	return;
}


NS_END(TA_Base_App)
















