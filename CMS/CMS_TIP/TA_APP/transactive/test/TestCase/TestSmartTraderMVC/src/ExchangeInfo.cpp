#include "ExchangeInfo.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_App_App)



CExchangeInfo::CExchangeInfo()
{
	m_strExchangeName.clear();
	m_strUnderlyingCode.clear();
	m_strInstrumentCode.clear();
}

CExchangeInfo::~CExchangeInfo()
{
	m_strExchangeName.clear();
	m_strUnderlyingCode.clear();
	m_strInstrumentCode.clear();
}


CExchangeInfo& CExchangeInfo::operator=( const CExchangeInfo& exchangeParam )
{
	m_strExchangeName = exchangeParam.m_strExchangeName;
	m_strExchangeName = exchangeParam.m_strUnderlyingCode;
	m_strExchangeName = exchangeParam.m_strInstrumentCode;

	return *this;
}


void CExchangeInfo::setValue( const Instrument &instrument)
{
	m_strExchangeName = instrument.getExchangeName().c_str();
	m_strUnderlyingCode = instrument.getUnderlyingCode().c_str();
	m_strInstrumentCode = instrument.getInstrumentCode().c_str();
}

void CExchangeInfo::setValue( const std::string& strExchangeName, const std::string& strUnderlyingCode, const std::string& strInstrumentCode )
{
	m_strExchangeName = strExchangeName.c_str();
	m_strUnderlyingCode = strUnderlyingCode.c_str();
	m_strInstrumentCode = strInstrumentCode.c_str();

}

QString CExchangeInfo::getExchangeName() const
{
	return m_strExchangeName;
}

QString CExchangeInfo::getUnderlyingCode() const
{
	return m_strUnderlyingCode;
}

QString CExchangeInfo::getInstrumentCode() const
{
	return m_strInstrumentCode;
}


void CExchangeInfo::logInfo( const QString& strLogInfo ) const
{
	LOG_DEBUG<<strLogInfo.toStdString()
		<<" "<<"m_strExchangeName="<<m_strExchangeName.toStdString()
		<<" "<<"m_strUnderlyingCode="<<m_strUnderlyingCode.toStdString()
		<<" "<<"m_strInstrumentCode="<<m_strInstrumentCode.toStdString();
}



NS_END(TA_App_App)












