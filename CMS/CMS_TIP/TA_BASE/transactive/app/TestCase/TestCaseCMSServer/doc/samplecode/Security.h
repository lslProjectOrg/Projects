#ifndef TICKS_SECURITY_H
#define TICKS_SECURITY_H
#include <QtCore/QChar>
#include <QtCore/QString>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

class Security 
{
public:
	Security(unsigned int id, 
		const QChar &securityType,
		const QString &code,
		const QString &symbol,
		const QString &securityId,
		const QString &securityExchange,
		double tickSize,
		double tickValue,
		int decimalPlace, 
		int maturityDate):
	m_id(id),
	m_securityType(securityType),
	m_code(code),
	m_symbol(symbol),
	m_securityId(securityId),
	m_securityExchange(securityExchange),
	m_tickSize(tickSize),
	m_tickValue(tickValue),
	m_decimalPlace(decimalPlace),
	m_maturityDate(maturityDate)
	{
	}

	void saveMetaData();
	QString getDatabaseName() const;

	typedef boost::shared_ptr<Security> Ptr;
	typedef boost::unordered_map<unsigned int, Security::Ptr> Table;

	static Table loadFromDatabase(const QString &connectionString);
private:
	unsigned int m_id;
	QChar m_securityType;
			
	QString m_code;
	QString m_symbol;
	QString m_securityId;
	QString m_securityExchange;

	double m_tickSize;
	double m_tickValue;
	int m_decimalPlace;	
	int m_maturityDate;
	static Table g_securities;
};

#endif
