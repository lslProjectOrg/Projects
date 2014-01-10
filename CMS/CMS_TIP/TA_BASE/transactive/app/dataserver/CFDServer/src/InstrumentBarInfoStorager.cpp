#include "InstrumentBarInfoStorager.h"

 

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include "MarketData.h"
#include "BarCalculator.h"
#include "CFDServerUtilityFun.h"


#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_Base_Core;

NS_BEGIN(TA_Base_App)


//////////////////////////////////////////////////////////////////////////
static const std::string str_SQliteDb_Instrument_BAR_DB_header = "SQLiteDB_";//SQLiteDB_3320.db

static const std::string str_Column_InstrumentID = "InstrumentID";
static const std::string str_Column_Timestamp = "Timestamp";
static const std::string str_Column_Open = "Open";
static const std::string str_Column_Close = "Close";
static const std::string str_Column_High = "High";
static const std::string str_Column_Low = "Low";
static const std::string str_Column_Volume = "Volume";
static const std::string str_Column_LastModified = "LastModified";


static const std::string str_Column_InstrumentID_Value = ":InstrumentID_Value";
static const std::string str_Column_Timestamp_Value = ":Timestamp_Value";
static const std::string str_Column_Open_Value = ":Open_Value";
static const std::string str_Column_Close_Value = ":Close_Value";
static const std::string str_Column_High_Value = ":High_Value";
static const std::string str_Column_Low_Value = ":Low_Value";
static const std::string str_Column_Volume_Value = ":Volume_Value";




CInstrumentBarInfoStorager::CInstrumentBarInfoStorager(unsigned int nInstrumentID, const CInstrumentBarInfoRequest& instrumentBarInfoRequest)
{
	BOOST_LOG_FUNCTION();
	m_nInstrumentID = nInstrumentID;
	m_InstrumentBarInfoRequest = instrumentBarInfoRequest;

	m_pUtilityFun = new CCFDServerUtilityFun();
	m_pmapIntervalDBTableName = new MapIntervalDBTableNameT();
	

	m_strDBType = defSQLiteDBName;
	m_nDBType = TA_Base_Core::enumSqliteDb;

	//qt not mysql driver need complie
	//m_strDBType = defMysqlDBName;
	//m_nDBType = TA_Base_Core::enumMysqlDb;

	m_strDBName = getBarInfoDBName(m_nInstrumentID, m_InstrumentBarInfoRequest);
	m_pQSqlDataBase = NULL;
	m_pQSqlQuery = NULL;
	_InitDataBase();

}

CInstrumentBarInfoStorager::~CInstrumentBarInfoStorager(void)
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
void CInstrumentBarInfoStorager::_InitDataBase()
{
	bool bExecRes = false;

	if (NULL != m_pQSqlDataBase)
	{
		m_pQSqlDataBase->close();
		delete m_pQSqlDataBase;
		m_pQSqlDataBase = NULL;
	}

	//SQLiteDB_3306.db
	//mysqldb_3306
	switch (m_nDBType)
	{
	case TA_Base_Core::enumSqliteDb:
		m_pQSqlDataBase = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", m_strDBName.c_str()));
		m_pQSqlDataBase->setDatabaseName(m_strDBName.c_str());
		break;
	case TA_Base_Core::enumMysqlDb:
		m_pQSqlDataBase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", m_strDBName.c_str()));
		m_pQSqlDataBase->setDatabaseName(m_strDBName.c_str());
		m_pQSqlDataBase->setHostName("127.0.0.1");
		m_pQSqlDataBase->setDatabaseName(m_strDBName.c_str());
		m_pQSqlDataBase->setUserName("root");
		m_pQSqlDataBase->setPassword("root");
		break;
	default:
		m_pQSqlDataBase = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", m_strDBName.c_str()));
		m_pQSqlDataBase->setDatabaseName(m_strDBName.c_str());
		break;
	}

	bExecRes = m_pQSqlDataBase->open();
	if (!bExecRes)
	{
		LOG_ERROR<<"Fail to open db:"<<m_strDBName<<" error:"<<m_pQSqlDataBase->lastError().text().toStdString();
	}
}

void CInstrumentBarInfoStorager::_UnInitDataBase()
{
	if (NULL != m_pQSqlDataBase)
	{
		m_pQSqlDataBase->close();
		delete m_pQSqlDataBase;
		m_pQSqlDataBase = NULL;
		//QSqlDatabase::removeDatabase(m_strDBName.c_str());
	}

	if (NULL != m_pQSqlQuery)
	{
		delete m_pQSqlQuery;
		m_pQSqlQuery = NULL;
	}

}


