#include "SqliteDbOper.h"

#include "CFDServerUtilityFun.h"

#include <boost/algorithm/string.hpp>

#include "BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_Base_App;


NS_BEGIN(TA_Base_App)


//////////////////////////////////////////////////////////////////////////


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




static const std::string str_BarData_Column_Timestamp = "Timestamp";
static const std::string str_BarData_Column_Open = "Open";
static const std::string str_BarData_Column_Close = "Close";
static const std::string str_BarData_Column_High = "High";
static const std::string str_BarData_Column_Low = "Low";
static const std::string str_BarData_Column_Volume = "Volume";
static const std::string str_BarData_Column_LastModified = "LastModified";


static const std::string str_BarData_Column_Timestamp_Value = ":Timestamp_Value";
static const std::string str_BarData_Column_Open_Value = ":Open_Value";
static const std::string str_BarData_Column_Close_Value = ":Close_Value";
static const std::string str_BarData_Column_High_Value = ":High_Value";
static const std::string str_BarData_Column_Low_Value = ":Low_Value";
static const std::string str_BarData_Column_Volume_Value = ":Volume_Value";
////
static const std::string str_TickData_Column_Timestamp = "Timestamp";
static const std::string str_TickData_Column_BidPx = "BidPx";
static const std::string str_TickData_Column_AskPx = "AskPx";
static const std::string str_TickData_Column_LastPx = "LastPx";
static const std::string str_TickData_Column_BidVol = "BidVol";
static const std::string str_TickData_Column_AskVol = "AskVol";
static const std::string str_TickData_Column_LastVol = "LastVol";

static const std::string str_TickData_Column_Timestamp_Value = ":Timestamp_Value";
static const std::string str_TickData_Column_BidPx_Value = ":BidPx_Value";
static const std::string str_TickData_Column_AskPx_Value = ":AskPx_Value";
static const std::string str_TickData_Column_LastPx_Value = ":LastPx_Value";
static const std::string str_TickData_Column_BidVol_Value = ":BidVol_Value";
static const std::string str_TickData_Column_AskVol_Value = ":AskVol_Value";
static const std::string str_TickData_Column_LastVol_Value = ":LastVol_Value";



CSqliteDbOper::CSqliteDbOper( const CDBOperParam& dbOperParam ) : IDbOper()
{
	BOOST_LOG_FUNCTION();
	
	m_pUtilityFun = NULL;
	m_pDBOperParam = NULL;
	m_pQSqlDataBaseBAR = NULL;
	m_pQSqlDataBaseTIK = NULL;
	m_bStartTransactionBAR = false;
	m_bStartTransactionTIK = false;

	m_pUtilityFun = new CCFDServerUtilityFun();

	m_pDBOperParam = new CDBOperParam();
	*m_pDBOperParam = dbOperParam;


	_InitDataBase();
}

CSqliteDbOper::~CSqliteDbOper(void)
{
	BOOST_LOG_FUNCTION();

	_UnInitDataBase();

	if (NULL != m_pDBOperParam)
	{
		delete m_pDBOperParam;
		m_pDBOperParam = NULL;
	}

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}
	

}

