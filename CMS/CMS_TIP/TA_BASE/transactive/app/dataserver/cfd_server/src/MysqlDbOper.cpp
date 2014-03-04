#include "MysqlDbOper.h"

#include "CFDServerUtilityFun.h"

#include <boost/algorithm/string.hpp>

#include "BoostLogger.h"
USING_BOOST_LOG;

using namespace TA_Base_App;


NS_BEGIN(TA_Base_App)


//boost::mutex g_mutex_Database_Mysql;
int g_n_SqlDataBase_Mysql_Ref = 0;
QSqlDatabase*	g_SqlDataBase_Mysql = NULL;


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



CMysqlDbOper::CMysqlDbOper( const CDBOperParam& dbOperParam ): IDbOper()
{
	BOOST_LOG_FUNCTION();
	
	m_pUtilityFun = NULL;
	m_pDBOperParam = NULL;
	m_pQSqlDataBase = NULL;
	m_bStartTransaction = false;

	m_pUtilityFun = new CCFDServerUtilityFun();

	m_pDBOperParam = new CDBOperParam();
	*m_pDBOperParam = dbOperParam;


	_InitDataBase();
}

CMysqlDbOper::~CMysqlDbOper(void)
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

void CMysqlDbOper::_InitDataBase()
{
	bool bExecRes = false;
	std::string strDBFileName;
	//boost::mutex::scoped_lock lock(g_mutex_Database_Mysql);

	switch (m_pDBOperParam->m_nDataSrcDbType)
	{
	case TA_Base_App::enumMysqlDb:

		if (0 == g_n_SqlDataBase_Mysql_Ref || NULL == g_SqlDataBase_Mysql)
		{
			m_pQSqlDataBase = new QSqlDatabase(QSqlDatabase::addDatabase(m_pDBOperParam->m_strQTDataSrcDbTypeName.c_str(), QLatin1String(m_pDBOperParam->m_strMysqlSchema.c_str())));
			m_pQSqlDataBase->setDatabaseName(m_pDBOperParam->m_strMysqlSchema.c_str());//oms
			m_pQSqlDataBase->setHostName(m_pDBOperParam->m_strMysqlIP.c_str());
			m_pQSqlDataBase->setUserName(m_pDBOperParam->m_strMysqlUser.c_str());
			m_pQSqlDataBase->setPassword(m_pDBOperParam->m_strMysqlPWD.c_str());

			LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"new Database  m_nDataSrcDbType="<<m_pDBOperParam->m_nDataSrcDbType
				<<" "<<"m_strMysqlSchema="<<m_pDBOperParam->m_strMysqlSchema
				<<" "<<"m_strMysqlIP="<<m_pDBOperParam->m_strMysqlIP
				<<" "<<"m_strMysqlUser="<<m_pDBOperParam->m_strMysqlUser
				<<" "<<"m_strMysqlPWD="<<m_pDBOperParam->m_strMysqlPWD;

			g_SqlDataBase_Mysql = m_pQSqlDataBase;
			g_n_SqlDataBase_Mysql_Ref++;
		}
		else
		{
			m_pQSqlDataBase = g_SqlDataBase_Mysql;
			g_n_SqlDataBase_Mysql_Ref++;
		}
		break;
	default:
		break;
	}

	bExecRes = m_pQSqlDataBase->open();
	if (!bExecRes)
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR<<" Fail to open!"
			<<" "<<"error:"<<m_pQSqlDataBase->lastError().text().toStdString();
	}


	//check
	if (false == m_pQSqlDataBase->isValid())
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR<<" is not Valid";
	}
	else
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR<<" is Valid";
	}

	if (false == m_pQSqlDataBase->driver()->hasFeature(QSqlDriver::Transactions)) 
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR<<" not support Transactions";
	}
	else
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName<<" "<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR<<" support Transactions";

	}
}

