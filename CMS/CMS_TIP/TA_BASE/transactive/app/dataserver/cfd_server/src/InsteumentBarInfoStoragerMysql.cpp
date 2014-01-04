#include "InsteumentBarInfoStoragerMysql.h"

 

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include "MarketData.h"
#include "BarCalculator.h"
#include "CFDServerUtilityFun.h"


#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_Base_Core;

NS_BEGIN(TA_Base_App)


//////////////////////////////////////////////////////////////////////////
static const std::string str_Column_InstrumentID = "InstrumentID";
static const std::string str_Column_Timestamp = "Timestamp";
static const std::string str_Column_Open = "Open";
static const std::string str_Column_Close = "Close";
static const std::string str_Column_High = "High";
static const std::string str_Column_Low = "Low";
static const std::string str_Column_Volume = "Volume";
static const std::string str_Column_LastModified = "LastModified";


static const std::string str_Column_InstrumentID_Value = "?";
static const std::string str_Column_Timestamp_Value= "?";
static const std::string str_Column_Open_Value = "?";
static const std::string str_Column_Close_Value= "?";
static const std::string str_Column_High_Value = "?";
static const std::string str_Column_Low_Value = "?";
static const std::string str_Column_Volume_Value = "?";




CInstrumentBarInfoStoragerMysql::CInstrumentBarInfoStoragerMysql(unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_pmapIntervalDBTableName = new MapIntervalDBTableNameT();
	m_nInstrumentID = nInstrumentID;

	m_strDBType = defMysqlDBName;
	m_nDBType = TA_Base_Core::enumMysqlDb;

	m_strDBName = _GetDBName(nInstrumentID);
	m_pMysqlDriver = NULL;
	m_pMysqlDataBase = NULL;
	_InitDataBase();

}

CInstrumentBarInfoStoragerMysql::~CInstrumentBarInfoStoragerMysql(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}

	if (NULL != m_pmapIntervalDBTableName)
	{
		m_pmapIntervalDBTableName->clear();
		delete m_pmapIntervalDBTableName;
		m_pmapIntervalDBTableName = NULL;
	}

	_UnInitDataBase();


}
void CInstrumentBarInfoStoragerMysql::_InitDataBase()
{
	bool bExecRes = false;
	//SQLiteDB_3306.db
	//mysqldb_3306

	try
	{
		switch (m_nDBType)
		{
		case TA_Base_Core::enumSqliteDb:
			break;
		case TA_Base_Core::enumMysqlDb:
			/* Using the Driver to create a connection */
			m_pMysqlDriver = get_driver_instance();//sql::mysql::get_driver_instance();
			m_pMysqlDataBase = m_pMysqlDriver->connect("tcp://127.0.0.1:3306", "root", "root");
			/* Connect to the MySQL test database */
			m_pMysqlDataBase->setSchema(m_strDBName.c_str());
			break;
		default:
			break;
		}
	}
	catch (sql::SQLException &e)
	{
		LOG_ERROR<<"Fail to open db:"<<m_strDBName<<" error:"<<e.what();
	}
}

void CInstrumentBarInfoStoragerMysql::_UnInitDataBase()
{
	if (NULL != m_pMysqlDataBase)
	{
		m_pMysqlDataBase->close();
		delete m_pMysqlDataBase;
		m_pMysqlDataBase = NULL;
	}

}


std::string CInstrumentBarInfoStoragerMysql::_GetDBName(unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strInstrumentSQLDBName;
	sreaamTmp.str("");

	//SQLiteDB_3306.db
	//mysqldb_3306
	switch (m_nDBType)
	{
	case TA_Base_Core::enumSqliteDb:
		sreaamTmp<<"SQLiteDB_"<<nInstrumentID<<".db";
		break;
	case TA_Base_Core::enumMysqlDb:
		sreaamTmp<<"mysqldb_"<<nInstrumentID;
		break;
	default:
		sreaamTmp<<"SQLiteDB_"<<nInstrumentID<<".db";
		break;
	}

	strInstrumentSQLDBName = sreaamTmp.str();


	return strInstrumentSQLDBName;
}