void CSqliteDbOper::_InitDataBase()
{
	bool bExecRes = false;
	std::string strDBFileName;

	_UnInitDataBase();

	switch (m_pDBOperParam->m_nDataSrcDbType)
	{
	case TA_Base_App::enumSqliteDb:
		//SQLiteDB_3306.db
		m_pQSqlDataBaseBAR = new QSqlDatabase(QSqlDatabase::addDatabase(m_pDBOperParam->m_strQTDataSrcDbTypeName.c_str(), QLatin1String(m_pDBOperParam->m_strSQLiteDBFileNameBAR.c_str())));
		//d://savedata//SQLiteDB_3306.db
		m_pQSqlDataBaseBAR->setDatabaseName(m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR.c_str());
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"new Database  m_nDataSrcDbType="<<m_pDBOperParam->m_nDataSrcDbType
			<<" "<<"m_strSQLiteDBFileNameBAR="<<m_pDBOperParam->m_strSQLiteDBFileNameBAR
			<<" "<<"m_strSQLiteDBFileNameFullPathBAR="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR;


		//SQLiteDB_3306.db
		m_pQSqlDataBaseTIK = new QSqlDatabase(QSqlDatabase::addDatabase(m_pDBOperParam->m_strQTDataSrcDbTypeName.c_str(), QLatin1String(m_pDBOperParam->m_strSQLiteDBFileNameTIK.c_str())));
		//d://savedata//SQLiteDB_3306.db
		m_pQSqlDataBaseTIK->setDatabaseName(m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK.c_str());
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"new Database  m_nDataSrcDbType="<<m_pDBOperParam->m_nDataSrcDbType
			<<" "<<"m_strSQLiteDBFileNameTIK="<<m_pDBOperParam->m_strSQLiteDBFileNameTIK
			<<" "<<"m_strSQLiteDBFileNameFullPathTIK="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK;
		
		break;
	default:
		break;
	}

	///////
	bExecRes = m_pQSqlDataBaseBAR->open();
	if (!bExecRes)
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR<<" Fail to open!"
			<<" "<<"error:"<<m_pQSqlDataBaseBAR->lastError().text().toStdString();
	}

	//check
	if (false == m_pQSqlDataBaseBAR->isValid())
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR<<" is not Valid";
	}
	else
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR<<" is Valid";
	}

	if (false == m_pQSqlDataBaseBAR->driver()->hasFeature(QSqlDriver::Transactions)) 
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR<<" not support Transactions";
	}
	else
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR<<" support Transactions";
	}
	///////


	///////
	bExecRes = m_pQSqlDataBaseTIK->open();
	if (!bExecRes)
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK<<" Fail to open!"
			<<" "<<"error:"<<m_pQSqlDataBaseBAR->lastError().text().toStdString();
	}

	//check
	if (false == m_pQSqlDataBaseTIK->isValid())
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK<<" is not Valid";
	}
	else
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK<<" is Valid";
	}

	if (false == m_pQSqlDataBaseTIK->driver()->hasFeature(QSqlDriver::Transactions)) 
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK<<" not support Transactions";
	}
	else
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK<<" support Transactions";
	}
	///////

}

void CSqliteDbOper::_UnInitDataBase()
{

	if (NULL != m_pQSqlDataBaseBAR)
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"delete Database  m_nDataSrcDbType="<<m_pDBOperParam->m_nDataSrcDbType
			<<" "<<"m_strSQLiteDBFileNameBAR="<<m_pDBOperParam->m_strSQLiteDBFileNameBAR
			<<" "<<"m_strSQLiteDBFileNameFullPathBAR="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR;

		m_pQSqlDataBaseBAR->close();
		delete m_pQSqlDataBaseBAR;
		m_pQSqlDataBaseBAR = NULL;
		//QSqlDatabase::removeDatabase(m_pDBOperParam->m_strSQLiteDBFileName.c_str());
	}

	if (NULL != m_pQSqlDataBaseTIK)
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"delete Database  m_nDataSrcDbType="<<m_pDBOperParam->m_nDataSrcDbType
			<<" "<<"m_strSQLiteDBFileNameTIK="<<m_pDBOperParam->m_strSQLiteDBFileNameTIK
			<<" "<<"m_strSQLiteDBFileNameFullPathTIK="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK;

		m_pQSqlDataBaseTIK->close();
		delete m_pQSqlDataBaseTIK;
		m_pQSqlDataBaseTIK = NULL;
		//QSqlDatabase::removeDatabase(m_pDBOperParam->m_strSQLiteDBFileName.c_str());
	}

}




int CSqliteDbOper::_StartTransactionBAR()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	m_bStartTransactionBAR = false;

	if (false == m_pQSqlDataBaseBAR->driver()->hasFeature(QSqlDriver::Transactions)) 
	{
		m_bStartTransactionBAR = false;

		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPathBAR="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
			<<" "<<"not support Transactions";
	}
	else
	{
		if (false == m_pQSqlDataBaseBAR->transaction())
		{
			m_bStartTransactionBAR = false;

			LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPathBAR="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
				<<" "<<"support Transactions but start transaction error!"
				<<" "<<"error: "<<QSqlDatabase::database().lastError().text().toStdString();
		}
		else
		{
			m_bStartTransactionBAR = true;

			LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPathBAR="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
				<<" "<<"start Transaction";
		}
	}

	return nFunRes;
}



