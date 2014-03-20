#include "DBOperParam.h"

#include <boost/algorithm/string.hpp>
#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)

// static const std::string DEFVALUE_String_Mysql_Schema="oms";
// static const std::string DEFVALUE_String_Mysql_IP="192.168.1.1";
// static const std::string DEFVALUE_String_Mysql_User="jadeserver";
// static const std::string DEFVALUE_String_Mysql_PWD="JadeServer123";

static const std::string DEFVALUE_String_Mysql_Schema="oms";
static const std::string DEFVALUE_String_Mysql_IP="127.0.0.1";
static const std::string DEFVALUE_String_Mysql_User="root";
static const std::string DEFVALUE_String_Mysql_PWD="root";


// static const std::string DEFVALUE_String_Mysql_Schema="tra_occ";
// static const std::string DEFVALUE_String_Mysql_IP="192.168.123.43";
// static const std::string DEFVALUE_String_Mysql_User="IT271350_5";
// static const std::string DEFVALUE_String_Mysql_PWD="IT271350_5";

static const std::string DEFVALUE_String_SQLITE_DbPath_Bar="C://TestData//SaveDataBar";
static const std::string DEFVALUE_String_SQLITE_DbPath_TIK="C://TestData//SaveDataTIK";
static const std::string DEFVALUE_String_SQLITE_DbFile_Bar_Header = "";//3320.BAR
static const std::string DEFVALUE_String_SQLITE_DbFile_Tick_Header = "";//3320.TIK
static const std::string DEFVALUE_String_SQLITE_DbFile_Bar_Tail = ".BAR";//
static const std::string DEFVALUE_String_SQLITE_DbFile_Tick_Tail = ".TIK";//


CDBOperParam::CDBOperParam(void)
{
	BOOST_LOG_FUNCTION();

	m_strDataSrcDbType.clear();
	m_strInstrumentID.clear();
	m_nDataSrcDbType = TA_Base_App::enumSqliteDb;
	m_strQTDataSrcDbTypeName = DEF_VALUE_STRING_QT_SUPPORT_DB_QSQLITE;
	m_nInstrumentID = 3320;
	m_strSQLiteDBPathBar.clear();
	m_strSQLiteDBFileNameBAR.clear();
	m_strSQLiteDBFileNameFullPathBAR.clear();
	m_strSQLiteDBPathTIK.clear();
	m_strSQLiteDBFileNameTIK.clear();
	m_strSQLiteDBFileNameFullPathTIK.clear();
	m_strMysqlSchema.clear();
	m_strMysqlIP.clear();
	m_strMysqlUser.clear();
	m_strMysqlPWD.clear();

	//
	m_nInstrumentID = 3320;
	m_strInstrumentID = "3320";//3320

	m_strDataSrcDbType = DEF_VALUE_STRING_DB_SQLITE;
	m_nDataSrcDbType = TA_Base_App::enumSqliteDb;
	m_strDataSrcDbType = DEF_VALUE_STRING_DB_MYSQL;
	m_nDataSrcDbType = TA_Base_App::enumMysqlDb;

	m_strSQLiteDBPathBar = DEFVALUE_String_SQLITE_DbPath_Bar;
	m_strSQLiteDBFileNameBAR = _GetSQLITEDBFileNameBar(m_nInstrumentID);
	m_strSQLiteDBFileNameFullPathBAR = _GetSQLITEDBFileNameFullPathBar(m_strSQLiteDBPathBar, m_nInstrumentID);

	m_strSQLiteDBPathTIK = DEFVALUE_String_SQLITE_DbPath_TIK;
	m_strSQLiteDBFileNameTIK = _GetSQLITEDBFileNameTIK(m_nInstrumentID);
	m_strSQLiteDBFileNameFullPathTIK = _GetSQLITEDBFileNameFullPathTIK(m_strSQLiteDBPathTIK, m_nInstrumentID);


	m_strMysqlSchema = DEFVALUE_String_Mysql_Schema;
	m_strMysqlIP = DEFVALUE_String_Mysql_IP;
	m_strMysqlUser = DEFVALUE_String_Mysql_User;
	m_strMysqlPWD = DEFVALUE_String_Mysql_PWD;
}

CDBOperParam::~CDBOperParam(void)
{
	BOOST_LOG_FUNCTION();
}


