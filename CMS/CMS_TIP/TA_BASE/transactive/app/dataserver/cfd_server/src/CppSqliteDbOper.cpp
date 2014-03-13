#include "CppSqliteDbOper.h"

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



CCppSqliteDbOper::CCppSqliteDbOper( const CDBOperParam& dbOperParam ) : IDbOper()
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

CCppSqliteDbOper::~CCppSqliteDbOper(void)
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

void CCppSqliteDbOper::_InitDataBase()
{
	bool bExecRes = false;
	std::string strDBFileName;

	_UnInitDataBase();

	switch (m_pDBOperParam->m_nDataSrcDbType)
	{
	case TA_Base_App::enumSqliteDb:
		//SQLiteDB_3306.db
		m_pQSqlDataBase = new CppSQLite3DB();
		//d://savedata//SQLiteDB_3306.db
		m_pQSqlDataBase->open(m_pDBOperParam->m_strSQLiteDBFileNameFullPath.c_str());
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"CppSQLite3DB"
			<<" "<<"new Database  m_nDataSrcDbType="<<m_pDBOperParam->m_nDataSrcDbType
			<<" "<<"m_strSQLiteDBFileName="<<m_pDBOperParam->m_strSQLiteDBFileName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath;
		break;
	default:
		break;
	}
}

void CCppSqliteDbOper::_UnInitDataBase()
{

	if (NULL != m_pQSqlDataBase)
	{
		LOG_INFO<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"CppSQLite3DB"
			<<" "<<"delete Database  m_nDataSrcDbType="<<m_pDBOperParam->m_nDataSrcDbType
			<<" "<<"m_strSQLiteDBFileName="<<m_pDBOperParam->m_strSQLiteDBFileName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath;

		m_pQSqlDataBase->close();
		delete m_pQSqlDataBase;
		m_pQSqlDataBase = NULL;
		//QSqlDatabase::removeDatabase(m_pDBOperParam->m_strSQLiteDBFileName.c_str());
	}

}




int CCppSqliteDbOper::_StartTransaction()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	m_bStartTransaction = false;

	try
	{
		m_pQSqlDataBase->execDML("BEGIN TRANSACTION"); 
		m_bStartTransaction = true;
		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"start Transaction";

	}
	catch (CppSQLite3Exception& e)
	{
		m_bStartTransaction = false;

		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"support Transactions but start transaction error!"
			<<" "<<"error: "<<e.errorCode()<<" "<<e.errorMessage();

	}
	catch (...)
	{
		m_bStartTransaction = false;

		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"support Transactions but start transaction error!";
	}

	return nFunRes;
}



int CCppSqliteDbOper::_CommitTransaction()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bNeedRoolBack = false;

	if (!m_bStartTransaction)
	{
		nFunRes = 0;
		return nFunRes;
	}


	try
	{
		m_pQSqlDataBase->execDML("COMMIT TRANSACTION"); 
		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"COMMIT Transaction";
		bNeedRoolBack = false;
	}
	catch (CppSQLite3Exception& e)
	{
		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"commit error!"
			<<" "<<"error: "<<e.errorCode()<<" "<<e.errorMessage();
		bNeedRoolBack = true;
	}
	catch (...)
	{
		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"commit error!";
		bNeedRoolBack = true; 
	}
	m_bStartTransaction = false;


	if (!bNeedRoolBack)
	{
		nFunRes = 0;
		return nFunRes;
	}

	try
	{
		m_pQSqlDataBase->execDML("ROOLBACK TRANSACTION"); 
		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"ROOLBACK Transaction";
	}
	catch (CppSQLite3Exception& e)
	{
		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"ROOLBACK error!"
			<<" "<<"error: "<<e.errorCode()<<" "<<e.errorMessage();
	}
	catch (...)
	{
		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"ROOLBACK error!";
	}

	return nFunRes;
}


std::string CCppSqliteDbOper::_GetTableNameBar(unsigned int nTableKey)
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



std::string CCppSqliteDbOper::_BuildSQLForCreateDBTableBar(unsigned int nTableKey)
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




int CCppSqliteDbOper::_CreateDBTableBar(unsigned int nTableKey)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	int nExecRes = 0;
	std::string strSQL;

	strSQL = _BuildSQLForCreateDBTableBar(nTableKey);

	LOG_DEBUG<<"begin exec strSQL="<<strSQL;

	try
	{
		nExecRes = m_pQSqlDataBase->execDML(strSQL.c_str());
		LOG_DEBUG<<"end exec strSQL="<<strSQL;
	}
	catch (CppSQLite3Exception& e)
	{
		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"Fail to exec sql:"<<strSQL
			<<" "<<"error: "<<e.errorCode()<<" "<<e.errorMessage();
	}
	catch (...)
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"Fail to exec sql:"<<strSQL;
	}

	return nFunRes;
}


