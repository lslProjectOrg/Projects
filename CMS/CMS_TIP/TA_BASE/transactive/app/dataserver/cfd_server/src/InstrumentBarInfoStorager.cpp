#include "CFDServerCommonData.h"
#include "InstrumentBarInfoStorager.h"
#include "MarketData.h"
#include "BarCalculator.h"
#include "CFDServerUtilityFun.h"

#include <boost/algorithm/string.hpp>

#include "BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_Base_App;


boost::mutex g_mutex_Database_Mysql;
bool g_bool_NeedToCheckAndInditMysqlDb = false;
QSqlDatabase*	g_SqlDataBase_Mysql = NULL;


NS_BEGIN(TA_Base_App)


//////////////////////////////////////////////////////////////////////////
// static const std::string str_Mysql_DBName="oms";
// static const std::string str_Mysql_IP="192.168.1.1";
// static const std::string str_Mysql_User="jadeserver";
// static const std::string str_Mysql_PWD="JadeServer123";

static const std::string str_Mysql_DBName="oms";
static const std::string str_Mysql_IP="127.0.0.1";
static const std::string str_Mysql_User="root";
static const std::string str_Mysql_PWD="root";

static const std::string str_Table_bar_data_5second = "bar_data_5second";
static const std::string str_Table_bar_data_5min = "bar_data_5min";
static const std::string str_Table_bar_data_15min = "bar_data_15min";
static const std::string str_Table_bar_data_1min = "bar_data_1min";
static const std::string str_Table_bar_data_30min = "bar_data_30min";
static const std::string str_Table_bar_data_60min = "bar_data_60min";
static const std::string str_Table_bar_data_1day = "bar_data_1day";
static const std::string str_Table_bar_data_30day = "bar_data_30day";
static const std::string str_Table_bar_data_1year = "bar_data_1year";

static const std::string str_Table_tick_data = "tick_data";



static const std::string str_SQliteDb_Instrument_BAR_DB_header = "SQLiteDB_";//SQLiteDB_3320.db

static const std::string str_BarData_Column_InstrumentID = "InstrumentID";
static const std::string str_BarData_Column_Timestamp = "Timestamp";
static const std::string str_BarData_Column_Open = "Open";
static const std::string str_BarData_Column_Close = "Close";
static const std::string str_BarData_Column_High = "High";
static const std::string str_BarData_Column_Low = "Low";
static const std::string str_BarData_Column_Volume = "Volume";
static const std::string str_BarData_Column_LastModified = "LastModified";


static const std::string str_BarData_Column_InstrumentID_Value = ":InstrumentID_Value";
static const std::string str_BarData_Column_Timestamp_Value = ":Timestamp_Value";
static const std::string str_BarData_Column_Open_Value = ":Open_Value";
static const std::string str_BarData_Column_Close_Value = ":Close_Value";
static const std::string str_BarData_Column_High_Value = ":High_Value";
static const std::string str_BarData_Column_Low_Value = ":Low_Value";
static const std::string str_BarData_Column_Volume_Value = ":Volume_Value";
////
static const std::string str_TickData_Column_InstrumentID = "InstrumentID";
static const std::string str_TickData_Column_Timestamp = "Timestamp";
static const std::string str_TickData_Column_BidPx = "BidPx";
static const std::string str_TickData_Column_AskPx = "AskPx";
static const std::string str_TickData_Column_LastPx = "LastPx";
static const std::string str_TickData_Column_BidVol = "BidVol";
static const std::string str_TickData_Column_AskVol = "AskVol";
static const std::string str_TickData_Column_LastVol = "LastVol";

static const std::string str_TickData_Column_InstrumentID_Value = ":InstrumentID_Value";
static const std::string str_TickData_Column_Timestamp_Value = ":Timestamp_Value";
static const std::string str_TickData_Column_BidPx_Value = ":BidPx_Value";
static const std::string str_TickData_Column_AskPx_Value = ":AskPx_Value";
static const std::string str_TickData_Column_LastPx_Value = ":LastPx_Value";
static const std::string str_TickData_Column_BidVol_Value = ":BidVol_Value";
static const std::string str_TickData_Column_AskVol_Value = ":AskVol_Value";
static const std::string str_TickData_Column_LastVol_Value = ":LastVol_Value";



CInstrumentBarInfoStorager::CInstrumentBarInfoStorager( const CInstrumentBarInfoRequest& instrumentBarInfoRequest, unsigned int nInstrumentID )
{
	BOOST_LOG_FUNCTION();
	m_nInstrumentID = nInstrumentID;
	m_InstrumentBarInfoRequest = instrumentBarInfoRequest;

	m_pUtilityFun = new CCFDServerUtilityFun();
	m_pMapIntervalDBTableName = new MapIntervalDBTableNameT();
	m_pLstInstrumentTickInfo = new LstInstrumentTickInfoT();

	m_strDBType = m_InstrumentBarInfoRequest.m_strDbType;
	m_nDBType = m_InstrumentBarInfoRequest.m_nDBType;
	
	m_strDBName = _GetBarInfoDBName(m_InstrumentBarInfoRequest.m_strSaveDataDirectory, m_nInstrumentID);

	m_pQSqlDataBase = NULL;
	m_pQSqlQueryForSelect = NULL;
	m_pQSqlQueryForInseert = NULL;
	m_pMapIntervalBarLst = new MapIntervalBarLstT();
	m_pMapIntervalBarLst->clear();
	_InitMapIntervalBarInfoLst(m_InstrumentBarInfoRequest, m_pMapIntervalBarLst);
	
	m_nBatchSize = 20000;//TODO.
	m_nBuffNum = 0;

	_InitDataBase();

	if (enumMysqlDb == m_nDBType)
	{
		boost::mutex::scoped_lock lock(g_mutex_Database_Mysql);
		if (g_bool_NeedToCheckAndInditMysqlDb)
		{
			_CheckAndInitDBTable(m_InstrumentBarInfoRequest);
		}
	}
	else
	{
		_CheckAndInitDBTable(m_InstrumentBarInfoRequest);
	}
}