int CSqliteDbOper::_CommitTransactionBAR()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	if (m_bStartTransactionBAR)
	{
		if(false == m_pQSqlDataBaseBAR->commit())  
		{  
			LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPathBAR="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
				<<" "<<"commit error!"
				<<" "<<"error: "<<QSqlDatabase::database().lastError().text().toStdString();

			if(false == m_pQSqlDataBaseBAR->rollback())  
			{  
				LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
					<<" "<<"m_strSQLiteDBFileNameFullPathBAR="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
					<<" "<<"rollback error!"
					<<" "<<"error: "<<QSqlDatabase::database().lastError().text().toStdString();
			}//if 
		}//if
		else
		{
			LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPathBAR="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
				<<" "<<"commit Transaction";
		}
	}//if

	m_bStartTransactionBAR = false;

	return nFunRes;
}


int CSqliteDbOper::_StartTransactionTIK()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	m_bStartTransactionTIK = false;

	if (false == m_pQSqlDataBaseTIK->driver()->hasFeature(QSqlDriver::Transactions)) 
	{
		m_bStartTransactionTIK = false;

		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPathTIK="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK
			<<" "<<"not support Transactions";
	}
	else
	{
		if (false == m_pQSqlDataBaseTIK->transaction())
		{
			m_bStartTransactionTIK = false;

			LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPathTIK="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK
				<<" "<<"support Transactions but start transaction error!"
				<<" "<<"error: "<<QSqlDatabase::database().lastError().text().toStdString();
		}
		else
		{
			m_bStartTransactionTIK = true;

			LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPathTIK="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK
				<<" "<<"start Transaction";
		}
	}

	return nFunRes;
}


int CSqliteDbOper::_CommitTransactionTIK()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	if (m_bStartTransactionTIK)
	{
		if(false == m_pQSqlDataBaseTIK->commit())  
		{  
			LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPathTIK="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK
				<<" "<<"commit error!"
				<<" "<<"error: "<<QSqlDatabase::database().lastError().text().toStdString();

			if(false == m_pQSqlDataBaseTIK->rollback())  
			{  
				LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
					<<" "<<"m_strSQLiteDBFileNameFullPathTIK="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK
					<<" "<<"rollback error!"
					<<" "<<"error: "<<QSqlDatabase::database().lastError().text().toStdString();
			}//if 
		}//if
		else
		{
			LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPathTIK="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathTIK
				<<" "<<"commit Transaction";
		}
	}//if

	m_bStartTransactionTIK = false;

	return nFunRes;
}



int CSqliteDbOper::_UseTransactionForBatchMode()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	
	_StartTransactionBAR();

	_CommitTransactionBAR();

	return nFunRes;
}






std::string CSqliteDbOper::_GetTableNameBar(unsigned int nTableKey)
{
	BOOST_LOG_FUNCTION();	
	std::string strTableName;
	std::ostringstream sreaamTmp;

	switch (nTableKey)
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
		sreaamTmp<<"bar_data_"<<nTableKey;
		break;
	}

	strTableName = sreaamTmp.str();
	return strTableName;
}



std::string CSqliteDbOper::_BuildSQLForCreateDBTableBar(unsigned int nTableKey)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;
	std::string strTableName;

	strTableName = _GetTableNameBar(nTableKey);

	/*
	enumSqliteDb
	CREATE TABLE IF NOT EXISTS bar_data_15min
	(
	//InstrumentID INTEGER NOT NULL, 
	Timestamp TIMESTAMP NOT NULL, 
	Open decimal(25,10) NOT NULL,
	Close decimal(25,10) NOT NULL,
	High decimal(25,10) NOT NULL,	
	Low decimal(25,10) NOT NULL,
	Volume NUMBER,
	PRIMARY KEY (InstrumentID, Timestamp)
	)
	*/

	{
		sreaamTmp.str("");
		sreaamTmp<<"CREATE TABLE IF NOT EXISTS "<<strTableName
			<<" "<<"("
			//<<" "<<str_BarData_Column_InstrumentID<<" "<<"INTEGER NOT NULL"<<","
			<<" "<<str_BarData_Column_Timestamp<<" "<<"TIMESTAMP NOT NULL"<<","
			<<" "<<str_BarData_Column_Open<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_Close<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_High<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_Low<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_BarData_Column_Volume<<" "<<"int NOT NULL"<<","
			<<" "<<"PRIMARY KEY ("<<str_BarData_Column_Timestamp<<")"
			//<<" "<<"PRIMARY KEY ("<<str_BarData_Column_InstrumentID<<", "<<str_BarData_Column_Timestamp<<")"
			<<" "<<")";
	}
	
	strSQL = sreaamTmp.str();
	return strSQL;
}




