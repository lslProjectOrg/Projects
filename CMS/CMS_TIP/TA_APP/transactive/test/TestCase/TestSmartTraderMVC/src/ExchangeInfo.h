#ifndef __CLASS_EXCHANGE_INFO_HH__
#define __CLASS_EXCHANGE_INFO_HH__

#include "ProjectCommonData.h"

//qt sys
#include <QtCore/QChar>
#include <QtCore/QString>

#include "Instrument.h"

NS_BEGIN(TA_App_App)

class CExchangeInfo 
{ 
public: 
	CExchangeInfo();

	virtual ~CExchangeInfo(); 
	CExchangeInfo& operator=(const CExchangeInfo& exchangeParam);

public:
	void setValue( const Instrument &instrument);
	void setValue(const std::string& strExchangeName, const std::string& strUnderlyingCode, const std::string& strInstrumentCode);

	QString getExchangeName() const;
	QString getUnderlyingCode() const;
	QString getInstrumentCode() const;

	void logInfo(const QString& strLogInfo) const;
private:
	QString m_strExchangeName;
	QString m_strUnderlyingCode;
	QString m_strInstrumentCode;

}; 


NS_END(TA_App_App)




#endif//__CLASS_EXCHANGE_INFO_HH__



