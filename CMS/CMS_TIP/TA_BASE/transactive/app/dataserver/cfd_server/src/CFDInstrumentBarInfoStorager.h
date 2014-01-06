#ifndef __CLASS_CFD_INSTUMENT_BAR_INFO_STORAGER__HH__
#define __CLASS_CFD_INSTUMENT_BAR_INFO_STORAGER__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <iostream>
#include <map>
#include <sstream>

#include <QtSql/QtSql>
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

#include "core/DataAccess/src/DataAccessCommonData.h"

class Bar;

NS_BEGIN(TA_Base_App) 

class CCFDServerUtilityFun;


class CCFDInstrumentBarInfoStorager 
{
public:
	typedef std::map<int, std::string>				   MapIntervalDBTableNameT;
	typedef std::map<int, std::string>::iterator      MapIntervalDBTableNameIterT;
	typedef std::map<int, std::string>::value_type    MapIntervalDBTableNameValueTypeT;
public:
	CCFDInstrumentBarInfoStorager(unsigned int nInstrumentID);
	~CCFDInstrumentBarInfoStorager(void);
public:
	int storeBarInfo(int interval, Bar* pBarInfo);

private:
	int _InsertData(int interval, Bar* pBarInfo);
	int _DropDBTable(const std::string& strDbTableName);
	int _CreateDBTable(const std::string& strDbTableName);
	std::string _GetDBTableName(unsigned int nInstrumentID, int interval);
	int _Exec( const std::string& strSQL);
	std::string _BuildInsertSQL(const std::string& strTableName);
	std::string _GetDBName(unsigned int nInstrumentID);
	std::string _BuildBarDataTableName(unsigned int nInstrumentID, int interval);
	void _InitDataBase();
	void _UnInitDataBase();
	std::string _BuildInsertSQLEx(const std::string& strTableName, Bar* pBarInfo);
	int _InsertDataEx(int interval, Bar* pBarInfo);
private:
	unsigned int m_nInstrumentID;
	std::string m_strDBName;//SQLiteDB_3306.db
	std::string m_strDBType;// defSQLiteDBName defMysqlDBName
	TA_Base_Core::DbServerType m_nDBType;//enumSqliteDb enumMysqlDb
private:
	QSqlDatabase		m_QSqlDataBase;
	CCFDServerUtilityFun*   m_pUtilityFun;
	MapIntervalDBTableNameT* m_pmapIntervalDBTableName;
};

NS_END(TA_Base_App) 


#endif //__CLASS_INSTUMENT_BAR_INFO_STORAGER__HH__