void CMysqlDbOper::_UnInitDataBase()
{
	//boost::mutex::scoped_lock lock(g_mutex_Database_Mysql);

	m_pQSqlDataBase = NULL;
	if (0 < g_n_SqlDataBase_Mysql_Ref)
	{
		g_n_SqlDataBase_Mysql_Ref--;
	}

	if (0 == g_n_SqlDataBase_Mysql_Ref && NULL != g_SqlDataBase_Mysql)
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"delete Database  m_nDataSrcDbType="<<m_pDBOperParam->m_nDataSrcDbType
			<<" "<<"m_strMysqlSchema="<<m_pDBOperParam->m_strMysqlSchema
			<<" "<<"m_strMysqlIP="<<m_pDBOperParam->m_strMysqlIP
			<<" "<<"m_strMysqlUser="<<m_pDBOperParam->m_strMysqlUser
			<<" "<<"m_strMysqlPWD="<<m_pDBOperParam->m_strMysqlPWD;

		g_SqlDataBase_Mysql->close();
		delete g_SqlDataBase_Mysql;
		g_SqlDataBase_Mysql = NULL;
		//QSqlDatabase::removeDatabase(m_pDBOperParam->m_strSQLiteDBFileName.c_str());
	}

}




int CMysqlDbOper::_StartTransaction()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	m_bStartTransaction = false;

	if (false == m_pQSqlDataBase->driver()->hasFeature(QSqlDriver::Transactions)) 
	{
		m_bStartTransaction = false;

		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
			<<" "<<"not support Transactions";
	}
	else
	{
		if (false == m_pQSqlDataBase->transaction())
		{
			m_bStartTransaction = false;

			LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
				<<" "<<"support Transactions but start transaction error!"
				<<" "<<"error: "<<QSqlDatabase::database().lastError().text().toStdString();
		}
		else
		{
			m_bStartTransaction = true;

			LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
				<<" "<<"start Transaction";
		}
	}

	return nFunRes;
}



int CMysqlDbOper::_CommitTransaction()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	if (m_bStartTransaction)
	{
		if(false == m_pQSqlDataBase->commit())  
		{  
			LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
				<<" "<<"commit error!"
				<<" "<<"error: "<<QSqlDatabase::database().lastError().text().toStdString();

			if(false == m_pQSqlDataBase->rollback())  
			{  
				LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
					<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
					<<" "<<"rollback error!"
					<<" "<<"error: "<<QSqlDatabase::database().lastError().text().toStdString();
			}//if 
		}//if
		else
		{
			LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
				<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPathBAR
				<<" "<<"commit Transaction";
		}
	}//if

	m_bStartTransaction = false;

	return nFunRes;
}




int CMysqlDbOper::_UseTransactionForBatchMode()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	
	_StartTransaction();

	_CommitTransaction();

	return nFunRes;
}






std::string CMysqlDbOper::_GetTableNameBar(unsigned int nTableKey)
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



std::string CMysqlDbOper::_BuildSQLForCreateDBTableBar(unsigned int nTableKey)
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
	
	strSQL = sreaamTmp.str();
	return strSQL;
}




int CMysqlDbOper::_CreateDBTableBar(unsigned int nTableKey)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = true;
	std::string strSQL;

	QSqlQuery* pSqlQuery = NULL;
	pSqlQuery = new QSqlQuery(*m_pQSqlDataBase);

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


int CMysqlDbOper::_CreateDBTableTick()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = true;
	std::string strSQL;

	QSqlQuery* pSqlQuery = NULL;
	pSqlQuery = new QSqlQuery(*m_pQSqlDataBase);

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


int CMysqlDbOper::checkAndInitDBTable(std::list<int>& lstTableKey)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::list<int>::iterator  iterLst;
	int nInterval = 0;
	//boost::mutex::scoped_lock lock(g_mutex_Database_Mysql);

	if (1 != g_n_SqlDataBase_Mysql_Ref)
	{
		return nFunRes;
	}

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






int CMysqlDbOper::transactionInsertDataBar(unsigned int nTableKey, LstInstrumentBarInfoT* pListBar )
{
	int nFunRes = 0;

	if (pListBar->empty())
	{
		nFunRes = 0;
		return nFunRes;
	}

	_StartTransaction();

	_AddDataArrayBar(nTableKey, pListBar);


	_CommitTransaction();

	return nFunRes;
}

int CMysqlDbOper::transactionInsertDataTick(LstInstrumentTickInfoT* pListTick)
{
	int nFunRes = 0;

	if (pListTick->empty())
	{
		nFunRes = 0;
		return nFunRes;
	}

	_StartTransaction();

	_AddDataArrayTick(pListTick);


	_CommitTransaction();

	return nFunRes;
}




std::string CMysqlDbOper::_BuildSQLForInsertBarData(unsigned int nTableKey)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;

	std::string strTableName;

	strTableName = _GetTableNameBar(nTableKey);


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