CInstrumentBarInfoStorager::~CInstrumentBarInfoStorager(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pMapIntervalBarLst)
	{
		_StartStransactionForMapBarInfoLstBatchMode(m_pMapIntervalBarLst);
		_ClearMapIntervalBarLst(m_pMapIntervalBarLst);
		m_pMapIntervalBarLst->clear();
		delete m_pMapIntervalBarLst;
		m_pMapIntervalBarLst = NULL;
	}

	if (NULL != m_pLstInstrumentTickInfo)
	{
		_StartStransactionForLstTickInfoBatchMode(m_pLstInstrumentTickInfo);
		_ClearLstInstrumentTickInfoT(m_pLstInstrumentTickInfo);//just clean data
		m_pLstInstrumentTickInfo->clear();
		delete m_pLstInstrumentTickInfo;
		m_pLstInstrumentTickInfo = NULL;
	}

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}

	if (NULL != m_pMapIntervalDBTableName)
	{
		m_pMapIntervalDBTableName->clear();
		delete m_pMapIntervalDBTableName;
		m_pMapIntervalDBTableName = NULL;
	}

	_UnInitDataBase();


}



int CInstrumentBarInfoStorager::_InitMapIntervalBarInfoLst(const CInstrumentBarInfoRequest& instrumentBarInfoRequest, MapIntervalBarLstT* pMapIntervalBarLstT)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::list<int> lstBarTime;
	std::list<int>::iterator  iterLst;
	int nInterval = 0;
	LstInstrumentBarInfoT* pLstInstrumentBarInfo = NULL;


	m_InstrumentBarInfoRequest.getLstBarTime(lstBarTime);
	iterLst = lstBarTime.begin();

	while (iterLst != lstBarTime.end())
	{
		nInterval = (*iterLst);
		pLstInstrumentBarInfo = new LstInstrumentBarInfoT();
		pMapIntervalBarLstT->insert(MapIntervalBarLstValueTypeT(nInterval, pLstInstrumentBarInfo));
		pLstInstrumentBarInfo = NULL;

		iterLst++;
	}//while

	lstBarTime.clear();

	return nFunRes;
}
void CInstrumentBarInfoStorager::_InitDataBase()
{
	bool bExecRes = false;
	std::string strDBFileName;
	boost::mutex::scoped_lock lock(g_mutex_Database_Mysql);


	_UnInitDataBase();
	
	//SQLiteDB_3306.db
	//mysqldb_3306
	switch (m_nDBType)
	{
	case TA_Base_App::enumSqliteDb:
		//SQLiteDB_3306.db
		strDBFileName = _GetBarInfoDBFileName(m_nInstrumentID);
		//SQLiteDB_3306.db
		m_pQSqlDataBase = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", QLatin1String(strDBFileName.c_str())));
		//d://savedata//SQLiteDB_3306.db
		m_pQSqlDataBase->setDatabaseName(m_strDBName.c_str());

		LOG_INFO<<"new Database  m_nDBType="<<m_nDBType<<" QSQLITE db="<<m_strDBName;

		break;
	case TA_Base_App::enumMysqlDb:
		if (NULL != g_SqlDataBase_Mysql)
		{
			m_pQSqlDataBase = g_SqlDataBase_Mysql;
			g_bool_NeedToCheckAndInditMysqlDb = false;
		}
		else
		{
			m_pQSqlDataBase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
			m_pQSqlDataBase->setDatabaseName(m_strDBName.c_str());//oms
			m_pQSqlDataBase->setHostName(str_Mysql_IP.c_str());
			m_pQSqlDataBase->setUserName(str_Mysql_User.c_str());
			m_pQSqlDataBase->setPassword(str_Mysql_PWD.c_str());
			g_SqlDataBase_Mysql = m_pQSqlDataBase;
			g_bool_NeedToCheckAndInditMysqlDb = true;

			LOG_INFO<<"new Database  m_nDBType="<<m_nDBType<<" QMYSQL db="<<m_strDBName
				<<" IP="<<str_Mysql_IP
				<<" User="<<str_Mysql_User
				<<" Pwd="<<str_Mysql_PWD;
		}

		break;
	default:
		m_strDBType = defSQLiteDBName;
		m_nDBType = TA_Base_App::enumSqliteDb;
		//SQLiteDB_3306.db
		strDBFileName = _GetBarInfoDBFileName(m_nInstrumentID);
		m_pQSqlDataBase = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", QLatin1String(strDBFileName.c_str())));
		m_pQSqlDataBase->setDatabaseName(m_strDBName.c_str());
		break;
	}

	bExecRes = m_pQSqlDataBase->open();
	if (!bExecRes)
	{
		LOG_ERROR<<"Fail to open db:"<<m_strDBName<<" error:"<<m_pQSqlDataBase->lastError().text().toStdString();
	}


	//check
	if (false == m_pQSqlDataBase->isValid())
	{
		LOG_ERROR<<"db="<<m_strDBName<<" is not Valid";
	}
	else
	{
		LOG_INFO<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<" is Valid";
	}

	if (false == m_pQSqlDataBase->driver()->hasFeature(QSqlDriver::Transactions)) 
	{
		LOG_INFO<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<" not support Transactions";
	}
	else
	{
		LOG_INFO<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<" support Transactions";

	}
}

