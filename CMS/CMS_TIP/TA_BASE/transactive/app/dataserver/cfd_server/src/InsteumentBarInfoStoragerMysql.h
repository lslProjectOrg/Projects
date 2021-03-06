/*
save data to SQLite
//db name and table name

//5secconds, 1mins��5mins��15mins��30mins��60mins��1day

//SQLiteDB_3306.db
//mysqldb_3306

//bar_data_5seconds
//bar_data_1min
//bar_data_5min
//bar_data_15min
//bar_data_30min
//bar_data_60min
//bar_data_1day
//bar_data_1mon
//bar_data_1year
//bar_data_30day
*/

#ifndef __CLASS_INSTUMENT_BAR_INFO_STORAGER_MYSQL__HH__
#define __CLASS_INSTUMENT_BAR_INFO_STORAGER_MYSQL__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"
#include "core/DataAccess/src/DataAccessCommonData.h"

#include <iostream>
#include <map>
#include <sstream>
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>


#include "mysql_connection.h"

#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"


class Bar;

NS_BEGIN(TA_Base_App) 

class CCFDServerUtilityFun;

class CInstrumentBarInfoStoragerMysql 
{
public:
	typedef std::map<int, std::string>				   MapIntervalDBTableNameT;
	typedef std::map<int, std::string>::iterator      MapIntervalDBTableNameIterT;
	typedef std::map<int, std::string>::value_type    MapIntervalDBTableNameValueTypeT;
public:
	CInstrumentBarInfoStoragerMysql(unsigned int nInstrumentID);
	~CInstrumentBarInfoStoragerMysql(void);
public:
	int storeBarInfo(int interval, Bar* pBarInfo);

private:
	int _InsertData(int interval, Bar* pBarInfo);
	int _DropDBTable(const std::string& strDbTableName);
	int _CreateDBTable(const std::string& strDbTableName);
	std::string _GetDBTableName(unsigned int nInstrumentID, int interval);
	int _Exec( const std::string& strSQL);
	std::string _BuildInsertSQL(const std::string& strTableName, Bar* pBarInfo);
	std::string _GetDBName(unsigned int nInstrumentID);
	std::string _BuildBarDataTableName(unsigned int nInstrumentID, int interval);
	void _InitDataBase();
	void _UnInitDataBase();

private:
	unsigned int m_nInstrumentID;
	std::string m_strDBName;//SQLiteDB_3306.db
	std::string m_strDBType;// defSQLiteDBName defMysqlDBName
	TA_Base_Core::DbServerType m_nDBType;//enumSqliteDb enumMysqlDb
private:
	QSqlDatabase		m_QSqlDataBase;

	sql::Driver*		m_pMysqlDriver;
	sql::Connection*    m_pMysqlDataBase;	
	
	CCFDServerUtilityFun*   m_pUtilityFun;
	MapIntervalDBTableNameT* m_pmapIntervalDBTableName;
};

NS_END(TA_Base_App) 


#endif //__CLASS_INSTUMENT_BAR_INFO_STORAGER__HH__