std::string CInstrumentBarInfoStorager::getBarInfoDBName(unsigned int nInstrumentID, const CInstrumentBarInfoRequest& instrumentBarInfoRequest)
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
		sreaamTmp<<instrumentBarInfoRequest.m_strInstrumetBarInfoTotal<<"//"
			<<str_SQliteDb_Instrument_BAR_DB_header<<nInstrumentID<<".db";
		break;
	case TA_Base_Core::enumMysqlDb:
		sreaamTmp<<"mysqldb_"<<nInstrumentID;
		break;
	default:
		sreaamTmp<<instrumentBarInfoRequest.m_strInstrumetBarInfoTotal<<"//"
			<<str_SQliteDb_Instrument_BAR_DB_header<<nInstrumentID<<".db";
		break;
	}

	strInstrumentSQLDBName = sreaamTmp.str();


	return strInstrumentSQLDBName;
}

std::string CInstrumentBarInfoStorager::getBarInfoDBTableName(unsigned int nInstrumentID, int interval)
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
std::string CInstrumentBarInfoStorager::_CheckAndInitDBTable(unsigned int nInstrumentID, int interval)
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

	strTableName = getBarInfoDBTableName(nInstrumentID, interval);
	_CreateDBTable(strTableName);
	m_pmapIntervalDBTableName->insert(MapIntervalDBTableNameValueTypeT(interval, strTableName));
	return strTableName;
}


std::string CInstrumentBarInfoStorager::_BuildCreateDBTableSQL(const std::string& strTableName)
{
	BOOST_LOG_FUNCTION();
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
	sreaamTmp<<"CREATE TABLE IF NOT EXISTS "<<strTableName
		<<" "<<"("
		<<" "<<str_Column_InstrumentID<<" "<<"INTEGER NOT NULL"<<","
		<<" "<<str_Column_Timestamp<<" "<<"TIMESTAMP NOT NULL"<<","
		<<" "<<str_Column_Open<<" "<<"decimal(25,10) NOT NULL"<<","
		<<" "<<str_Column_Close<<" "<<"decimal(25,10) NOT NULL"<<","
		<<" "<<str_Column_High<<" "<<"decimal(25,10) NOT NULL"<<","
		<<" "<<str_Column_Low<<" "<<"decimal(25,10) NOT NULL"<<","
		<<" "<<str_Column_Volume<<" "<<"decimal(25,10) NOT NULL"<<","
		<<" "<<"PRIMARY KEY ("<<str_Column_InstrumentID<<", "<<str_Column_Timestamp<<")"
		<<" "<<")";

	strSQL = sreaamTmp.str();
	return strSQL;
}
int CInstrumentBarInfoStorager::_CreateDBTable(const std::string& strDbTableName)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::string strSQL;
	strSQL = _BuildCreateDBTableSQL(strDbTableName);
	nFunRes = _Exec(strSQL);
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


std::string CInstrumentBarInfoStorager::_BuildInsertSQL(const std::string& strTableName)
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
		<<" "<<str_Column_InstrumentID_Value<<","
		<<" "<<str_Column_Timestamp_Value<<","
		<<" "<<str_Column_Open_Value<<","
		<<" "<<str_Column_Close_Value<<","
		<<" "<<str_Column_High_Value<<","
		<<" "<<str_Column_Low_Value<<","
		<<" "<<str_Column_Volume_Value
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
	FROM bar_data_15mins
	order by Timestamp
	;
	*/

	sreaamTmp.str("");
	sreaamTmp<<"SELECT"
		<<" "<<str_Column_InstrumentID<<","
		<<" "<<str_Column_Timestamp<<","
		<<" "<<str_Column_Open<<","
		<<" "<<str_Column_Close<<","
		<<" "<<str_Column_High<<","
		<<" "<<str_Column_Low<<","
		<<" "<<str_Column_Volume
		<<" "<<"FROM"<<" "<<strTableName
		<<" "<<"ORDER BY"<<" "<<str_Column_Timestamp;

	strSQL = sreaamTmp.str();
	return strSQL;	
}