int CCppSqliteDbOper::_CreateDBTableTick()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	int nExecRes = 0;
	std::string strSQL;

	strSQL = _BuildSQLForCreateDBTableTick();

	LOG_DEBUG<<"begin exec strSQL="<<strSQL;

	try
	{
		nExecRes = m_pQSqlDataBase->execDML(strSQL.c_str());
		LOG_DEBUG<<"end exec strSQL="<<strSQL;
	}
	catch (CppSQLite3Exception& e)
	{
		LOG_DEBUG<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"Fail to exec sql:"<<strSQL
			<<" "<<"error: "<<e.errorCode()<<" "<<e.errorMessage();
	}
	catch (...)
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"Fail to exec sql:"<<strSQL;
	}

	return nFunRes;
}


int CCppSqliteDbOper::checkAndInitDBTable(std::list<int>& lstTableKey)
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




int CCppSqliteDbOper::transactionInsertDataBar(unsigned int nTableKey, LstInstrumentBarInfoT* pListBar )
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

int CCppSqliteDbOper::transactionInsertDataTick(LstInstrumentTickInfoT* pListTick)
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




std::string CCppSqliteDbOper::_BuildSQLForInsertBarData(unsigned int nTableKey)
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
		<<" "<<str_BarData_Column_InstrumentID_Value<<","
		<<" "<<str_BarData_Column_Timestamp_Value<<","
		<<" "<<str_BarData_Column_Open_Value<<","
		<<" "<<str_BarData_Column_Close_Value<<","
		<<" "<<str_BarData_Column_High_Value<<","
		<<" "<<str_BarData_Column_Low_Value<<","
		<<" "<<str_BarData_Column_Volume_Value
		<<" "<<")";

	strSQL = sreaamTmp.str();
	return strSQL;	
}


std::string CCppSqliteDbOper::_BuildSQLForInsertTickData()
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
		<<" "<<str_TickData_Column_InstrumentID_Value<<","
		<<" "<<str_TickData_Column_Timestamp_Value<<","
		<<" "<<str_TickData_Column_BidPx_Value<<","
		<<" "<<str_TickData_Column_AskPx_Value<<","
		<<" "<<str_TickData_Column_LastPx_Value<<","
		<<" "<<str_TickData_Column_BidVol_Value<<","
		<<" "<<str_TickData_Column_AskVol_Value<<","
		<<" "<<str_TickData_Column_LastVol_Value
		<<" "<<")";

	strSQL = sreaamTmp.str();
	return strSQL;	
}

int CCppSqliteDbOper::_AddDataArrayBar(unsigned int nTableKey, LstInstrumentBarInfoT*  pListBar)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = false;
	std::string strSQL;
	std::string strTimeStr;
	LstInstrumentBarInfoIterT iterLst;
	CInstrumentBarInfo* pInstrumentBarInfo = NULL;
	CppSQLite3Statement stmt;

	if (pListBar->empty())
	{
		nFunRes = 0;
		return nFunRes;
	}

	strSQL = _BuildSQLForInsertBarData(nTableKey);

	LOG_DEBUG<<"m_pDBOperParam->m_nInstrumentID="<<m_pDBOperParam->m_nInstrumentID
		<<"  "<<"strSQL="<<strSQL
		<<"  "<<"pLstInstrumentBarInfo.size="<<pListBar->size();

	stmt = m_pQSqlDataBase->compileStatement(strSQL.c_str());

	try
	{
		iterLst = pListBar->begin();
		while (iterLst != pListBar->end())
		{
			pInstrumentBarInfo = (*iterLst);

			stmt.bind(str_BarData_Column_InstrumentID_Value.c_str(), (const int)m_pDBOperParam->m_nInstrumentID);
			strTimeStr = m_pUtilityFun->dataTimeToStr(pInstrumentBarInfo->m_BarInfo.Time);
			stmt.bind(str_BarData_Column_Timestamp_Value.c_str(), strTimeStr.c_str());
// 			stmt.bind(str_BarData_Column_Open_Value.c_str(), (const double)pInstrumentBarInfo->m_BarInfo.Open);
// 			stmt.bind(str_BarData_Column_Close_Value.c_str(), (const double)pInstrumentBarInfo->m_BarInfo.Close);
// 			stmt.bind(str_BarData_Column_High_Value.c_str(), (const double)pInstrumentBarInfo->m_BarInfo.High);
// 			stmt.bind(str_BarData_Column_Low_Value.c_str(), (const double)pInstrumentBarInfo->m_BarInfo.Low);

			stmt.bind(str_BarData_Column_Open_Value.c_str(), (const double)1.0);
			stmt.bind(str_BarData_Column_Close_Value.c_str(), (const double)1.0);
			stmt.bind(str_BarData_Column_High_Value.c_str(), (const double)1.0);
			stmt.bind(str_BarData_Column_Low_Value.c_str(), (const double)1.0);

			stmt.bind(str_BarData_Column_Volume_Value.c_str(), pInstrumentBarInfo->m_BarInfo.Volume);

			stmt.execDML();
			stmt.reset();

			iterLst++;
		}//while

	}
	catch (CppSQLite3Exception& e)
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"Fail to execBatch sql:"<<strSQL <<" pLstInstrumentBarInfo.size="<<pListBar->size()
		    <<" "<<"error: "<<e.errorCode()<<" "<<e.errorMessage();
	}
	catch (...)
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"Fail to execBatch sql:"<<strSQL <<" pLstInstrumentBarInfo.size="<<pListBar->size();
	}


	return nFunRes;
}


