#include "InstrumentBarInfo.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CInstrumentBarInfo::CInstrumentBarInfo(int interval, Bar& barInfo)
{
	BOOST_LOG_FUNCTION();
	m_nInterval = interval;
	m_BarInfo = barInfo;

}

CInstrumentBarInfo::~CInstrumentBarInfo(void)
{
	BOOST_LOG_FUNCTION();
}


CInstrumentBarInfo& CInstrumentBarInfo::operator=(const CInstrumentBarInfo& instrumentBarInfo )
{
	BOOST_LOG_FUNCTION();
	m_nInterval = instrumentBarInfo.m_nInterval;
	m_BarInfo = instrumentBarInfo.m_BarInfo;
	return *this;
}




void CInstrumentBarInfo::logInfo()
{
	BOOST_LOG_FUNCTION();

	return;
}


NS_END(TA_Base_App)
