int CInstrumentBarInfoStorager::_InsertData(int interval, Bar* pBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = true;
	std::string strTimeStr;
	QSqlQuery query(*m_pQSqlDataBase);

	std::string strDBTableName;
	std::string strSQL;
	strDBTableName = _CheckAndInitDBTable(m_nInstrumentID, interval);
	strSQL = _BuildInsertSQL(strDBTableName);
	LOG_DEBUG<<"strSQL="<<strSQL;

	query.prepare(strSQL.c_str());
	query.bindValue(str_Column_InstrumentID_Value.c_str(), m_nInstrumentID);
	strTimeStr = m_pUtilityFun->dataTimeToStr(pBarInfo->Time);
	query.bindValue(str_Column_Timestamp_Value.c_str(), strTimeStr.c_str());
	query.bindValue(str_Column_Open_Value.c_str(), pBarInfo->Open);
	query.bindValue(str_Column_Close_Value.c_str(), pBarInfo->Close);
	query.bindValue(str_Column_High_Value.c_str(), pBarInfo->High);
	query.bindValue(str_Column_Low_Value.c_str(), pBarInfo->Low);
	query.bindValue(str_Column_Volume_Value.c_str(), pBarInfo->Volume);

	bExecRes = query.exec();
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"Fail to exec sql:"<<strSQL<<" error:"<<query.lastError().text().toStdString();
	}


	return nFunRes;
}


int CInstrumentBarInfoStorager::_Exec( const std::string& strSQL)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = true;
	LOG_DEBUG<<"strSQL="<<strSQL;

	QSqlQuery query(*m_pQSqlDataBase);
	bExecRes = query.exec(strSQL.c_str());
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"Fail to exec sql:"<<strSQL<<" error:"<<query.lastError().text().toStdString();
	}
	return nFunRes;
}




int CInstrumentBarInfoStorager::storeBarInfo(int interval, Bar* pBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	nFunRes = _InsertData(interval, pBarInfo);
	
	return nFunRes;
}

//////////////////////////////////////////////////////////////////////////
int CInstrumentBarInfoStorager::beginGetBarInfo(unsigned int nInstrumentID, int interval)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::string strTableName;
	std::string strSQL;
	bool bExecRes = false;
	int nRows = 0;

	if (NULL != m_pQSqlQuery)
	{
		delete m_pQSqlQuery;
		m_pQSqlQuery = NULL;
	}

	if (!m_pQSqlDataBase->isOpen())
	{
		LOG_ERROR<<"m_QSqlDataBase is closed";
		nFunRes = -1;
		return nFunRes;
	}

	m_pQSqlQuery = new QSqlQuery(*m_pQSqlDataBase);

	strTableName = getBarInfoDBTableName(nInstrumentID, interval);
	strSQL = _BuildSelectSQL(strTableName);
	LOG_DEBUG<<"begin Exec sql="<<strSQL;

	bExecRes = m_pQSqlQuery->exec(strSQL.c_str());
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"Fail to exec sql:"<<strSQL<<" error:"<<m_pQSqlQuery->lastError().text().toStdString();
	}
	else
	{
		nRows = m_pQSqlQuery->size();
		LOG_INFO<<"end Exec sql="<<strSQL<<" and get rows="<<nRows;
	}
	return nFunRes;
}
int CInstrumentBarInfoStorager::getNextBarInfo(Bar& getNextBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	unsigned int nInstrumentIDTmp = 0;
	int nColumnIndex = 0;

	if (NULL == m_pQSqlQuery)
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
	if (m_pQSqlQuery->isActive() && m_pQSqlQuery->isSelect() && m_pQSqlQuery->next()) 
	{ 		
		nColumnIndex = 0;
		nInstrumentIDTmp = (unsigned int)m_pQSqlQuery->value(nColumnIndex++).toInt();
		getNextBarInfo.Time = (unsigned int )m_pQSqlQuery->value(nColumnIndex++).toDateTime().toTime_t();
		getNextBarInfo.Open = (float)m_pQSqlQuery->value(nColumnIndex++).toReal();
		getNextBarInfo.Close = (float)m_pQSqlQuery->value(nColumnIndex++).toReal();
		getNextBarInfo.High = (float)m_pQSqlQuery->value(nColumnIndex++).toReal();
		getNextBarInfo.Low = (float)m_pQSqlQuery->value(nColumnIndex++).toReal();
		getNextBarInfo.Volume = (int)m_pQSqlQuery->value(nColumnIndex++).toInt();
	} 
	else
	{
		nFunRes = -1;
		delete m_pQSqlQuery;
		m_pQSqlQuery = NULL;
	}
	return nFunRes;
}




NS_END(TA_Base_App) 