void CInstrumentBarInfoStorager::_UnInitDataBase()
{
	if (NULL != m_pQSqlQueryForSelect)
	{
		delete m_pQSqlQueryForSelect;
		m_pQSqlQueryForSelect = NULL;
	}

	if (NULL != m_pQSqlQueryForInseert)
	{
		delete m_pQSqlQueryForInseert;
		m_pQSqlQueryForInseert = NULL;
	}	

	if (m_nDBType == enumMysqlDb)
	{
		m_pQSqlDataBase = NULL;
	}
	else
	{
		if (NULL != m_pQSqlDataBase)
		{
			LOG_INFO<<"delete Database  m_nDBType="<<m_nDBType<<" QSQLITE db="<<m_strDBName;

			m_pQSqlDataBase->close();
			delete m_pQSqlDataBase;
			m_pQSqlDataBase = NULL;
			//QSqlDatabase::removeDatabase(m_strDBName.c_str());
		}
	}
}


std::string CInstrumentBarInfoStorager::_GetBarInfoDBFileName(unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strInstrumentSQLDBFileName;
	sreaamTmp.str("");

	//SQLiteDB_3306.db
	//oms
	switch (m_nDBType)
	{
	case TA_Base_App::enumSqliteDb:
		sreaamTmp<<str_SQliteDb_Instrument_BAR_DB_header<<nInstrumentID<<".db";
		break;
	case TA_Base_App::enumMysqlDb:
		sreaamTmp<<str_Mysql_DBName;
		break;
	default:
		sreaamTmp<<str_SQliteDb_Instrument_BAR_DB_header<<nInstrumentID<<".db";
		break;
	}

	strInstrumentSQLDBFileName = sreaamTmp.str();

	return strInstrumentSQLDBFileName;
}

std::string CInstrumentBarInfoStorager::_GetBarInfoDBName(const std::string& strPathInstrumentBarInfoTotal, unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strInstrumentSQLDBName;
	std::string strDBFileName;

	strDBFileName = _GetBarInfoDBFileName(nInstrumentID);
	sreaamTmp.str("");

	//SQLiteDB_3306.db
	//oms
	switch (m_nDBType)
	{
	case TA_Base_App::enumSqliteDb:
		sreaamTmp<<strPathInstrumentBarInfoTotal<<"//"<<strDBFileName;
		break;
	case TA_Base_App::enumMysqlDb:
		sreaamTmp<<str_Mysql_DBName;
		break;
	default:
		sreaamTmp<<strPathInstrumentBarInfoTotal<<"//"<<strDBFileName;
		break;
	}

	strInstrumentSQLDBName = sreaamTmp.str();

	return strInstrumentSQLDBName;
}

std::string CInstrumentBarInfoStorager::_GetBarInfoDBTableName(int interval)
{
	BOOST_LOG_FUNCTION();	
	std::string strBarDataTableName;
	std::ostringstream sreaamTmp;

	switch (interval)
	{
	case TIME_BASE_S_5S:
		sreaamTmp<<str_Table_bar_data_5second;
		break;
	case TIME_BASE_S_1MIN:
		sreaamTmp<<str_Table_bar_data_1min;
		break;
	case TIME_BASE_S_5MIN:
		sreaamTmp<<str_Table_bar_data_5min;
		break;
	case TIME_BASE_S_15MIN:
		sreaamTmp<<str_Table_bar_data_15min;
		break;
	case TIME_BASE_S_30MIN:
		sreaamTmp<<str_Table_bar_data_30min;
		break;
	case TIME_BASE_S_1HOUR:
		sreaamTmp<<str_Table_bar_data_60min;
		break;
	case TIME_BASE_S_1DAY:
		sreaamTmp<<str_Table_bar_data_1day;
		break;
	case TIME_BASE_S_1MON:
		sreaamTmp<<str_Table_bar_data_30day;
		break;
	case TIME_BASE_S_1YEAR:
		sreaamTmp<<str_Table_bar_data_1year;
		break;
	default:
		sreaamTmp<<"bar_data_"<<interval;
		break;
	}

	strBarDataTableName = sreaamTmp.str();
	return strBarDataTableName;
}

std::string CInstrumentBarInfoStorager::_GetTickInfoDBTableName(unsigned int strInstrumentID)
{
	BOOST_LOG_FUNCTION();	
	std::string strTableName;
	std::ostringstream sreaamTmp;
	sreaamTmp<<str_Table_tick_data;

	strTableName = sreaamTmp.str();
	return strTableName;
}

int CInstrumentBarInfoStorager::_CheckAndInitDBTable(CInstrumentBarInfoRequest& instrumentBarInfoRequest)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::string strTableName;
	std::list<int> lstBarTime;
	std::list<int>::iterator  iterLst;
	int nInterval = 0;

	m_InstrumentBarInfoRequest.getLstBarTime(lstBarTime);
	iterLst = lstBarTime.begin();

	while (iterLst != lstBarTime.end())
	{
		nInterval = (*iterLst);

		strTableName = _GetBarInfoDBTableName(nInterval);
		_CreateDBTableForBarInfo(strTableName);
		m_pMapIntervalDBTableName->insert(MapIntervalDBTableNameValueTypeT(nInterval, strTableName));

		iterLst++;
	}//while
	lstBarTime.clear();


	strTableName.clear();
	strTableName = _GetTickInfoDBTableName(m_nInstrumentID);
	_CreateDBTableForTickInfo(strTableName);

	return nFunRes;
}


