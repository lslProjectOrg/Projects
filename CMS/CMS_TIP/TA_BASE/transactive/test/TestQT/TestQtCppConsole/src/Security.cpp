#include "Security.h"
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>

Security::Table Security::g_securities;
static QString historyDatafolder = "data";

QString Security::getDatabaseName() const
{
	QString filename = historyDatafolder;
	filename += QDir::separator();
	filename += m_securityExchange;
	filename += "-";
	filename += m_code;
	filename += ".dat";
	return filename;
}

void Security::saveMetaData()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", getDatabaseName());	
	db.setDatabaseName(getDatabaseName());
	db.open();

	QSqlQuery query(db);

	size_t recordCount = 0;
	query.exec("SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND NAME='security' ;");
	if (query.next())
	{
		recordCount = query.value(0).toUInt();
	}		

	if (recordCount == 0)
	{
		QString ddl = 
			"create table security(" 
			"id int," 
			"code varchar(32),"
			"symbol varchar(32)," 
			"securityType char(1), "
			"securityId varchar(32), "
			"securityExchange varchar(32), "
			"tickSize decimal, "
			"tickValue decimal,"
			"decimalPlace int, "
			"maturityDate int); ";
		query.exec(ddl);
	}

	query.prepare("SELECT COUNT(*) FROM security WHERE id = :id ;");
	query.bindValue(":id", m_id);
	query.exec();
	
	recordCount = 0;		
	if (!query.next())
	{
		recordCount = query.value(0).toUInt();
	}	

	if (recordCount == 0)
	{
		query.prepare("INSERT INTO security VALUES(:id,:code,:symbol,:securityType,"
				":securityId,:securityExchange,:tickSize,:tickValue,:decimalPlace,:maturityDate);");
	}
	else 
	{
		query.prepare("UPDATE security set code=:code symbol=:symbol securityType=:securityType "
				"securityId=:securityId securityExchange=:securityExchange tickSize=:tickSize "
				"tickValue=:tickValue decimalPlace=:decimalPlace maturityDate=:maturityDate WHERE id=:id;");	
	}

	
	query.bindValue(":id", m_id);
	query.bindValue(":code", m_code);
	query.bindValue(":symbol", m_symbol);
	query.bindValue(":securityType", m_securityType);
	query.bindValue(":securityId", m_securityId);
	query.bindValue(":securityExchange", m_securityExchange);
	query.bindValue(":tickSize", m_tickSize);
	query.bindValue(":tickValue", m_tickValue);
	query.bindValue(":decimalPlace", m_decimalPlace);
	query.bindValue(":maturityDate", m_maturityDate);
	if (!query.exec())
	{
		std::cerr << query.lastError().text().toStdString() << std::endl;
	}

	db.close();
}

Security::Table Security::loadFromDatabase(const QString &connectionString)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setUserName("root");
	db.setDatabaseName("oms");	
	db.open();


	QSqlQuery query(db);
	query.exec("select id,code,securityExchange,securityId,securityType,symbol,"
		"tickSize,TickValue,DecimalPlace,MaturityDate from instrument;");

	while (query.next())
	{
		unsigned int id;
		QString code;
		QString securityExchange;	
		QString securityId;
		QChar securityType;
		QString symbol;
		double tickSize;
		double tickValue;
		int decimalPlace;
		int maturityDate;
		
		id = query.value(0).toUInt();
		code = query.value(1).toString();
		securityExchange = query.value(2).toString();
		securityId = query.value(3).toString();
		securityType = query.value(4).toChar();	
		symbol = query.value(5).toString();
		tickSize = query.value(6).toDouble();
		tickValue = query.value(7).toDouble();
		decimalPlace = query.value(8).toInt();
		maturityDate = query.value(9).toInt();

		boost::shared_ptr<Security> security(new Security(id,
								securityType,
								code,
								symbol,
								securityId,
								securityExchange,
								tickSize,
								tickValue,
								decimalPlace,
								maturityDate));
		g_securities[id] = security;
		
	}
	
	return g_securities;
}
