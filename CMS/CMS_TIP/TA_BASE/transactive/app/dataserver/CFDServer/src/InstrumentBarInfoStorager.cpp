#include "CFDServerCommonData.h"
#include "InstrumentBarInfoStorager.h"
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



CInstrumentBarInfoStorager::CInstrumentBarInfoStorager( const std::string& strPathInstrumentBarInfoTotal, unsigned int nInstrumentID )
{
	BOOST_LOG_FUNCTION();
	m_nInstrumentID = nInstrumentID;
	m_strPathInstrumentBarInfoTotal = strPathInstrumentBarInfoTotal;

	m_pUtilityFun = new CCFDServerUtilityFun();
	m_pmapIntervalDBTableName = new MapIntervalDBTableNameT();
	

	m_strDBType = defSQLiteDBName;
	m_nDBType = TA_Base_Core::enumSqliteDb;

	//qt not mysql driver need complie
	//m_strDBType = defMysqlDBName;
	//m_nDBType = TA_Base_Core::enumMysqlDb;

	m_strDBName = _GetBarInfoDBName(strPathInstrumentBarInfoTotal, nInstrumentID);
	m_pQSqlDataBase = NULL;
	m_pQSqlQueryForSelect = NULL;
	m_pLstInstrumentBarInfo = new LstInstrumentBarInfoT();
	m_pLstInstrumentBarInfo->clear();
	m_nBatchSize = 100;//TODO.
	_InitDataBase();

}

CInstrumentBarInfoStorager::~CInstrumentBarInfoStorager(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pLstInstrumentBarInfo)
	{
		_StoreBarInfoBatchMode(m_pLstInstrumentBarInfo);
		_ClearInstrumentBarInfoLst(m_pLstInstrumentBarInfo);
		m_pLstInstrumentBarInfo->clear();
		delete m_pLstInstrumentBarInfo;
		m_pLstInstrumentBarInfo = NULL;
	}


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

	if (NULL != m_pQSqlQueryForSelect)
	{
		delete m_pQSqlQueryForSelect;
		m_pQSqlQueryForSelect = NULL;
	}

}



std::string CInstrumentBarInfoStorager::_GetBarInfoDBName(const std::string& strPathInstrumentBarInfoTotal, unsigned int nInstrumentID)
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
		sreaamTmp<<strPathInstrumentBarInfoTotal<<"//"
			<<str_SQliteDb_Instrument_BAR_DB_header<<nInstrumentID<<".db";
		break;
	case TA_Base_Core::enumMysqlDb:
		sreaamTmp<<"mysqldb_"<<nInstrumentID;
		break;
	default:
		sreaamTmp<<strPathInstrumentBarInfoTotal<<"//"
			<<str_SQliteDb_Instrument_BAR_DB_header<<nInstrumentID<<".db";
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

	strTableName = _GetBarInfoDBTableName(interval);
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
	bool bExecRes = true;
	std::string strSQL;

	QSqlQuery query(*m_pQSqlDataBase);

	strSQL = _BuildCreateDBTableSQL(strDbTableName);

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


std::string CInstrumentBarInfoStorager::_BuildInsertSQL(const std::string& strTableName)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;

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



int CInstrumentBarInfoStorager::_InsertData(int interval, Bar& barInfo)
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
	strTimeStr = m_pUtilityFun->dataTimeToStr(barInfo.Time);
	query.bindValue(str_Column_Timestamp_Value.c_str(), strTimeStr.c_str());
	query.bindValue(str_Column_Open_Value.c_str(), barInfo.Open);
	query.bindValue(str_Column_Close_Value.c_str(), barInfo.Close);
	query.bindValue(str_Column_High_Value.c_str(), barInfo.High);
	query.bindValue(str_Column_Low_Value.c_str(), barInfo.Low);
	query.bindValue(str_Column_Volume_Value.c_str(), barInfo.Volume);

	bExecRes = query.exec();
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"Fail to exec sql:"<<strSQL<<" error:"<<query.lastError().text().toStdString();
	}

	return nFunRes;
}


int CInstrumentBarInfoStorager::storeBarInfo(int interval, Bar& barInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	LstInstrumentBarInfoIterT iterLst;
	CInstrumentBarInfo* pInstrumentBarInfo = NULL;
	
	pInstrumentBarInfo = new CInstrumentBarInfo(interval, barInfo);

	m_pLstInstrumentBarInfo->push_back(pInstrumentBarInfo);

	if (m_pLstInstrumentBarInfo->size() < m_nBatchSize)
	{
		return nFunRes;
	}

	nFunRes = _StoreBarInfoBatchMode(m_pLstInstrumentBarInfo);
	_ClearInstrumentBarInfoLst(m_pLstInstrumentBarInfo);

	return nFunRes;
}

void CInstrumentBarInfoStorager::setSoreBatchSize(unsigned int nBatchSize)
{
	BOOST_LOG_FUNCTION();
	m_nBatchSize = nBatchSize;
}




int CInstrumentBarInfoStorager::_StoreBarInfoBatchMode(LstInstrumentBarInfoT*  pLstInstrumentBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	LstInstrumentBarInfoIterT iterLst;
	CInstrumentBarInfo* pInstrumentBarInfo = NULL;

	pInstrumentBarInfo = NULL;
	iterLst = pLstInstrumentBarInfo->begin();
	while (iterLst != pLstInstrumentBarInfo->end())
	{
		pInstrumentBarInfo = (*iterLst);
		nFunRes = _InsertData(pInstrumentBarInfo->m_nInterval, pInstrumentBarInfo->m_BarInfo);
		if (0 != nFunRes)
		{
			return nFunRes;
		}
		iterLst++;
	}//while

	return nFunRes;
}


void CInstrumentBarInfoStorager::_ClearInstrumentBarInfoLst(LstInstrumentBarInfoT* pLstInstrumentBarInfo)
{
	LstInstrumentBarInfoIterT iterLst;
	CInstrumentBarInfo* pInstrumentBarInfo = NULL;

	iterLst = pLstInstrumentBarInfo->begin();
	while (iterLst != pLstInstrumentBarInfo->end())
	{
		pInstrumentBarInfo = (*iterLst);

		delete pInstrumentBarInfo;
		pInstrumentBarInfo = NULL;

		iterLst++;
	}
	pLstInstrumentBarInfo->clear();

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





NS_END(TA_Base_App) 