int CSqliteDbOper::_CreateDBTableBar(unsigned int nTableKey)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = true;
	std::string strSQL;

	QSqlQuery* pSqlQuery = NULL;
	pSqlQuery = new QSqlQuery(*m_pQSqlDataBaseBAR);

	strSQL = _BuildSQLForCreateDBTableBar(nTableKey);

	LOG_DEBUG<<"exec strSQL="<<strSQL;
	bExecRes = pSqlQuery->exec(strSQL.c_str());
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"Fail to exec sql:"<<strSQL<<" error:"<<pSqlQuery->lastError().text().toStdString();
	}

	if (NULL != pSqlQuery)
	{
		delete pSqlQuery;
		pSqlQuery = NULL;
	}

	return nFunRes;
}


int CSqliteDbOper::_CreateDBTableTick()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = true;
	std::string strSQL;

	QSqlQuery* pSqlQuery = NULL;
	pSqlQuery = new QSqlQuery(*m_pQSqlDataBaseTIK);

	strSQL = _BuildSQLForCreateDBTableTick();

	LOG_DEBUG<<"exec strSQL="<<strSQL;
	bExecRes = pSqlQuery->exec(strSQL.c_str());
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"Fail to exec sql:"<<strSQL<<" error:"<<pSqlQuery->lastError().text().toStdString();
	}

	if (NULL != pSqlQuery)
	{
		delete pSqlQuery;
		pSqlQuery = NULL;
	}

	return nFunRes;
}


int CSqliteDbOper::checkAndInitDBTable(std::list<int>& lstTableKey)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::list<int>::iterator  iterLst;
	int nInterval = 0;

	iterLst = lstTableKey.begin();
	while (iterLst != lstTableKey.end())
	{
		nInterval = (*iterLst);

		_CreateDBTableBar(nInterval);

		iterLst++;
	}//while
	lstTableKey.clear();


	_CreateDBTableTick();

	return nFunRes;
}




int CSqliteDbOper::transactionInsertDataBar(unsigned int nTableKey, LstInstrumentBarInfoT* pListBar )
{
	int nFunRes = 0;

	if (pListBar->empty())
	{
		nFunRes = 0;
		return nFunRes;
	}

	_StartTransactionBAR();

	_AddDataArrayBar(nTableKey, pListBar);


	_CommitTransactionBAR();

	return nFunRes;
}

int CSqliteDbOper::transactionInsertDataTick(LstInstrumentTickInfoT* pListTick)
{
	int nFunRes = 0;

	if (pListTick->empty())
	{
		nFunRes = 0;
		return nFunRes;
	}

	_StartTransactionTIK();

	_AddDataArrayTick(pListTick);


	_CommitTransactionTIK();

	return nFunRes;
}