std::string CInstrumentBarInfoStorager::_BuildSQLForCreateDBTableBarInfo(const std::string& strTableName)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;

	/*
	enumMysqlDb
	CREATE TABLE IF NOT EXISTS bar_data_1day
	(
	`InstrumentID` int(10) unsigned NOT NULL,
	`Timestamp` datetime NOT NULL,
	`Open` decimal(25,10) NOT NULL,
	`Close` decimal(25,10) NOT NULL,
	`High` decimal(25,10) NOT NULL,
	`Low` decimal(25,10) NOT NULL,
	`Volume` int(10) unsigned NOT NULL,
	`LastModified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	PRIMARY KEY (`InstrumentID`,`Timestamp`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	//TIMESTR--%Y%m%d%H%M%S
	//%04d-%02d-%02d %02d:%02d:%02d
	*/

	/*
	enumSqliteDb
	CREATE TABLE IF NOT EXISTS bar_data_15min
	(
	InstrumentID INTEGER NOT NULL, 
	Timestamp TIMESTAMP NOT NULL, 
	Open decimal(25,10) NOT NULL,
	Close decimal(25,10) NOT NULL,
	High decimal(25,10) NOT NULL,	
	Low decimal(25,10) NOT NULL,
	Volume NUMBER,
	PRIMARY KEY (InstrumentID, Timestamp)
	)
	*/


	if (TA_Base_App::enumMysqlDb == m_nDBType)
	{
		sreaamTmp.str("");
		sreaamTmp<<"CREATE TABLE IF NOT EXISTS "<<strTableName
			<<" "<<"("
			<<" "<<str_BarData_Column_InstrumentID<<" "<<"int(10) unsigned NOT NULL"<<","
			<<" "<<str_BarData_Column_Timestamp<<" "<<"datetime NOT NULL"<<","
			<<" "<<str_BarData_Column_Open<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_Close<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_High<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_Low<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_Volume<<" "<<"int(10) unsigned NOT NULL"<<","
			<<" "<<str_BarData_Column_LastModified<<" "<<"timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP"<<","
			<<" "<<"PRIMARY KEY ("<<str_BarData_Column_InstrumentID<<", "<<str_BarData_Column_Timestamp<<")"
			<<" "<<")";
	}
	//else if (TA_Base_App::enumSqliteDb == m_nDBType)
	else
	{
		sreaamTmp.str("");
		sreaamTmp<<"CREATE TABLE IF NOT EXISTS "<<strTableName
			<<" "<<"("
			<<" "<<str_BarData_Column_InstrumentID<<" "<<"INTEGER NOT NULL"<<","
			<<" "<<str_BarData_Column_Timestamp<<" "<<"TIMESTAMP NOT NULL"<<","
			<<" "<<str_BarData_Column_Open<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_Close<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_High<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_Low<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_Volume<<" "<<"int NOT NULL"<<","
			<<" "<<"PRIMARY KEY ("<<str_BarData_Column_InstrumentID<<", "<<str_BarData_Column_Timestamp<<")"
			<<" "<<")";
	}
	



	strSQL = sreaamTmp.str();
	return strSQL;
}


std::string CInstrumentBarInfoStorager::_BuildSQLForCreateDBTableTickInfo(const std::string& strTableName)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;


	static const std::string str_Tick_Column_InstrumentID = "InstrumentID";
	static const std::string str_Tick_Column_Timestamp = "Timestamp";
	static const std::string str_Tick_Column_BidPx = "BidPx";
	static const std::string str_Tick_Column_AskPx = "AskPx";
	static const std::string str_Tick_Column_LastPx = "LastPx";
	static const std::string str_Tick_Column_BidVol = "BidVol";
	static const std::string str_Tick_Column_AskVol = "AskVol";
	static const std::string str_Tick_Column_LastVol = "LastVol";

	/*
	enumMysqlDb
	CREATE TABLE IF NOT EXISTS tick_data
	(
	`InstrumentID` int(10) unsigned NOT NULL,
	`Timestamp` datetime NOT NULL,
	`BidPx` decimal(25,10) NOT NULL,
	`AskPx` decimal(25,10) NOT NULL,
	`LastPx` decimal(25,10) NOT NULL,
	`BidVol` int(10) unsigned NOT NULL,
	`AskVol` int(10) unsigned NOT NULL,
	`LastVol` int(10) unsigned NOT NULL,
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	//TIMESTR--%Y%m%d%H%M%S
	//%04d-%02d-%02d %02d:%02d:%02d
	*/

	/*
	enumSqliteDb
	CREATE TABLE IF NOT EXISTS tick_data
	(
	InstrumentID INTEGER NOT NULL,
	Timestamp TIMESTAMP NOT NULL,
	BidPx decimal(25,10) NOT NULL,
	AskPx decimal(25,10) NOT NULL,
	LastPx decimal(25,10) NOT NULL,
	BidVol int NOT NULL,
	AskVol int NOT NULL,
	LastVol int NOT NULL,
	)
	*/


	if (TA_Base_App::enumMysqlDb == m_nDBType)
	{
		sreaamTmp.str("");
		sreaamTmp<<"CREATE TABLE IF NOT EXISTS "<<strTableName
			<<" "<<"("
			<<" "<<str_Tick_Column_InstrumentID<<" "<<"int(10) unsigned NOT NULL"<<","
			<<" "<<str_Tick_Column_Timestamp<<" "<<"datetime NOT NULL"<<","
			<<" "<<str_Tick_Column_BidPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_Tick_Column_AskPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_Tick_Column_LastPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_Tick_Column_BidVol<<" "<<"int(10) unsigned NOT NULL"<<","
			<<" "<<str_Tick_Column_AskVol<<" "<<"int(10) unsigned NOT NULL"<<","
			<<" "<<str_Tick_Column_LastVol<<" "<<"int(10) unsigned NOT NULL"
			<<" "<<")";		

	}
	//else if (TA_Base_App::enumSqliteDb == m_nDBType)
	else
	{
		sreaamTmp.str("");
		sreaamTmp<<"CREATE TABLE IF NOT EXISTS "<<strTableName
			<<" "<<"("
			<<" "<<str_Tick_Column_InstrumentID<<" "<<"INTEGER NOT NULL"<<","
			<<" "<<str_Tick_Column_Timestamp<<" "<<"TIMESTAMP NOT NULL"<<","
			<<" "<<str_Tick_Column_BidPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_Tick_Column_AskPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_Tick_Column_LastPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_Tick_Column_BidVol<<" "<<"int NOT NULL"<<","
			<<" "<<str_Tick_Column_AskVol<<" "<<"int NOT NULL"<<","
			<<" "<<str_Tick_Column_LastVol<<" "<<"int NOT NULL"
			<<" "<<")";
	}
	
	strSQL = sreaamTmp.str();
	return strSQL;
}

