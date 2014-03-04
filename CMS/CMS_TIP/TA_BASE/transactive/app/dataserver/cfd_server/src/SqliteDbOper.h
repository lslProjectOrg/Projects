#ifndef __CLASS_SQLITE_DB_OPER__HH__
#define __CLASS_SQLITE_DB_OPER__HH__

#include "CFDServerCommonData.h"

#include "IDbOper.h"

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
#include "InstrumentTickInfo.h"
#include "BarCalculator.h"


#include "DBOperParam.h"

NS_BEGIN(TA_Base_App) 

class CCFDServerUtilityFun;

class CSqliteDbOper : public IDbOper
{
public:
	CSqliteDbOper(const CDBOperParam&  dbOperParam);
	~CSqliteDbOper(void);
public:
	int checkAndInitDBTable(std::list<int>& lstTableKey);
	int transactionInsertDataBar(unsigned int nTableKey, LstInstrumentBarInfoT* pListBar);
	int transactionInsertDataTick(LstInstrumentTickInfoT* pListTick);


private:
	void _InitDataBase();
	void _UnInitDataBase();
	int _CommitTransactionBAR();
	int _StartTransactionBAR();
	int _StartTransactionTIK();
	int _CommitTransactionTIK();

	int _UseTransactionForBatchMode();
private:
	std::string _GetTableNameBar(unsigned int nTableKey);
	std::string _BuildSQLForCreateDBTableBar(unsigned int nTableKey);
	int _CreateDBTableBar(unsigned int nTableKey);
	int _AddDataArrayBar(unsigned int nTableKey, LstInstrumentBarInfoT* pListBar);
	std::string _BuildSQLForInsertBarData(unsigned int nTableKey);
	int _AddDataArrayTick(LstInstrumentTickInfoT* pLstTick);
	std::string _BuildSQLForCreateDBTableTick();
	int _CreateDBTableTick();
	std::string _BuildSQLForInsertTickData();
private:
	QSqlDatabase*		m_pQSqlDataBaseBAR;
	QSqlDatabase*		m_pQSqlDataBaseTIK;
	bool                m_bStartTransactionBAR;
	bool                m_bStartTransactionTIK;


private:
	CDBOperParam*  m_pDBOperParam;
	CCFDServerUtilityFun*  m_pUtilityFun;
};

NS_END(TA_Base_App) 


#endif //__CLASS_SQLITE_DB_OPER__HH__