CDBOperParam& CDBOperParam::operator=(const CDBOperParam& dbOperParam )
{
	BOOST_LOG_FUNCTION();
	//
	m_strDataSrcDbType = dbOperParam.m_strDataSrcDbType;
	m_strInstrumentID = dbOperParam.m_strInstrumentID;

	m_nDataSrcDbType = dbOperParam.m_nDataSrcDbType;
	m_strQTDataSrcDbTypeName = dbOperParam.m_strQTDataSrcDbTypeName;
	m_nInstrumentID = dbOperParam.m_nInstrumentID;

	//
	m_strMysqlSchema = dbOperParam.m_strMysqlSchema;
	m_strMysqlIP = dbOperParam.m_strMysqlIP;
	m_strMysqlUser = dbOperParam.m_strMysqlUser;
	m_strMysqlPWD = dbOperParam.m_strMysqlPWD;

	//
	m_strSQLiteDBPathBar = dbOperParam.m_strSQLiteDBPathBar;
	m_strSQLiteDBFileNameBAR = dbOperParam.m_strSQLiteDBFileNameBAR;
	m_strSQLiteDBFileNameFullPathBAR = dbOperParam.m_strSQLiteDBFileNameFullPathBAR;

	m_strSQLiteDBPathTIK = dbOperParam.m_strSQLiteDBPathTIK;
	m_strSQLiteDBFileNameTIK = dbOperParam.m_strSQLiteDBFileNameTIK;
	m_strSQLiteDBFileNameFullPathTIK = dbOperParam.m_strSQLiteDBFileNameFullPathTIK;

	return *this;
}

void CDBOperParam::logInfo()
{
	BOOST_LOG_FUNCTION();

	LOG_INFO<<" "<<"m_strDataSrcDbType="<<m_strDataSrcDbType;
	LOG_INFO<<" "<<"m_strInstrumentID="<<m_strInstrumentID;
	LOG_INFO<<" "<<"m_nInstrumentID="<<m_nInstrumentID;
	LOG_INFO<<" "<<"m_nDataSrcDbType="<<m_nDataSrcDbType;
	LOG_INFO<<" "<<"m_strQTDataSrcDbTypeName="<<m_strQTDataSrcDbTypeName;

	LOG_INFO<<" "<<"m_strSQLiteDBPathBar="<<m_strSQLiteDBPathBar;
	LOG_INFO<<" "<<"m_strSQLiteDBFileNameBar="<<m_strSQLiteDBFileNameBAR;
	LOG_INFO<<" "<<"m_strSQLiteDBFileNameFullPathBar="<<m_strSQLiteDBFileNameFullPathBAR;

	LOG_INFO<<" "<<"m_strSQLiteDBPathTIK="<<m_strSQLiteDBPathTIK;
	LOG_INFO<<" "<<"m_strSQLiteDBFileNameTIK="<<m_strSQLiteDBFileNameTIK;
	LOG_INFO<<" "<<"m_strSQLiteDBFileNameFullPathTIK="<<m_strSQLiteDBFileNameFullPathTIK;

	LOG_INFO<<" "<<"m_strMysqlSchema="<<m_strMysqlSchema;
	LOG_INFO<<" "<<"m_strMysqlIP="<<m_strMysqlIP;
	LOG_INFO<<" "<<"m_strMysqlUser="<<m_strMysqlUser;
	LOG_INFO<<" "<<"m_strMysqlPWD="<<m_strMysqlPWD;



	return;
}

void CDBOperParam::setValue(TA_Base_App::DbServerType nDataSrcDbType, unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();

	std::ostringstream sreaamTmp;
	std::string strTmp;

	sreaamTmp.str("");
	sreaamTmp<<nInstrumentID;
	strTmp = sreaamTmp.str();


	//
	m_nInstrumentID = nInstrumentID;
	m_strInstrumentID = strTmp;//3320

	if (nDataSrcDbType == TA_Base_App::enumSqliteDb)
	{
		m_strDataSrcDbType = DEF_VALUE_STRING_DB_SQLITE;
		m_nDataSrcDbType = TA_Base_App::enumSqliteDb;
		m_strQTDataSrcDbTypeName = DEF_VALUE_STRING_QT_SUPPORT_DB_QSQLITE;

		m_strSQLiteDBPathBar = DEFVALUE_String_SQLITE_DbPath_Bar;
		m_strSQLiteDBFileNameBAR = _GetSQLITEDBFileNameBar(m_nInstrumentID);
		m_strSQLiteDBFileNameFullPathBAR = _GetSQLITEDBFileNameFullPathBar(m_strSQLiteDBPathBar, m_nInstrumentID);

		m_strSQLiteDBPathTIK = DEFVALUE_String_SQLITE_DbPath_TIK;
		m_strSQLiteDBFileNameTIK = _GetSQLITEDBFileNameTIK(m_nInstrumentID);
		m_strSQLiteDBFileNameFullPathTIK = _GetSQLITEDBFileNameFullPathTIK(m_strSQLiteDBPathTIK, m_nInstrumentID);

	}
	else
	{
		m_strDataSrcDbType = DEF_VALUE_STRING_DB_MYSQL;
		m_nDataSrcDbType = TA_Base_App::enumMysqlDb;
		m_strQTDataSrcDbTypeName = DEF_VALUE_STRING_QT_SUPPORT_DB_QMYSQL;

		m_strMysqlSchema = DEFVALUE_String_Mysql_Schema;
		m_strMysqlIP = DEFVALUE_String_Mysql_IP;
		m_strMysqlUser = DEFVALUE_String_Mysql_User;
		m_strMysqlPWD = DEFVALUE_String_Mysql_PWD;
	}

	return;
}