int CInstrumentBarInfoStorager::_CreateDBTableForBarInfo(const std::string& strDbTableName)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = true;
	std::string strSQL;

	QSqlQuery query(*m_pQSqlDataBase);

	strSQL = _BuildSQLForCreateDBTableBarInfo(strDbTableName);

	LOG_DEBUG<<"exec strSQL="<<strSQL;
	bExecRes = query.exec(strSQL.c_str());
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"Fail to exec sql:"<<strSQL<<" error:"<<query.lastError().text().toStdString();
	}

	return nFunRes;
}


int CInstrumentBarInfoStorager::_CreateDBTableForTickInfo(const std::string& strDbTableName)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = true;
	std::string strSQL;

	QSqlQuery query(*m_pQSqlDataBase);

	strSQL = _BuildSQLForCreateDBTableTickInfo(strDbTableName);

	LOG_DEBUG<<"exec strSQL="<<strSQL;
	bExecRes = query.exec(strSQL.c_str());
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"Fail to exec sql:"<<strSQL<<" error:"<<query.lastError().text().toStdString();
	}

	return nFunRes;
}

std::string CInstrumentBarInfoStorager::_BuildDropDBTableSQL(const std::string& strDbTableName)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;
	
	/*DROP TABLE Table_BarInfo*/
	sreaamTmp.str("");
	sreaamTmp<<"DROP TABLE "<<strDbTableName;

	strSQL = sreaamTmp.str();
	return strSQL;
}


std::string CInstrumentBarInfoStorager::_BuildSQLForInsertBarInfoBatchMode(const std::string& strTableName)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;

	/*
	INSERT INTO bar_data_15min
	(
	InstrumentID, 
	Timestamp, 
	Open, 
	Close, 
	High,
	Low, 
	Volume
	) 
	VALUES 
	(
	:InstrumentID_Value, 
	:Timestamp_Value,
	:Open_Value,
	:Close_Value,
	:High_Value,
	:Low_Value,
	:Volume_Value 
	);
	*/

	sreaamTmp.str("");
	sreaamTmp<<"INSERT INTO "<<strTableName
		<<" "<<"("
		<<" "<<str_BarData_Column_InstrumentID<<","
		<<" "<<str_BarData_Column_Timestamp<<","
		<<" "<<str_BarData_Column_Open<<","
		<<" "<<str_BarData_Column_Close<<","
		<<" "<<str_BarData_Column_High<<","
		<<" "<<str_BarData_Column_Low<<","
		<<" "<<str_BarData_Column_Volume
		<<" "<<")"
		<<" "<<"VALUES"
		<<" "<<"("
		<<" "<<"?"<<","
		<<" "<<"?"<<","
		<<" "<<"?"<<","
		<<" "<<"?"<<","
		<<" "<<"?"<<","
		<<" "<<"?"<<","
		<<" "<<"?"
		<<" "<<")";


	strSQL = sreaamTmp.str();
	return strSQL;	
}


std::string CInstrumentBarInfoStorager::_BuildSQLForInsertTickInfoBatchMode(const std::string& strTableName)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;	

	/*
	INSERT INTO tick_data
	(
	InstrumentID, 
	Timestamp, 
	BidPx, 
	AskPx, 
	LastPx, 
	BidVol, 
	AskVol, 
	LastVol
	) 
	VALUES 
	(
	:InstrumentID_Value, 
	:Timestamp_Value,
	:BidPx_Value,
	:AskPx_Value,
	:LastPx_Value,
	:BidVol_Value,
	:AskVol_Value,
	:LastVol_Value 
	);
	*/

	sreaamTmp.str("");
	sreaamTmp<<"INSERT INTO "<<strTableName
		<<" "<<"("
		<<" "<<str_TickData_Column_InstrumentID<<","
		<<" "<<str_TickData_Column_Timestamp<<","
		<<" "<<str_TickData_Column_BidPx<<","
		<<" "<<str_TickData_Column_AskPx<<","
		<<" "<<str_TickData_Column_LastPx<<","
		<<" "<<str_TickData_Column_BidVol<<","
		<<" "<<str_TickData_Column_AskVol<<","
		<<" "<<str_TickData_Column_LastVol
		<<" "<<")"
		<<" "<<"VALUES"
		<<" "<<"("
		<<" "<<"?"<<","
		<<" "<<"?"<<","
		<<" "<<"?"<<","
		<<" "<<"?"<<","
		<<" "<<"?"<<","
		<<" "<<"?"<<","
		<<" "<<"?"<<","
		<<" "<<"?"
		<<" "<<")";


	strSQL = sreaamTmp.str();
	return strSQL;	
}


std::string CInstrumentBarInfoStorager::_BuildSelectSQL(const std::string& strTableName)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;

	/*
	SELECT 
	InstrumentID, 
	Timestamp, 
	Open, 
	Close, 
	High,
	Low, 
	Volume
	FROM bar_data_15min
	order by Timestamp
	;
	*/

	sreaamTmp.str("");
	sreaamTmp<<"SELECT"
		<<" "<<str_BarData_Column_InstrumentID<<","
		<<" "<<str_BarData_Column_Timestamp<<","
		<<" "<<str_BarData_Column_Open<<","
		<<" "<<str_BarData_Column_Close<<","
		<<" "<<str_BarData_Column_High<<","
		<<" "<<str_BarData_Column_Low<<","
		<<" "<<str_BarData_Column_Volume
		<<" "<<"FROM"<<" "<<strTableName
		<<" "<<"ORDER BY"<<" "<<str_BarData_Column_Timestamp;

	strSQL = sreaamTmp.str();
	return strSQL;	
}