std::string CInstrumentBarInfoStoragerMysql::_BuildBarDataTableName(unsigned int nInstrumentID, int interval)
{
	BOOST_LOG_FUNCTION();	
	std::string strBarDataTableName;
	std::ostringstream sreaamTmp;

	switch (interval)
	{
	case TIME_BASE_S_1S:
		sreaamTmp<<"bar_data_1seconds";
		break;
	case TIME_BASE_S_5S:
		sreaamTmp<<"bar_data_5seconds";
		break;
	case TIME_BASE_S_1MIN:
		sreaamTmp<<"bar_data_1mins";
		break;
	case TIME_BASE_S_5MIN:
		sreaamTmp<<"bar_data_5mins";
		break;
	case TIME_BASE_S_15MIN:
		sreaamTmp<<"bar_data_15mins";
		break;
	case TIME_BASE_S_30MIN:
		sreaamTmp<<"bar_data_30mins";
		break;
	case TIME_BASE_S_1HOUR:
		sreaamTmp<<"bar_data_1hours";
		break;
	case TIME_BASE_S_1DAY:
		sreaamTmp<<"bar_data_1days";
		break;
	case TIME_BASE_S_1MON:
		sreaamTmp<<"bar_data_1mons";
		break;
	case TIME_BASE_S_1YEAR:
		sreaamTmp<<"bar_data_1years";
		break;
	default:
		sreaamTmp<<"bar_data_"<<interval;
		break;
	}

	strBarDataTableName = sreaamTmp.str();
	return strBarDataTableName;
}
std::string CInstrumentBarInfoStoragerMysql::_GetDBTableName(unsigned int nInstrumentID, int interval)
{
	BOOST_LOG_FUNCTION();

	//find
	std::string strTableName;
	MapIntervalDBTableNameIterT  iterMap;

	iterMap = m_pmapIntervalDBTableName->find(interval);
	if (iterMap != m_pmapIntervalDBTableName->end())
	{
		strTableName = (iterMap->second);
		return strTableName;
	}

	strTableName = _BuildBarDataTableName(nInstrumentID, interval);
	_CreateDBTable(strTableName);
	m_pmapIntervalDBTableName->insert(MapIntervalDBTableNameValueTypeT(interval, strTableName));
	return strTableName;
}



int CInstrumentBarInfoStoragerMysql::_CreateDBTable(const std::string& strDbTableName)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::ostringstream sreaamTmp;
	std::string strSQL;

	/*
	CREATE TABLE IF NOT EXISTS bar_data_15mins
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
	//TIMESTR--%Y%m%d%H%M%S
	//%04d-%02d-%02d %02d:%02d:%02d
	*/

	sreaamTmp.str("");
	sreaamTmp<<"CREATE TABLE IF NOT EXISTS "<<strDbTableName
		<<" "<<"("
		<<" "<<str_Column_InstrumentID<<" "<<"INTEGER NOT NULL"<<","
		<<" "<<str_Column_Timestamp<<" "<<"TIMESTAMP NOT NULL"<<","
		<<" "<<str_Column_Open<<" "<<"decimal(25,10) NOT NULL"<<","
		<<" "<<str_Column_Close<<" "<<"decimal(25,10) NOT NULL"<<","
		<<" "<<str_Column_High<<" "<<"decimal(25,10) NOT NULL"<<","
		<<" "<<str_Column_Low<<" "<<"decimal(25,10) NOT NULL"<<","
		<<" "<<str_Column_Volume<<" "<<"decimal(25,10) NOT NULL"<<","
		<<" "<<"PRIMARY KEY (InstrumentID, Timestamp)"
		<<" "<<")";

	strSQL = sreaamTmp.str();
	nFunRes = _Exec(strSQL);
	return nFunRes;
}

