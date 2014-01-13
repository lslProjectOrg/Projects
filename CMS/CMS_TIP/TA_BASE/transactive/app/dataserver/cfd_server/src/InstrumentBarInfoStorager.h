/*
save data to SQLite
//db name and table name

//5secconds, 1mins��5mins��15mins��30mins��60mins��1day

//SQLiteDB_3620.db
//mysqldb_3620

//bar_data_5seconds
//bar_data_1min
//bar_data_5min
//bar_data_15min
//bar_data_30min
//bar_data_60min
//bar_data_1day
//bar_data_1mon
//bar_data_1year
//bar_data_30day
*/

#ifndef __CLASS_INSTUMENT_BAR_INFO_STORAGER__HH__
#define __CLASS_INSTUMENT_BAR_INFO_STORAGER__HH__

#include "core/DataAccess/src/DataAccessCommonData.h"
#include "CFDServerCommonData.h"

#include <QtSql/QtSql>
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>


#include "InstrumentBarInfoRequest.h"
#include "InstrumentBarInfo.h"
#include "BarCalculator.h"

NS_BEGIN(TA_Base_App) 

class CCFDServerUtilityFun;

class CInstrumentBarInfoStorager 
{
private:
	typedef std::map<int, std::string>				  MapIntervalDBTableNameT;
	typedef std::map<int, std::string>::iterator      MapIntervalDBTableNameIterT;
	typedef std::map<int, std::string>::value_type    MapIntervalDBTableNameValueTypeT;

	typedef std::list<CInstrumentBarInfo*>							  LstInstrumentBarInfoT;
	typedef std::list<CInstrumentBarInfo*>::iterator				  LstInstrumentBarInfoIterT;

	typedef std::map<int, LstInstrumentBarInfoT*>				      MapIntervalBarLstT;
	typedef std::map<int, LstInstrumentBarInfoT*>::iterator           MapIntervalBarLstIterT;
	typedef std::map<int, LstInstrumentBarInfoT*>::value_type         MapIntervalBarLstValueTypeT;
public:
	/*
	eg.		strPathInstrumentBarInfoTotal="C://TestData//InstrumetBarInfoTotal";
			nInstrumentID = 3320
			SQLiteDB file name SQLiteDB_3320.db
	*/
	CInstrumentBarInfoStorager(const CInstrumentBarInfoRequest& instrumentBarInfoRequest, unsigned int nInstrumentID);
	~CInstrumentBarInfoStorager(void);
public:
	int storeBarInfo(int interval, Bar& barInfo);
	void setSoreBatchSize(unsigned int nBatchSize);
	/*fetch data, if no data will return -1*/
	int getBarInfo(int interval, Bar& getBarInfo);
private:
	/*exec sql= SELECT *** */
	int _GetBarInfoFromDB(unsigned int nInstrumentID, int interval);
	void _InitDataBase();
	void _UnInitDataBase();

	std::string _GetBarInfoDBName(const std::string& strPathInstrumentBarInfoTotal, unsigned int nInstrumentID);
	std::string _GetBarInfoDBTableName(int interval);

	int _InsertData(int interval, Bar& barInfo);
	std::string _CheckAndInitDBTable(unsigned int nInstrumentID, int interval);
	int _CreateDBTable(const std::string& strDbTableName);
	std::string _BuildInsertSQL(const std::string& strTableName);
	std::string _BuildSelectSQL(const std::string& strTableName);
	std::string _BuildCreateDBTableSQL(const std::string& strTableName);
	std::string _BuildDropDBTableSQL(const std::string& strDbTableName);

	void _ClearMapIntervalDBarLst(MapIntervalBarLstT* pMapIntervalBarLstT);
	void _ClearInstrumentBarInfoLst(LstInstrumentBarInfoT* plstInstrumentBarInfo);
	int _StoreMapIntervalBarLstBatchMode(MapIntervalBarLstT* pMapIntervalBarLst);
	int _StoreLstInstrumentBarInfoBatchMode(LstInstrumentBarInfoT* pLstInstrumentBarInfo);
	int _InitMapIntervalBarInfoLst(const CInstrumentBarInfoRequest& instrumentBarInfoRequest, MapIntervalBarLstT* pMapIntervalBarLstT);
private:
	unsigned int m_nInstrumentID;
	std::string m_strDBName;//SQLiteDB_3306.db
	std::string m_strDBType;// defSQLiteDBName defMysqlDBName
	TA_Base_App::DbServerType m_nDBType;//enumSqliteDb enumMysqlDb

private:
	QSqlDatabase*		m_pQSqlDataBase;
	QSqlQuery*			m_pQSqlQueryForSelect;
	CCFDServerUtilityFun*   m_pUtilityFun;
	MapIntervalDBTableNameT* m_pmapIntervalDBTableName;
	CInstrumentBarInfoRequest m_InstrumentBarInfoRequest;

	MapIntervalBarLstT*    m_pMapIntervalBarLst;
	unsigned int       m_nBatchSize;
	unsigned int       m_nBuffNum;
};

NS_END(TA_Base_App) 


#endif //__CLASS_INSTUMENT_BAR_INFO_STORAGER__HH__


