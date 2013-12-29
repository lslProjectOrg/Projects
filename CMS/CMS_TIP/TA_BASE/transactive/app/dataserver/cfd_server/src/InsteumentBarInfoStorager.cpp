#include "InsteumentBarInfoStorager.h"


#include "MarketData.h"
#include "BarCalculator.h"
#include "CFDServerUtilityFun.h"


#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_App)


//////////////////////////////////////////////////////////////////////////

CInstrumentBarInfoStorager::CInstrumentBarInfoStorager(unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_pmapIntervalDBTableName = new MapIntervalDBTableNameT();
	m_nInstrumentID = nInstrumentID;
	m_strSQLiteDBName = _GetDBName(nInstrumentID);
	m_QSqlDataBase = QSqlDatabase::addDatabase("QSQLITE", m_strSQLiteDBName.c_str());	
	m_QSqlDataBase.setDatabaseName(m_strSQLiteDBName.c_str());
	m_QSqlDataBase.open();
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

	m_QSqlDataBase.close();


}

std::string CInstrumentBarInfoStorager::_GetDBName(unsigned int nInstrumentID)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strInstrumentSQLDBName;
	sreaamTmp.str("");
	sreaamTmp<<"SQLiteDB_"<<nInstrumentID<<".db";
	strInstrumentSQLDBName = sreaamTmp.str();

	return strInstrumentSQLDBName;
}
std::string CInstrumentBarInfoStorager::_GetDBTableName(unsigned int nInstrumentID, int interval)
{
	BOOST_LOG_FUNCTION();

	//find
	std::string strTableName;
	MapIntervalDBTableNameIterT  iterMap;
	std::ostringstream sreaamTmp;

	iterMap = m_pmapIntervalDBTableName->find(interval);
	if (iterMap != m_pmapIntervalDBTableName->end())
	{
		strTableName = (iterMap->second);
		return strTableName;
	}

	sreaamTmp.str("");
	sreaamTmp<<"Table_BarInfo_"<<nInstrumentID<<"_"<<interval;
	strTableName = sreaamTmp.str();
	_CreateDBTable(strTableName);
	m_pmapIntervalDBTableName->insert(MapIntervalDBTableNameValueTypeT(interval, strTableName));
	return strTableName;
}



int CInstrumentBarInfoStorager::_CreateDBTable(const std::string& strDbTableName)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::ostringstream sreaamTmp;
	std::string strSQL;
	//create table Table_BarInfo(INDEX Integer primary key, value text);
	//insert into Table_BarInfo values(null,'Acuzio');
	//select last_insert_rowid();

	/*
	CREATE TABLE Table_BarInfo(SEQINDEX INTEGER PRIMARY KEY, INTERVAL NUMBER,
	INSTRUMENTID NUMBER, TIME NUMBER, TIMESTR TIMESTAMP
	OPEN NUMBER(20,8), CLOSE NUMBER(20,8), HIGH NUMBER(20,8), LOW NUMBER(20,8),
	VOLUME NUMBER)
	//TIMESTR--%Y%m%d%H%M%S
	*/
	sreaamTmp.str("");
	sreaamTmp<<"CREATE TABLE IF NOT EXISTS "<<strDbTableName
		<<" "<<"(SEQINDEX INTEGER PRIMARY KEY"<<","
		<<" "<<"INTERVAL NUMBER"<<","
		<<" "<<"INSTRUMENTID NUMBER"<<","
		<<" "<<"TIME NUMBER"<<","
		<<" "<<"TIMESTR TIMESTAMP"<<","
		<<" "<<"OPEN NUMBER(20,8)"<<","
		<<" "<<"CLOSE NUMBER(20,8)"<<","
		<<" "<<"HIGH NUMBER(20,8)"<<","
		<<" "<<"LOW NUMBER(20,8)"<<","
		<<" "<<"VOLUME NUMBER)";

	strSQL = sreaamTmp.str();
	nFunRes = _Exec(strSQL);
	return nFunRes;
}

int CInstrumentBarInfoStorager::_DropDBTable(const std::string& strDbTableName)
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


std::string CInstrumentBarInfoStorager::_BuildInsertSQL(const std::string& strTableName)
{
	BOOST_LOG_FUNCTION();
	std::ostringstream sreaamTmp;
	std::string strSQL;

	//TIMESTR--%Y%m%d%H%M%S
	/*
	INSERT INTO Table_BarInfo 
	(
	SEQINDEX, INTERVAL, INSTRUMENTID, TIME, TIMESTR, 
	OPEN, CLOSE, HIGH, LOW, VOLUME
	) VALUES 
	(
	NULL, :INTERVAL_Value, :INSTRUMENTID_Value, :TIME_Value, :TIMESTR_Value, 
	:OPEN_Value, :CLOSE_Value, :HIGH_Value, :LOW_Value, :VOLUME_Value
	}
	*/
	sreaamTmp.str("");
	sreaamTmp<<"INSERT INTO "<<strTableName
		<<" "<<"("
		<<" "<<"SEQINDEX, INTERVAL, INSTRUMENTID, TIME, TIMESTR,"
		<<" "<<"OPEN, CLOSE, HIGH, LOW, VOLUME"
		<<" "<<")"
		<<" "<<"VALUES"
		<<" "<<"("
		<<" "<<"NULL, :INTERVAL_Value, :INSTRUMENTID_Value, :TIME_Value, :TIMESTR_Value,"
		<<" "<<":OPEN_Value, :CLOSE_Value, :HIGH_Value, :LOW_Value, :VOLUME_Value"
		<<" "<<")";

	strSQL = sreaamTmp.str();
	return strSQL;	
}

int CInstrumentBarInfoStorager::_InsertData(int interval, Bar* pBarInfo)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	bool bExecRes = true;
	std::string strTimeStr;
	QSqlQuery query(m_QSqlDataBase);

	std::string strDBTableName;
	std::string strSQL;
	strDBTableName = _GetDBTableName(m_nInstrumentID, interval);
	strSQL = _BuildInsertSQL(strDBTableName);
	LOG_DEBUG<<"strSQL="<<strSQL;

	query.prepare(strSQL.c_str());
	query.bindValue(":INTERVAL_Value", interval);
	query.bindValue(":INSTRUMENTID_Value", m_nInstrumentID);
	query.bindValue(":TIME_Value", pBarInfo->Time);
	strTimeStr = m_pUtilityFun->getTimeStringForSQL(pBarInfo->Time);
	query.bindValue(":TIMESTR_Value", strTimeStr.c_str());
	query.bindValue(":OPEN_Value", pBarInfo->Open);
	query.bindValue(":CLOSE_Value", pBarInfo->Close);
	query.bindValue(":HIGH_Value", pBarInfo->High);
	query.bindValue(":LOW_Value", pBarInfo->Low);
	query.bindValue(":VOLUME_Value", pBarInfo->Volume);

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

	QSqlQuery query(m_QSqlDataBase);
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





NS_END(TA_Base_App) 