int CInstrumentBarInfoStorager::storeBarInfo(int interval, Bar& barInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	MapIntervalBarLstIterT iterMap;
	CInstrumentBarInfo* pInstrumentBarInfo = NULL;
	LstInstrumentBarInfoT* pList = NULL;

	m_nBuffNum++;

	pInstrumentBarInfo = new CInstrumentBarInfo(interval, barInfo);
	
	iterMap = m_pMapIntervalBarLst->find(interval);
	if (iterMap != m_pMapIntervalBarLst->end())
	{
		pList = iterMap->second;
		pList->push_back(pInstrumentBarInfo);
	}

	if (m_nBuffNum < m_nBatchSize)
	{
		return nFunRes;
	}

	nFunRes = _StartStransactionForMapBarInfoLstBatchMode(m_pMapIntervalBarLst);
	_ClearMapIntervalBarLst(m_pMapIntervalBarLst);
	m_pMapIntervalBarLst->clear();
	nFunRes = _StartStransactionForLstTickInfoBatchMode(m_pLstInstrumentTickInfo);
	_ClearLstInstrumentTickInfoT(m_pLstInstrumentTickInfo);//just clean data
	m_pLstInstrumentTickInfo->clear();

	m_nBuffNum = 0;

	return nFunRes;
}


int CInstrumentBarInfoStorager::storeTickInfo(CMyTick& tickInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CInstrumentTickInfo* pInstrumentTickInfo = NULL;

	m_nBuffNum++;

	pInstrumentTickInfo = new CInstrumentTickInfo(tickInfo);

	m_pLstInstrumentTickInfo->push_back(pInstrumentTickInfo);
	pInstrumentTickInfo = NULL;


	return nFunRes;
}

void CInstrumentBarInfoStorager::setStoreBatchSize(unsigned int nBatchSize)
{
	BOOST_LOG_FUNCTION();
	m_nBatchSize = nBatchSize;
}




int CInstrumentBarInfoStorager::_StartStransactionForMapBarInfoLstBatchMode(MapIntervalBarLstT*  pMapIntervalBarLst)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	MapIntervalBarLstIterT iterMap;
	int nInterval = 0;
	LstInstrumentBarInfoT* pLstInstrumentBarInfo = NULL;
	bool bStartTransaction = false;

	bStartTransaction = false;
	//if (!QSqlDatabase::database().driver()->hasFeature(QSqlDriver::Transactions)) 
	if (false == m_pQSqlDataBase->driver()->hasFeature(QSqlDriver::Transactions)) 
	{
		bStartTransaction = false;
		LOG_INFO<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<" not support Transactions";
	}
	else
	{
		if (false == m_pQSqlDataBase->transaction())
		//if (false == QSqlDatabase::database().transaction())
		{
			LOG_ERROR<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<" support Transactions but start transaction error!"
				<<" error: "<<QSqlDatabase::database().lastError().text().toStdString();
		}
		else
		{
			LOG_INFO<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<"start Transaction";
			bStartTransaction = true;
		}
	}



	iterMap = pMapIntervalBarLst->begin();
	while (iterMap != pMapIntervalBarLst->end())
	{
		nInterval = (iterMap->first);
		pLstInstrumentBarInfo = (iterMap->second);
		nFunRes = _StoreLstInstrumentBarInfoBatchMode(nInterval, pLstInstrumentBarInfo);
		_ClearLstInstrumentBarInfo(pLstInstrumentBarInfo);//just clean data
		iterMap++;
	}//while

	if (bStartTransaction)
	{
		if(false == m_pQSqlDataBase->commit())  
		//if(!QSqlDatabase::database().commit())  
		{  
			LOG_ERROR<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<" commit error!"
				<<" error: "<<QSqlDatabase::database().lastError().text().toStdString();

			if(false == m_pQSqlDataBase->rollback())  
			//if(!QSqlDatabase::database().rollback())  
			{  
				LOG_ERROR<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<" rollback error!"
					<<" error: "<<QSqlDatabase::database().lastError().text().toStdString();
			}//if 
		}//if
		else
		{
			LOG_INFO<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<"  commit Transaction";
		}
	}//if

	return nFunRes;
}

int CInstrumentBarInfoStorager::_StoreLstInstrumentBarInfoBatchMode(int nInterval, LstInstrumentBarInfoT*  pLstInstrumentBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = false;
	std::string strDBTableName;
	std::string strSQL;
	std::string strTimeStr;
	LstInstrumentBarInfoIterT iterLst;
	CInstrumentBarInfo* pInstrumentBarInfo = NULL;
	QVariantList lstInstrumentID;
	QVariantList lstTimestamp;
	QVariantList lstOpen;
	QVariantList lstClose;
	QVariantList lstHigh;
	QVariantList lstLow;
	QVariantList lstVolume;

	if (NULL != m_pQSqlQueryForInseert)
	{
		delete m_pQSqlQueryForInseert;
		m_pQSqlQueryForInseert = NULL;
	}

	m_pQSqlQueryForInseert = new QSqlQuery(*m_pQSqlDataBase);

	strDBTableName = _GetBarInfoDBTableName(nInterval);
	strSQL = _BuildSQLForInsertBarInfoBatchMode(strDBTableName);
	LOG_DEBUG<<"m_nInstrumentID="<<m_nInstrumentID<<" strSQL="<<strSQL<<" pLstInstrumentBarInfo.size="<<pLstInstrumentBarInfo->size();

	m_pQSqlQueryForInseert->prepare(strSQL.c_str());

	iterLst = pLstInstrumentBarInfo->begin();
	while (iterLst != pLstInstrumentBarInfo->end())
	{
		pInstrumentBarInfo = (*iterLst);

		lstInstrumentID<<m_nInstrumentID;
		strTimeStr = m_pUtilityFun->dataTimeToStr(pInstrumentBarInfo->m_BarInfo.Time);
		lstTimestamp << strTimeStr.c_str();
		lstOpen<<pInstrumentBarInfo->m_BarInfo.Open;
		lstClose<<pInstrumentBarInfo->m_BarInfo.Close;
		lstHigh<<pInstrumentBarInfo->m_BarInfo.High;
		lstLow<<pInstrumentBarInfo->m_BarInfo.Low;
		lstVolume<<pInstrumentBarInfo->m_BarInfo.Volume;

		iterLst++;
	}//while

	m_pQSqlQueryForInseert->addBindValue(lstInstrumentID);
	m_pQSqlQueryForInseert->addBindValue(lstTimestamp);
	m_pQSqlQueryForInseert->addBindValue(lstOpen);
	m_pQSqlQueryForInseert->addBindValue(lstClose);
	m_pQSqlQueryForInseert->addBindValue(lstHigh);
	m_pQSqlQueryForInseert->addBindValue(lstLow);
	m_pQSqlQueryForInseert->addBindValue(lstVolume);

	bExecRes = m_pQSqlQueryForInseert->execBatch();
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"execBatch strSQL="<<strSQL<<" pLstInstrumentBarInfo.size="<<pLstInstrumentBarInfo->size()
			<<" "<<"error:"<<m_pQSqlQueryForInseert->lastError().text().toStdString();
	}

	if (NULL != m_pQSqlQueryForInseert)
	{
		delete m_pQSqlQueryForInseert;
		m_pQSqlQueryForInseert = NULL;
	}
	return nFunRes;
}