void CDBOperParam::setValueSqliteDbPathBAR(const std::string& strSQLiteDBPathBar)
{
	BOOST_LOG_FUNCTION();

	m_strSQLiteDBPathBar = strSQLiteDBPathBar;
	m_strSQLiteDBFileNameBAR = _GetSQLITEDBFileNameBar(m_nInstrumentID);
	m_strSQLiteDBFileNameFullPathBAR = _GetSQLITEDBFileNameFullPathBar(m_strSQLiteDBPathBar, m_nInstrumentID);

	return;
}

void CDBOperParam::setValueSqliteDbPathTIK(const std::string& strSQLiteDBPathTIK)
{
	BOOST_LOG_FUNCTION();

	m_strSQLiteDBPathTIK = strSQLiteDBPathTIK;
	m_strSQLiteDBFileNameTIK = _GetSQLITEDBFileNameTIK(m_nInstrumentID);
	m_strSQLiteDBFileNameFullPathTIK = _GetSQLITEDBFileNameFullPathTIK(m_strSQLiteDBPathTIK, m_nInstrumentID);

	return;
}



std::string CDBOperParam::_GetSQLITEDBFileNameBar(unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strInstrumentSQLDBFileName;
	sreaamTmp.str("");

	//SQLiteDB_3306.db
	//oms
	switch (m_nDataSrcDbType)
	{
	case TA_Base_App::enumSqliteDb:
		sreaamTmp<<DEFVALUE_String_SQLITE_DbFile_Bar_Header<<nInstrumentID<<DEFVALUE_String_SQLITE_DbFile_Bar_Tail;
		break;
	default:
		sreaamTmp<<DEFVALUE_String_SQLITE_DbFile_Bar_Header<<nInstrumentID<<DEFVALUE_String_SQLITE_DbFile_Bar_Tail;
		break;
	}

	strInstrumentSQLDBFileName = sreaamTmp.str();

	return strInstrumentSQLDBFileName;
}


std::string CDBOperParam::_GetSQLITEDBFileNameTIK(unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strInstrumentSQLDBFileName;
	sreaamTmp.str("");

	//SQLiteDB_3306.db
	//oms
	switch (m_nDataSrcDbType)
	{
	case TA_Base_App::enumSqliteDb:
		sreaamTmp<<DEFVALUE_String_SQLITE_DbFile_Tick_Header<<nInstrumentID<<DEFVALUE_String_SQLITE_DbFile_Tick_Tail;
		break;
	default:
		sreaamTmp<<DEFVALUE_String_SQLITE_DbFile_Tick_Header<<nInstrumentID<<DEFVALUE_String_SQLITE_DbFile_Tick_Tail;
		break;
	}

	strInstrumentSQLDBFileName = sreaamTmp.str();

	return strInstrumentSQLDBFileName;
}


std::string CDBOperParam::_GetSQLITEDBFileNameFullPathBar(const std::string& strSQLiteDBPath, unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQLiteDBFileNameFullPath;//D://SaveDataBar//3306.BAR
	std::string strSQLiteDBFileName;//3306.BAR

	strSQLiteDBFileName = _GetSQLITEDBFileNameBar(nInstrumentID);
	sreaamTmp.str("");

	//SQLiteDB_3306.db
	//oms
	switch (m_nDataSrcDbType)
	{
	case TA_Base_App::enumSqliteDb:
		sreaamTmp<<strSQLiteDBPath<<"//"<<strSQLiteDBFileName;
		break;
	default:
		sreaamTmp<<strSQLiteDBPath<<"//"<<strSQLiteDBFileName;
		break;
	}

	strSQLiteDBFileNameFullPath = sreaamTmp.str();

	return strSQLiteDBFileNameFullPath;
}



std::string CDBOperParam::_GetSQLITEDBFileNameFullPathTIK(const std::string& strSQLiteDBPath, unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQLiteDBFileNameFullPath;//D://SaveDataBar//3306.BAR
	std::string strSQLiteDBFileName;//3306.BAR

	strSQLiteDBFileName = _GetSQLITEDBFileNameTIK(nInstrumentID);
	sreaamTmp.str("");

	//SQLiteDB_3306.db
	//oms
	switch (m_nDataSrcDbType)
	{
	case TA_Base_App::enumSqliteDb:
		sreaamTmp<<strSQLiteDBPath<<"//"<<strSQLiteDBFileName;
		break;
	default:
		sreaamTmp<<strSQLiteDBPath<<"//"<<strSQLiteDBFileName;
		break;
	}

	strSQLiteDBFileNameFullPath = sreaamTmp.str();

	return strSQLiteDBFileNameFullPath;
}

NS_END(TA_Base_App)
















