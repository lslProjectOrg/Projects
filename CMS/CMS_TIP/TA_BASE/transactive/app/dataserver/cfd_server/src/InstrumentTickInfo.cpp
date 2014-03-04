#include "InstrumentTickInfo.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)

CInstrumentTickInfo::CInstrumentTickInfo( Tick& tickInfo )
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

	LOG_TRACE<<"Tick"<<"->"<<"nTimeValue="<<m_TickInfo.Time;
	//LOG_TRACE<<"Tick"<<"->"<<"strTimeValue="<<strTimeValue;
	LOG_TRACE<<"Tick"<<"->"<<"BidPx="<<m_TickInfo.BidPx;
	LOG_TRACE<<"Tick"<<"->"<<"AskPx="<<m_TickInfo.AskPx;
	LOG_TRACE<<"Tick"<<"->"<<"LastPx="<<m_TickInfo.LastPx;
	LOG_TRACE<<"Tick"<<"->"<<"BidVol="<<m_TickInfo.BidVol;
	LOG_TRACE<<"Tick"<<"->"<<"AskVol="<<m_TickInfo.AskVol;
	LOG_TRACE<<"Tick"<<"->"<<"LastVol="<<m_TickInfo.LastVol;

	return;
}


NS_END(TA_Base_App)
