int CCppSqliteDbOper::_AddDataArrayTick(LstInstrumentTickInfoT* pLstTick)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::string strDBTableName;
	std::string strSQL;
	std::string strTimeStr;
	LstInstrumentTickInfoIterT iterLst;
	CInstrumentTickInfo* pInstrumentTickInfo = NULL;
	CppSQLite3Statement stmt;


	if (pLstTick->empty())
	{
		nFunRes = 0;
		return nFunRes;
	}

	strSQL = _BuildSQLForInsertTickData();

	LOG_DEBUG<<"m_pDBOperParam->m_nInstrumentID="<<m_pDBOperParam->m_nInstrumentID
		<<" "<<"strSQL="<<strSQL
		<<" "<<"pLstInstrumentTickInfo.size="<<pLstTick->size();

	stmt = m_pQSqlDataBase->compileStatement(strSQL.c_str());

	try
	{
		iterLst = pLstTick->begin();
		while (iterLst != pLstTick->end())
		{
			pInstrumentTickInfo = (*iterLst);
			strTimeStr = m_pUtilityFun->dataTimeToStr(pInstrumentTickInfo->m_TickInfo.Time);

			stmt.bind(str_TickData_Column_InstrumentID_Value.c_str(), (const int)m_pDBOperParam->m_nInstrumentID);
			stmt.bind(str_TickData_Column_Timestamp_Value.c_str(), strTimeStr.c_str());
// 			stmt.bind(str_TickData_Column_BidPx_Value.c_str(), (const double)pInstrumentTickInfo->m_TickInfo.BidPx);
// 			stmt.bind(str_TickData_Column_AskPx_Value.c_str(), (const double)pInstrumentTickInfo->m_TickInfo.AskPx);
// 			stmt.bind(str_TickData_Column_LastPx_Value.c_str(), (const double)pInstrumentTickInfo->m_TickInfo.LastPx);
			stmt.bind(str_TickData_Column_BidPx_Value.c_str(), (const double)1.0);
			stmt.bind(str_TickData_Column_AskPx_Value.c_str(), (const double)1.0);
			stmt.bind(str_TickData_Column_LastPx_Value.c_str(), (const double)1.0);

			stmt.bind(str_TickData_Column_BidVol_Value.c_str(), pInstrumentTickInfo->m_TickInfo.BidVol);
			stmt.bind(str_TickData_Column_AskVol_Value.c_str(), pInstrumentTickInfo->m_TickInfo.AskVol);
			stmt.bind(str_TickData_Column_LastVol_Value.c_str(), pInstrumentTickInfo->m_TickInfo.LastVol);

			stmt.execDML();
			stmt.reset();

			iterLst++;
		}//while

	}
	catch (CppSQLite3Exception& e)
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"Fail to execBatch sql:"<<strSQL <<" pLstInstrumentBarInfo.size="<<pLstTick->size()
			<<" "<<"error: "<<e.errorCode()<<" "<<e.errorMessage();
	}
	catch (...)
	{
		LOG_ERROR<<m_pDBOperParam->m_strQTDataSrcDbTypeName
			<<" "<<"m_strSQLiteDBFileNameFullPath="<<m_pDBOperParam->m_strSQLiteDBFileNameFullPath
			<<" "<<"Fail to execBatch sql:"<<strSQL <<" pLstInstrumentBarInfo.size="<<pLstTick->size();
	}


	return nFunRes;
}


std::string CCppSqliteDbOper::_BuildSQLForCreateDBTableTick()
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;

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

	{
		sreaamTmp.str("");
		sreaamTmp<<"CREATE TABLE IF NOT EXISTS "<<str_Table_tick_data
			<<" "<<"("
			<<" "<<str_TickData_Column_InstrumentID<<" "<<"INTEGER NOT NULL"<<","
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



