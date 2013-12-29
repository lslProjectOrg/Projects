/*
save data to SQLite
//db name and table name
//SQLiteDB_3306.db
//Table_BarInfo_3306_60
//Table_BarInfo_3306_300
*/

#ifndef __CLASS_INSTUMENT_BAR_INFO_STORAGER__HH__
#define __CLASS_INSTUMENT_BAR_INFO_STORAGER__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

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

class Bar;

NS_BEGIN(TA_Base_App) 

class CCFDServerUtilityFun;

class CInstrumentBarInfoStorager 
{
public:
	typedef std::map<int, std::string>				   MapIntervalDBTableNameT;
	typedef std::map<int, std::string>::iterator      MapIntervalDBTableNameIterT;
	typedef std::map<int, std::string>::value_type    MapIntervalDBTableNameValueTypeT;
public:
	CInstrumentBarInfoStorager(unsigned int nInstrumentID);
	~CInstrumentBarInfoStorager(void);
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
private:
	unsigned int m_nInstrumentID;
	std::string m_strSQLiteDBName;
private:
	QSqlDatabase		m_QSqlDataBase;
	CCFDServerUtilityFun*   m_pUtilityFun;
	MapIntervalDBTableNameT* m_pmapIntervalDBTableName;
};

NS_END(TA_Base_App) 


#endif //__CLASS_INSTUMENT_BAR_INFO_STORAGER__HH__


