#ifndef __CLASS_DBOPERPARAM_H__
#define __CLASS_DBOPERPARAM_H__


#include "CFDServerCommonData.h"

NS_BEGIN(TA_Base_App)


class CDBOperParam
{
public:
	CDBOperParam(void);	
	~CDBOperParam(void);

public:
	CDBOperParam& operator=(const CDBOperParam& dbOperParam);

public:
	void logInfo();
	void setValue(TA_Base_App::DbServerType nDataSrcDbType, unsigned int nInstrumentID);
	void setValueSqliteDbPathTIK(const std::string& strSQLiteDBPathTIK);
	void setValueSqliteDbPathBAR(const std::string& strSQLiteDBPathBar);

private:
	std::string _GetSQLITEDBFileNameBar(unsigned int nInstrumentID);
	std::string _GetSQLITEDBFileNameFullPathBar(const std::string& strSQLiteDBPath, unsigned int nInstrumentID);
	std::string _GetSQLITEDBFileNameTIK(unsigned int nInstrumentID);
	std::string _GetSQLITEDBFileNameFullPathTIK(const std::string& strSQLiteDBPath, unsigned int nInstrumentID);
public:
	std::string m_strDataSrcDbType;//"MYSQL"  "SQLITE"
	std::string m_strInstrumentID;//"3320"

	TA_Base_App::DbServerType m_nDataSrcDbType;//enumSqliteDb
	std::string m_strQTDataSrcDbTypeName;//"QSQLITE"
	unsigned int m_nInstrumentID;//3320

public:
	std::string m_strSQLiteDBPathBar;//D://SaveDataBar
	std::string m_strSQLiteDBPathTIK;//D://SaveDataTIK
	std::string m_strSQLiteDBFileNameBAR;//3306.BAR
	std::string m_strSQLiteDBFileNameTIK;//3306.TIK
	std::string m_strSQLiteDBFileNameFullPathBAR;//D://SaveDataBar//3306.BAR
	std::string m_strSQLiteDBFileNameFullPathTIK;//D://SaveDataTIK//3306.TIK

public:
	std::string m_strMysqlSchema;
	std::string m_strMysqlIP;
	std::string m_strMysqlUser;
	std::string m_strMysqlPWD;
};


NS_END(TA_Base_App)


#endif // __CLASS_DBOPERPARAM_H__









