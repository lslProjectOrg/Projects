#ifndef __CLASS_CPP_SQLITE_DB_OPER__HH__
#define __CLASS_CPP_SQLITE_DB_OPER__HH__

#include "CFDServerCommonData.h"

#include "IDbOper.h"
#include "CppSQLite3.h"

#include "InstrumentBarInfoRequest.h"
#include "InstrumentBarInfo.h"
#include "InstrumentTickInfo.h"
#include "BarCalculator.h"


#include "DBOperParam.h"

NS_BEGIN(TA_Base_App) 

class CCFDServerUtilityFun;

class CCppSqliteDbOper : public IDbOper
{
public:
	CCppSqliteDbOper(const CDBOperParam&  dbOperParam);
	~CCppSqliteDbOper(void);
public:
	int checkAndInitDBTable(std::list<int>& lstTableKey);
	int transactionInsertDataBar(unsigned int nTableKey, LstInstrumentBarInfoT* pListBar);
	int transactionInsertDataTick(LstInstrumentTickInfoT* pListTick);


private:
	void _InitDataBase();
	void _UnInitDataBase();
	int _CommitTransaction();
	int _StartTransaction();
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
	CppSQLite3DB*		m_pQSqlDataBase;
	bool                m_bStartTransaction;


private:
	CDBOperParam*  m_pDBOperParam;
	CCFDServerUtilityFun*  m_pUtilityFun;
};

NS_END(TA_Base_App) 


#endif //__CLASS_CPP_SQLITE_DB_OPER__HH__


