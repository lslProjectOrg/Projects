#include "DataAccessTestWorker.h"

#include "core/threads/src/BoostThread.h"
#include "core/CppSQLiteLib/src/CppSQLite3DB.h"
#include "core/DataAccess/src/SQLCode.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_Test) 




//////////////////////////////////////////////////////////////////////////
CDataAccesssTestWorker::CDataAccesssTestWorker(void)
{	
	BOOST_LOG_FUNCTION();	
	m_toTerminate = false;
	m_nThreadJobState = JobState_Begin;

}

CDataAccesssTestWorker::~CDataAccesssTestWorker(void)
{
	BOOST_LOG_FUNCTION();

}


void CDataAccesssTestWorker::run()
{
	BOOST_LOG_FUNCTION();

	m_nThreadJobState = JobState_Begin;


	while (false == m_toTerminate)
	{
		_ThreadJob();
		TA_Base_Core::CBoostThread::sleep(1000);
	}

	_ProcessUserTerminate();
}

void CDataAccesssTestWorker::terminate()
{
	BOOST_LOG_FUNCTION();

	m_toTerminate = true;
}


int CDataAccesssTestWorker::_ProcessUserTerminate()
{
	BOOST_LOG_FUNCTION();

	int nFunRes = 0;
	m_toTerminate = true;
	m_nThreadJobState = JobState_End;
	return nFunRes;
}

bool CDataAccesssTestWorker::isFinishWork()
{
	BOOST_LOG_FUNCTION();

	bool bFinishWork = false;
	if (JobState_End == m_nThreadJobState)
	{
		bFinishWork = true;
	}
	return bFinishWork;

}

void CDataAccesssTestWorker::_ThreadJob()
{
	BOOST_LOG_FUNCTION();

	switch (m_nThreadJobState)
	{
	case JobState_Begin:
		m_nThreadJobState = JobState_DO_LogTest;
		break;
	case JobState_DO_LogTest:
		_Process_JobState_DO_LogTest();
		m_nThreadJobState = JobState_DO_ThreadTest;
		break;
	case JobState_DO_ThreadTest:
		_Process_JobState_DO_ThreadTest();
		m_nThreadJobState = JobState_DO_SQliteTest;
		break;
	case JobState_DO_SQliteTest:
		_Process_JobState_DO_SQliteTest();
		m_nThreadJobState = JobState_DO_DataAccessTest;
		break;
	case JobState_DO_DataAccessTest:
		_Process_JobState_DO_DataAccessTest();
		m_nThreadJobState = JobState_End;
		break;
	case JobState_End:
		break;
	default:
		TA_Base_Core::CBoostThread::sleep(1000);
		break;

	}//switch
}


void CDataAccesssTestWorker::_Process_JobState_DO_LogTest()
{
	BOOST_LOG_FUNCTION();
	TA_Base_Core::CBoostLogger::getInstance().testBoostLog();


}


void CDataAccesssTestWorker::_Process_JobState_DO_ThreadTest()
{
	BOOST_LOG_FUNCTION();
}


void CDataAccesssTestWorker::_Process_JobState_DO_SQliteTest()
{
	BOOST_LOG_FUNCTION();
	std::string strSQLiteDBFile = "SQLiteDBFileTest.db";
	TA_Base_Core::CppSQLite3DB db;
	int nAffectedRows = 0;
	std::string strSQL_Create_Table = "create table emp(empno int, empname char(20))";
	std::string strSQL_Insert_Data = "insert into emp values (7, 'David Beckham')";
	std::string strSQL_Update_Data = "update emp set empname = 'Christiano Ronaldo' where empno = 7";
	std::string strSQL_Delete_Data = "delete from emp where empno = 7";
	



	LOG_DEBUG<<"SQLite Version: "<<db.SQLiteVersion();
	remove(strSQLiteDBFile.c_str());
	db.open(strSQLiteDBFile.c_str());


	LOG_DEBUG<<"Creating emp table";
	db.execDML(strSQL_Create_Table.c_str());
	LOG_DEBUG<<"DML tests";
	nAffectedRows = db.execDML(strSQL_Insert_Data.c_str());
	LOG_DEBUG<<"insert nAffectedRows="<<nAffectedRows;
	nAffectedRows = db.execDML(strSQL_Update_Data.c_str());
	LOG_DEBUG<<"update nAffectedRows="<<nAffectedRows;
	nAffectedRows = db.execDML(strSQL_Delete_Data.c_str());
	LOG_DEBUG<<"delete nAffectedRows="<<nAffectedRows;

	db.close();


}


void CDataAccesssTestWorker::_Process_JobState_DO_DataAccessTest()
{
	BOOST_LOG_FUNCTION();
	TA_Base_Core::CppSQLite3DB db;
	int nAffectedRows = 0;
	std::string strSQLiteDBFile = "SQLiteDBFileTest.db";
	TA_Base_Core::CppSQLite3Query queryGetTime;
	std::string strSysDataNow = "";
	TA_Base_Core::SQLStatement stSqlStatement; 
	TA_Base_Core::SQLCode::getInstance();

	TA_Base_Core::SQLCode::getInstance().prepareSQLStatement(
		stSqlStatement, TABLETESTDAITEST_CREATE_6000);

		LOG_DEBUG<<"SQLite Version: "<<db.SQLiteVersion();
	remove(strSQLiteDBFile.c_str());
	db.open(strSQLiteDBFile.c_str());

	

	LOG_DEBUG<<"Creating table  Dai Test ";
	db.execDML(stSqlStatement.strSQLiteSQL.c_str());
	LOG_DEBUG<<"DML tests";

	//SELECT STRFTIME('%%Y-%%m-%%d %%H:%%M:%%S', 'now', 'localtime') AS SYSDATENOW
	TA_Base_Core::SQLCode::getInstance().prepareSQLStatement(
		stSqlStatement, SYS_SYSDATE_NOW_SELECT_1000);
	queryGetTime = db.execQuery(stSqlStatement.strSQLiteSQL.c_str());
	strSysDataNow = queryGetTime.getStringField("SYSDATENOW");
	queryGetTime.finalize();
	

	TA_Base_Core::SQLCode::getInstance().prepareSQLStatement(
		stSqlStatement, TABLETESTDAITEST_INSERT_2000, strSysDataNow, strSysDataNow);

	nAffectedRows = db.execDML(stSqlStatement.strSQLiteSQL.c_str());
	LOG_DEBUG<<"insert nAffectedRows="<<nAffectedRows;

	TA_Base_Core::SQLCode::removeInstance();

}

NS_END(TA_Base_Test) 