void CInstrumentBarInfoStorager::_ClearLstInstrumentBarInfo(LstInstrumentBarInfoT* pLstInstrumentBarInfo)
{
	BOOST_LOG_FUNCTION();

	LstInstrumentBarInfoIterT iterLst;
	CInstrumentBarInfo* pData = NULL;

	iterLst = pLstInstrumentBarInfo->begin();
	while (iterLst != pLstInstrumentBarInfo->end())
	{
		pData = (*iterLst);

		delete pData;
		pData = NULL;

		iterLst++;
	}
	pLstInstrumentBarInfo->clear();

}



void CInstrumentBarInfoStorager::_ClearMapIntervalBarLst( MapIntervalBarLstT* pMapIntervalBarLstT )
{
	BOOST_LOG_FUNCTION();

	MapIntervalBarLstIterT iterMap;
	LstInstrumentBarInfoT* pData = NULL;

	iterMap = pMapIntervalBarLstT->begin();
	while (iterMap != pMapIntervalBarLstT->end())
	{
		pData = (iterMap->second);

		_ClearLstInstrumentBarInfo(pData);

		pData->clear();
		delete pData;
		pData = NULL;

		iterMap++;
	}
	pMapIntervalBarLstT->clear();
}



int CInstrumentBarInfoStorager::_GetBarInfoFromDB(unsigned int nInstrumentID, int interval)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::string strTableName;
	std::string strSQL;
	bool bExecRes = false;
	int nRows = 0;

	if (NULL != m_pQSqlQueryForSelect)
	{
		delete m_pQSqlQueryForSelect;
		m_pQSqlQueryForSelect = NULL;
	}

	if (!m_pQSqlDataBase->isOpen())
	{
		LOG_ERROR<<"m_QSqlDataBase is closed";
		nFunRes = -1;
		return nFunRes;
	}

	m_pQSqlQueryForSelect = new QSqlQuery(*m_pQSqlDataBase);

	strTableName = _GetBarInfoDBTableName(interval);
	strSQL = _BuildSelectSQL(strTableName);
	LOG_DEBUG<<"begin Exec sql="<<strSQL;

	bExecRes = m_pQSqlQueryForSelect->exec(strSQL.c_str());
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"Fail to exec sql:"<<strSQL<<" error:"<<m_pQSqlQueryForSelect->lastError().text().toStdString();
		delete m_pQSqlQueryForSelect;
		m_pQSqlQueryForSelect = NULL;
	}
	else
	{
		nRows = m_pQSqlQueryForSelect->size();
		LOG_INFO<<"end Exec sql="<<strSQL<<" and get rows="<<nRows;
	}
	return nFunRes;
}


int CInstrumentBarInfoStorager::getBarInfo(int interval, Bar& getBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	unsigned int nInstrumentIDTmp = 0;
	int nColumnIndex = 0;

	if (NULL == m_pQSqlQueryForSelect)
	{
		nFunRes = _GetBarInfoFromDB(m_nInstrumentID, interval);
	}

	if (0 != nFunRes || NULL == m_pQSqlQueryForSelect)
	{
		nFunRes = -1;
		return nFunRes;
	}

	/*
	0 1 2 3 4 5 6 
	InstrumentID INTEGER NOT NULL, 
	Timestamp TIMESTAMP NOT NULL, 
	Open decimal(25,10) NOT NULL,
	Close decimal(25,10) NOT NULL,
	High decimal(25,10) NOT NULL,	
	Low decimal(25,10) NOT NULL,
	Volume NUMBER,
	*/
	if (m_pQSqlQueryForSelect->isActive() && m_pQSqlQueryForSelect->isSelect() && m_pQSqlQueryForSelect->next()) 
	{ 		
		nColumnIndex = 0;
		nInstrumentIDTmp = (unsigned int)m_pQSqlQueryForSelect->value(nColumnIndex++).toInt();
		getBarInfo.Time = (unsigned int )m_pQSqlQueryForSelect->value(nColumnIndex++).toDateTime().toTime_t();
		getBarInfo.Open = (float)m_pQSqlQueryForSelect->value(nColumnIndex++).toReal();
		getBarInfo.Close = (float)m_pQSqlQueryForSelect->value(nColumnIndex++).toReal();
		getBarInfo.High = (float)m_pQSqlQueryForSelect->value(nColumnIndex++).toReal();
		getBarInfo.Low = (float)m_pQSqlQueryForSelect->value(nColumnIndex++).toReal();
		getBarInfo.Volume = (int)m_pQSqlQueryForSelect->value(nColumnIndex++).toInt();
	} 
	else
	{
		nFunRes = -1;
		delete m_pQSqlQueryForSelect;
		m_pQSqlQueryForSelect = NULL;
	}
	return nFunRes;
}