int CInstrumentBarInfoStoragerMysql::_DropDBTable(const std::string& strDbTableName)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::ostringstream sreaamTmp;
	std::string strSQL;
	
	/*DROP TABLE Table_BarInfo*/
	sreaamTmp.str("");
	sreaamTmp<<"DROP TABLE "<<strDbTableName;

	strSQL = sreaamTmp.str();
	nFunRes = _Exec(strSQL);
	return nFunRes;
}


std::string CInstrumentBarInfoStoragerMysql::_BuildInsertSQL(const std::string& strTableName, Bar* pBarInfo)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;

	/*
	INSERT INTO bar_data_15mins
	(
	InstrumentID, Timestamp, 
	Open, Close, High, Low, 
	Volume
	) VALUES 
	(
	3620, '2013-12-20 09:00:02', 
	2340.8, 2340.8, 2340.8, 2340.8, 
	0 
	);
	*/
	/*
	INSERT INTO bar_data_15mins
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
	std::string strTimeStr;
	strTimeStr = m_pUtilityFun->getTimeStringForSQL(pBarInfo->Time);

	//INSERT INTO bar_data_5seconds ( InstrumentID, Timestamp, Open, Close, High, Low, Volume ) VALUES ( 3620, '2014-01-05 14:15:12', 2340.8, 2340.8, 2340.8, 2340.8, 0 );

	sreaamTmp.str("");
	sreaamTmp<<"INSERT INTO "<<strTableName
		<<" "<<"("
		<<" "<<str_Column_InstrumentID<<","
		<<" "<<str_Column_Timestamp<<","
		<<" "<<str_Column_Open<<","
		<<" "<<str_Column_Close<<","
		<<" "<<str_Column_High<<","
		<<" "<<str_Column_Low<<","
		<<" "<<str_Column_Volume
		<<" "<<")"
		<<" "<<"VALUES"
		<<" "<<"("
		<<" "<<m_nInstrumentID<<","
		<<" "<<"'"<<strTimeStr<<"'"<<","
		<<" "<<pBarInfo->Open<<","
		<<" "<<pBarInfo->Close<<","
		<<" "<<pBarInfo->High<<","
		<<" "<<pBarInfo->Low<<","
		<<" "<<pBarInfo->Volume
		<<" "<<")";


	strSQL = sreaamTmp.str();
	return strSQL;	
}

int CInstrumentBarInfoStoragerMysql::_InsertData(int interval, Bar* pBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = true;
	std::string strTimeStr;
	std::string strDBTableName;
	std::string strSQL;

	if (NULL == m_pMysqlDataBase || m_pMysqlDataBase->isClosed())
	{
		LOG_ERROR<<"DB error!";
		nFunRes = -1;
		return nFunRes;
	}

	strDBTableName = _GetDBTableName(m_nInstrumentID, interval);
	strSQL = _BuildInsertSQL(strDBTableName, pBarInfo);
	nFunRes = _Exec(strSQL);
	return nFunRes;
}

int CInstrumentBarInfoStoragerMysql::_Exec( const std::string& strSQL)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	sql::Statement *stmt = NULL;

	if (NULL == m_pMysqlDataBase || m_pMysqlDataBase->isClosed())
	{
		LOG_ERROR<<"DB error!";
		nFunRes = -1;
		return nFunRes;
	}

	LOG_DEBUG<<"strSQL="<<strSQL;

	try
	{
		stmt = m_pMysqlDataBase->createStatement();
		stmt->execute(strSQL.c_str());
	}
	catch (sql::SQLException &e)
	{
		LOG_ERROR<<"Fail to exec:"<<strSQL<<" error:"<<e.what();
	}
	
	if (NULL != stmt)
	{
		delete stmt;
		stmt = NULL;
	}

	return nFunRes;
}




int CInstrumentBarInfoStoragerMysql::storeBarInfo(int interval, Bar* pBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	nFunRes = _InsertData(interval, pBarInfo);
	
	return nFunRes;
}





NS_END(TA_Base_App) 