std::string CMysqlDbOper::_BuildSQLForInsertTickData()
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
	sreaamTmp<<"INSERT INTO "<<str_Table_tick_data
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

int CMysqlDbOper::_AddDataArrayBar(unsigned int nTableKey, LstInstrumentBarInfoT*  pListBar)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = false;
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
	QSqlQuery* pQSqlQueryForInseert = NULL;

	if (pListBar->empty())
	{
		nFunRes = 0;
		return nFunRes;
	}

	pQSqlQueryForInseert = new QSqlQuery(*m_pQSqlDataBase);

	strSQL = _BuildSQLForInsertBarData(nTableKey);

	LOG_DEBUG<<"m_pDBOperParam->m_nInstrumentID="<<m_pDBOperParam->m_nInstrumentID
		<<"  "<<"strSQL="<<strSQL
		<<"  "<<"pLstInstrumentBarInfo.size="<<pListBar->size();

	pQSqlQueryForInseert->prepare(strSQL.c_str());

	iterLst = pListBar->begin();
	while (iterLst != pListBar->end())
	{
		pInstrumentBarInfo = (*iterLst);

		lstInstrumentID<<m_pDBOperParam->m_nInstrumentID;
		strTimeStr = m_pUtilityFun->dataTimeToStr(pInstrumentBarInfo->m_BarInfo.Time);
		lstTimestamp << strTimeStr.c_str();
		lstOpen<<pInstrumentBarInfo->m_BarInfo.Open;
		lstClose<<pInstrumentBarInfo->m_BarInfo.Close;
		lstHigh<<pInstrumentBarInfo->m_BarInfo.High;
		lstLow<<pInstrumentBarInfo->m_BarInfo.Low;
		lstVolume<<pInstrumentBarInfo->m_BarInfo.Volume;

		iterLst++;
	}//while

	pQSqlQueryForInseert->addBindValue(lstInstrumentID);
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


int CMysqlDbOper::_AddDataArrayTick(LstInstrumentTickInfoT* pLstTick)
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
	QSqlQuery* pQSqlQueryForInseert = NULL;

	if (pLstTick->empty())
	{
		nFunRes = 0;
		return nFunRes;
	}

	pQSqlQueryForInseert = new QSqlQuery(*m_pQSqlDataBase);

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

		lstInstrumentID<<m_pDBOperParam->m_nInstrumentID;
		lstTimestamp << strTimeStr.c_str();
		lstBidPx << pInstrumentTickInfo->m_TickInfo.BidPx;
		lstAskPx << pInstrumentTickInfo->m_TickInfo.AskPx;
		lstLastPx << pInstrumentTickInfo->m_TickInfo.LastPx;
		lstBidVol << pInstrumentTickInfo->m_TickInfo.BidVol;
		lstAskVol << pInstrumentTickInfo->m_TickInfo.AskVol;
		lstLastVol << pInstrumentTickInfo->m_TickInfo.LastVol;

		iterLst++;
	}//while

	pQSqlQueryForInseert->addBindValue(lstInstrumentID);
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


std::string CMysqlDbOper::_BuildSQLForCreateDBTableTick()
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;

	
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
	*/

	//TIMESTR--%Y%m%d%H%M%S
	//%04d-%02d-%02d %02d:%02d:%02d

	{

		sreaamTmp.str("");
		sreaamTmp<<"CREATE TABLE IF NOT EXISTS "<<str_Table_tick_data
			<<" "<<"("
			<<" "<<str_TickData_Column_InstrumentID<<" "<<"int(10) unsigned NOT NULL"<<","
			<<" "<<str_TickData_Column_Timestamp<<" "<<"datetime NOT NULL"<<","
			<<" "<<str_TickData_Column_BidPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_TickData_Column_AskPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_TickData_Column_LastPx<<" "<<"decimal(25,10) NOT NULL"<<","
			<<" "<<str_TickData_Column_BidVol<<" "<<"int(10) unsigned NOT NULL"<<","
			<<" "<<str_TickData_Column_AskVol<<" "<<"int(10) unsigned NOT NULL"<<","
			<<" "<<str_TickData_Column_LastVol<<" "<<"int(10) unsigned NOT NULL"
			<<" "<<")";	
	}
	
	strSQL = sreaamTmp.str();
	return strSQL;
}



NS_END(TA_Base_App) 