std::string CSqliteDbOper::_BuildSQLForInsertBarData(unsigned int nTableKey)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;

	std::string strTableName;

	strTableName = _GetTableNameBar(nTableKey);


	/*
	INSERT INTO bar_data_15min
	(
	Timestamp, 
	Open, 
	Close, 
	High,
	Low, 
	Volume
	) 
	VALUES 
	(
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
		<<" "<<"?"
		<<" "<<")";


	strSQL = sreaamTmp.str();
	return strSQL;	
}


std::string CSqliteDbOper::_BuildSQLForInsertTickData()
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;	

	/*
	INSERT INTO tick_data
	(
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
	sreaamTmp<<"INSERT INTO "<<str_Table_tick_data
		<<" "<<"("
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
		<<" "<<"?"
		<<" "<<")";


	strSQL = sreaamTmp.str();
	return strSQL;	
}

int CSqliteDbOper::_AddDataArrayBar(unsigned int nTableKey, LstInstrumentBarInfoT*  pListBar)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = false;
	std::string strSQL;
	std::string strTimeStr;
	LstInstrumentBarInfoIterT iterLst;
	CInstrumentBarInfo* pInstrumentBarInfo = NULL;
	//QVariantList lstInstrumentID;
	QVariantList lstTimestamp;
	QVariantList lstOpen;
	QVariantList lstClose;
	QVariantList lstHigh;
	QVariantList lstLow;
	QVariantList lstVolume;
	QSqlQuery* pQSqlQueryForInseert = NULL;

	if (pListBar->empty())
	{
		nFunRes = 0;
		return nFunRes;
	}

	pQSqlQueryForInseert = new QSqlQuery(*m_pQSqlDataBaseBAR);

	strSQL = _BuildSQLForInsertBarData(nTableKey);

	LOG_DEBUG<<"m_pDBOperParam->m_nInstrumentID="<<m_pDBOperParam->m_nInstrumentID
		<<"  "<<"strSQL="<<strSQL
		<<"  "<<"pLstInstrumentBarInfo.size="<<pListBar->size();

	pQSqlQueryForInseert->prepare(strSQL.c_str());

	iterLst = pListBar->begin();
	while (iterLst != pListBar->end())
	{
		pInstrumentBarInfo = (*iterLst);

		//lstInstrumentID<<m_pDBOperParam->m_nInstrumentID;
		strTimeStr = m_pUtilityFun->dataTimeToStr(pInstrumentBarInfo->m_BarInfo.Time);
		lstTimestamp << strTimeStr.c_str();
		lstOpen<<pInstrumentBarInfo->m_BarInfo.Open;
		lstClose<<pInstrumentBarInfo->m_BarInfo.Close;
		lstHigh<<pInstrumentBarInfo->m_BarInfo.High;
		lstLow<<pInstrumentBarInfo->m_BarInfo.Low;
		lstVolume<<pInstrumentBarInfo->m_BarInfo.Volume;

		iterLst++;
	}//while

	//pQSqlQueryForInseert->addBindValue(lstInstrumentID);
	pQSqlQueryForInseert->addBindValue(lstTimestamp);
	pQSqlQueryForInseert->addBindValue(lstOpen);
	pQSqlQueryForInseert->addBindValue(lstClose);
	pQSqlQueryForInseert->addBindValue(lstHigh);
	pQSqlQueryForInseert->addBindValue(lstLow);
	pQSqlQueryForInseert->addBindValue(lstVolume);

	bExecRes = pQSqlQueryForInseert->execBatch();
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"execBatch strSQL="<<strSQL<<" pLstInstrumentBarInfo.size="<<pListBar->size()
			<<" "<<"error:"<<pQSqlQueryForInseert->lastError().text().toStdString();
	}

	if (NULL != pQSqlQueryForInseert)
	{
		delete pQSqlQueryForInseert;
		pQSqlQueryForInseert = NULL;
	}
	return nFunRes;
}


int CSqliteDbOper::_AddDataArrayTick(LstInstrumentTickInfoT* pLstTick)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = false;
	std::string strDBTableName;
	std::string strSQL;
	std::string strTimeStr;
	LstInstrumentTickInfoIterT iterLst;
	CInstrumentTickInfo* pInstrumentTickInfo = NULL;
	//QVariantList lstInstrumentID;
	QVariantList lstTimestamp;
	QVariantList lstBidPx;
	QVariantList lstAskPx;
	QVariantList lstLastPx;
	QVariantList lstBidVol;
	QVariantList lstAskVol;
	QVariantList lstLastVol;
	QSqlQuery* pQSqlQueryForInseert = NULL;

	if (pLstTick->empty())
	{
		nFunRes = 0;
		return nFunRes;
	}

	pQSqlQueryForInseert = new QSqlQuery(*m_pQSqlDataBaseTIK);

	strDBTableName = str_Table_tick_data;
	strSQL = _BuildSQLForInsertTickData();

	LOG_DEBUG<<"m_pDBOperParam->m_nInstrumentID="<<m_pDBOperParam->m_nInstrumentID
		<<" "<<"strSQL="<<strSQL
		<<" "<<"pLstInstrumentTickInfo.size="<<pLstTick->size();

	pQSqlQueryForInseert->prepare(strSQL.c_str());

	iterLst = pLstTick->begin();
	while (iterLst != pLstTick->end())
	{
		pInstrumentTickInfo = (*iterLst);
		strTimeStr = m_pUtilityFun->dataTimeToStr(pInstrumentTickInfo->m_TickInfo.Time);

		//lstInstrumentID<<m_pDBOperParam->m_nInstrumentID;
		lstTimestamp << strTimeStr.c_str();
		lstBidPx << pInstrumentTickInfo->m_TickInfo.BidPx;
		lstAskPx << pInstrumentTickInfo->m_TickInfo.AskPx;
		lstLastPx << pInstrumentTickInfo->m_TickInfo.LastPx;
		lstBidVol << pInstrumentTickInfo->m_TickInfo.BidVol;
		lstAskVol << pInstrumentTickInfo->m_TickInfo.AskVol;
		lstLastVol << pInstrumentTickInfo->m_TickInfo.LastVol;

		iterLst++;
	}//while

	//pQSqlQueryForInseert->addBindValue(lstInstrumentID);
	pQSqlQueryForInseert->addBindValue(lstTimestamp);
	pQSqlQueryForInseert->addBindValue(lstBidPx);
	pQSqlQueryForInseert->addBindValue(lstAskPx);
	pQSqlQueryForInseert->addBindValue(lstLastPx);
	pQSqlQueryForInseert->addBindValue(lstBidVol);
	pQSqlQueryForInseert->addBindValue(lstAskVol);
	pQSqlQueryForInseert->addBindValue(lstLastVol);

	bExecRes = pQSqlQueryForInseert->execBatch();
	if (!bExecRes)
	{
		nFunRes = -1;
		LOG_ERROR<<"execBatch strSQL="<<strSQL<<" pLstInstrumentTickInfo.size="<<pLstTick->size()
			<<" "<<"error:"<<pQSqlQueryForInseert->lastError().text().toStdString();
	}

	if (NULL != pQSqlQueryForInseert)
	{
		delete pQSqlQueryForInseert;
		pQSqlQueryForInseert = NULL;
	}
	return nFunRes;
}


std::string CSqliteDbOper::_BuildSQLForCreateDBTableTick()
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;

	/*
	enumSqliteDb
	CREATE TABLE IF NOT EXISTS tick_data
	(
	//InstrumentID INTEGER NOT NULL,
	Timestamp TIMESTAMP NOT NULL,
	BidPx decimal(25,10) NOT NULL,
	AskPx decimal(25,10) NOT NULL,
	LastPx decimal(25,10) NOT NULL,
	BidVol int NOT NULL,
	AskVol int NOT NULL,
	LastVol int NOT NULL,
	)
	*/

	{
		sreaamTmp.str("");
		sreaamTmp<<"CREATE TABLE IF NOT EXISTS "<<str_Table_tick_data
			<<" "<<"("
			//<<" "<<str_TickData_Column_InstrumentID<<" "<<"INTEGER NOT NULL"<<","
			<<" "<<str_TickData_Column_Timestamp<<" "<<"TIMESTAMP NOT NULL"<<","
			<<" "<<str_TickData_Column_BidPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_TickData_Column_AskPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_TickData_Column_LastPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_TickData_Column_BidVol<<" "<<"int NOT NULL"<<","
			<<" "<<str_TickData_Column_AskVol<<" "<<"int NOT NULL"<<","
			<<" "<<str_TickData_Column_LastVol<<" "<<"int NOT NULL"
			<<" "<<")";
	}
	
	strSQL = sreaamTmp.str();
	return strSQL;
}



NS_END(TA_Base_App) 



