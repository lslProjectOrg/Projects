#ifndef __CLASS_MYSQL_DB_OPER__HH__
#define __CLASS_MYSQL_DB_OPER__HH__

#include "ProjectCommonData.h"

#include "IDbOper.h"


#include <QtSql/QtSql>
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>


#include "DBOperParam.h"

NS_BEGIN(TA_Base_App) 

class CProjectUtilityFun;

class CMysqlDbOper : public IDbOper
{
public:
	CMysqlDbOper(const CDBOperParam&  dbOperParam);
	~CMysqlDbOper(void);
public:
	int checkAndInitDBTable(std::list<int>& lstTableKey);


private:
	void _InitDataBase();
	void _UnInitDataBase();
	int _CommitTransaction();
	int _StartTransaction();
	int _UseTransactionForBatchMode();
private:
	std::string _GetTableNameBar(unsigned int nTableKey);
	std::string _BuildSQLForCreateDBTableBar(unsigned int nTableKey);
	int _CreateDBTableBar(unsigned int nTableKey);
	std::string _BuildSQLForInsertBarData(unsigned int nTableKey);
	std::string _BuildSQLForCreateDBTableTick();
	int _CreateDBTableTick();
	std::string _BuildSQLForInsertTickData();
private:
	QSqlDatabase*		m_pQSqlDataBase;
	bool                m_bStartTransaction;


private:
	CDBOperParam*  m_pDBOperParam;
	CProjectUtilityFun*  m_pUtilityFun;
};

NS_END(TA_Base_App) 


#endif //__CLASS_MYSQL_DB_OPER__HH__