int CInstrumentBarInfoStorager::_StartStransactionForLstTickInfoBatchMode( LstInstrumentTickInfoT* pLstInstrumentTickInfo )
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bStartTransaction = false;

	bStartTransaction = false;

	if (false == m_pQSqlDataBase->driver()->hasFeature(QSqlDriver::Transactions)) 
	{
		bStartTransaction = false;
		LOG_INFO<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<" not support Transactions";
	}
	else
	{
		if (false == m_pQSqlDataBase->transaction())
		{
			LOG_ERROR<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<" support Transactions but start transaction error!"
				<<" error: "<<QSqlDatabase::database().lastError().text().toStdString();
		}
		else
		{
			LOG_INFO<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<"start Transaction";
			bStartTransaction = true;
		}
	}


	_StoreLstInstrumentTickInfoTBatchMode(pLstInstrumentTickInfo);
	_ClearLstInstrumentTickInfoT(pLstInstrumentTickInfo);//just clean data


	if (bStartTransaction)
	{
		if(false == m_pQSqlDataBase->commit())  
		{  
			LOG_ERROR<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<" commit error!"
				<<" error: "<<QSqlDatabase::database().lastError().text().toStdString();

			if(false == m_pQSqlDataBase->rollback())  
			{  
				LOG_ERROR<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<" rollback error!"
					<<" error: "<<QSqlDatabase::database().lastError().text().toStdString();
			}//if 
		}//if
		else
		{
			LOG_INFO<<"Database  m_nDBType="<<m_nDBType<<" db="<<m_strDBName<<"  commit Transaction";
		}
	}//if

	return nFunRes;
}



int CInstrumentBarInfoStorager::_StoreLstInstrumentTickInfoTBatchMode(LstInstrumentTickInfoT* pLstInstrumentTickInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = false;
	std::string strDBTableName;
	std::string strSQL;
	std::string strTimeStr;
	LstInstrumentTickInfoIterT iterLst;
	CInstrumentTickInfo* pInstrumentTickInfo = NULL;
	QVariantList lstInstrumentID;
	QVariantList lstTimestamp;
	QVariantList lstBidPx;
	QVariantList lstAskPx;
	QVariantList lstLastPx;
	QVariantList lstBidVol;
	QVariantList lstAskVol;
	QVariantList lstLastVol;

	if (NULL != m_pQSqlQueryForInseert)
	{
		delete m_pQSqlQueryForInseert;
		m_pQSqlQueryForInseert = NULL;
	}

	m_pQSqlQueryForInseert = new QSqlQuery(*m_pQSqlDataBase);

	strDBTableName = _GetTickInfoDBTableName(m_nInstrumentID);
	strSQL = _BuildSQLForInsertTickInfoBatchMode(strDBTableName);

	LOG_DEBUG<<"m_nInstrumentID="<<m_nInstrumentID<<" strSQL="<<strSQL<<" pLstInstrumentTickInfo.size="<<pLstInstrumentTickInfo->size();

	m_pQSqlQueryForInseert->prepare(strSQL.c_str());

	iterLst = pLstInstrumentTickInfo->begin();
	while (iterLst != pLstInstrumentTickInfo->end())
	{
		pInstrumentTickInfo = (*iterLst);
		strTimeStr = m_pUtilityFun->dataTimeToStr(pInstrumentTickInfo->m_TickInfo.Time);

		lstInstrumentID<<m_nInstrumentID;
		lstTimestamp << strTimeStr.c_str();
		lstBidPx << pInstrumentTickInfo->m_TickInfo.BidPx;
		lstAskPx << pInstrumentTickInfo->m_TickInfo.AskPx;
		lstLastPx << pInstrumentTickInfo->m_TickInfo.LastPx;
		lstBidVol << pInstrumentTickInfo->m_TickInfo.BidVol;
		lstAskVol << pInstrumentTickInfo->m_TickInfo.AskVol;
		lstLastVol << pInstrumentTickInfo->m_TickInfo.LastVol;

		iterLst++;
	}//while

	m_pQSqlQueryForInseert->addBindValue(lstInstrumentID);
	m_pQSqlQueryForInseert->addBindValue(lstTimestamp);
	m_pQSqlQueryForInseert->addBindValue(lstBidPx);
	m_pQSqlQueryForInseert->addBindValue(lstAskPx);
	m_pQSqlQueryForInseert->addBindValue(lstLastPx);
	m_pQSqlQueryForInseert->addBindValue(lstBidVol);
	m_pQSqlQueryForInseert->addBindValue(lstAskVol);
	m_pQSqlQueryForInseert->addBindValue(lstLastVol);

	bExecRes = m_pQSqlQueryForInseert->execBatch();
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"execBatch strSQL="<<strSQL<<" pLstInstrumentTickInfo.size="<<pLstInstrumentTickInfo->size()
			<<" "<<"error:"<<m_pQSqlQueryForInseert->lastError().text().toStdString();
	}

	if (NULL != m_pQSqlQueryForInseert)
	{
		delete m_pQSqlQueryForInseert;
		m_pQSqlQueryForInseert = NULL;
	}
	return nFunRes;
}

void CInstrumentBarInfoStorager::_ClearLstInstrumentTickInfoT( LstInstrumentTickInfoT* pLstInstrumentTickInfo )
{
	BOOST_LOG_FUNCTION();

	LstInstrumentTickInfoIterT iterLst;
	CInstrumentTickInfo* pData = NULL;

	iterLst = pLstInstrumentTickInfo->begin();
	while (iterLst != pLstInstrumentTickInfo->end())
	{
		pData = (*iterLst);

		delete pData;
		pData = NULL;

		iterLst++;
	}
	pLstInstrumentTickInfo->clear();
}





NS_END(TA_Base_App) 



