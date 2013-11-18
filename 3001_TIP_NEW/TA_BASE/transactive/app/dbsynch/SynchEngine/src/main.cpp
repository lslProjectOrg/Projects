// SynchEnginer.cpp : Defines the entry point for the console application.
//
#include "vld.h"
#include <signal.h>
//
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"

//
#include "core/database/src/IDatabase.h"
#include "core/database/src/DatabaseFactory.h"
//#include "core/exceptions/src/DbSyncException.h"
#include "core/database/src/sqlmacrodef.h"

//
#include "core/threads/src/LFThreadPoolManager.h"
#include "ClassThreadPoolExecuteItemImp.h"
#include "core/threads/src/Thread.h"
//



bool g_b_main_continue = true;
void usr_signal(int SigNo)
{
	if (SigNo == SIGINT)
	{
		g_b_main_continue = false;
	}
	
#ifndef WIN32
	if (SigNo == SIGHUP)
	{
	}
#endif


}


//////////////////////////////////////////////////////////////////////////

void test_sqlitedb();

void test_threadpool();




//////////////////////////////////////////////////////////////////////////


int main( int argc, char* argv[] )
{ 	   
	signal(SIGINT, usr_signal);
#ifndef WIN32
	signal(SIGHUP, usr_signal);	//close putty
#endif
	bool bSet = TA_Base_Core::RunParams::getInstance().parseCmdLine(argc, argv);


	//
	test_sqlitedb();
	//test_threadpool();

	//
	
	return 0;
}




void test_sqlitedb()
{ 	   

	TA_Base_Core::DebugUtil::getInstance();

	TA_Base_Core::RunParams::getInstance().set(RPARAM_DBCONNECTIONFILE,"./OCConnectionStrings.csv");
	TA_Base_Core::DebugUtil::getInstance().setFile("log_dbSynch.log");
	TA_Base_Core::DebugUtil::getInstance().setMaxSize(2000000);
	TA_Base_Core::DebugUtil::getInstance().setLevel(TA_Base_Core::DebugUtil::DebugSQL);	

	//Event_Ad,Read,Sqlite,tableTest,noUser,noPWD,127.0.0.1,Mysql,tra_occ,IT271350_5,IT271350_5,192.168.123.43,
	//Oracle,TRANSACT,IT271350_5,IT271350_5,192.168.123.43
	//
	TA_Base_Core::RunParams::getInstance().set("DataBase_tableTest.db@127.0.0.1",RPARAM_DBONLINE); 	 
	//TA_Base_Core::RunParams::getInstance().set("DataBase_tra_occ@192.168.123.43",RPARAM_DBOFFLINE); 	 
	//TA_Base_Core::RunParams::getInstance().set("DataBase_TRANSACT@192.168.123.43",RPARAM_DBONLINE); 	 

	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "test_sqlitedb");



	TA_Base_Core::EDataTypes TestEDataType = TA_Base_Core::Event_Ad;
	TA_Base_Core::EDataActions TestEDataAction = TA_Base_Core::Read;
	TA_Base_Core::IDatabase* pDatabase = NULL;
	TA_Base_Core::IData* pData = NULL;
	unsigned long ulongPkeyValue = 0;
	std::string strValue = "";
	std::string strSYSDATENOW = "";
	std::vector<std::string> columnNames;


	pDatabase = TA_Base_Core::DatabaseFactory::getInstance().getDatabase(TestEDataType, TestEDataAction);

	TA_Base_Core::SQLStatement rSqlObj;
	rSqlObj.clear();
	pDatabase->prepareSQLStatement(rSqlObj, TABLETESTDAITEST_DROP_5000);
	try
	{
		pDatabase->executeModification(rSqlObj);
	}
	catch (...)
	{
		//not this table
	}
	rSqlObj.clear();


	rSqlObj.clear();
	pDatabase->prepareSQLStatement(rSqlObj, TABLETESTDAITEST_CREATE_6000);
	pDatabase->executeModification(rSqlObj);
	rSqlObj.clear();

	//SYS_SYSDATE_NOW_SELECT_1000
	//SYS_SYSDATE_NOW_SELECT_1000=SELECT TO_CHAR(SYSDATE£¬'YYYY-MM-DD HH24:MI:SS') AS SYSDATENOW  FROM DUAL
	columnNames.clear();
	columnNames.push_back("SYSDATENOW");
	rSqlObj.clear();
	pDatabase->prepareSQLStatement(rSqlObj, SYS_SYSDATE_NOW_SELECT_1000);
	pData = pDatabase->executeQuery(rSqlObj, columnNames);
	rSqlObj.clear();
	strSYSDATENOW = pData->getStringData(0, "SYSDATENOW");
	delete pData;
	pData = NULL;

	columnNames.clear();
	columnNames.push_back("SYSDATENOW");
	rSqlObj.clear();
	pDatabase->prepareSQLStatement(rSqlObj, SYS_SYSDATE_NOW_SELECT_1001);
	pData = pDatabase->executeQuery(rSqlObj, columnNames);
	rSqlObj.clear();
	//time_t nTimeTGet = pData->getDateData(0, "SYSDATENOW");
	delete pData;
	pData = NULL;

	columnNames.clear();
	columnNames.push_back("SYSDATENOW");
	rSqlObj.clear();
	pDatabase->prepareSQLStatement(rSqlObj, SYS_SYSDATE_NOW_SELECT_1003);
	pData = pDatabase->executeQuery(rSqlObj, columnNames);
	rSqlObj.clear();
	pData->getTimestampData(0, "SYSDATENOW");
	delete pData;
	pData = NULL;


	/*
	TABLETESTDAITEST_INSERT_2000=INSERT INTO TABLE_TEST_DAITEST(LINE_ID, TEST_TYPE, INT_COLUMN, STR_COLUMN, BOOL_COLUMN, REAL_COLUMN,
	+ DATE_COLUMN, TIMESTAMP_COLUMN) VALUES (1000, 'Test_Type_NormalValue', 1000000, 'str_normal_value', 1, 10000000.10001, '%s', '%s')
	*/
	rSqlObj.clear();
	pDatabase->prepareSQLStatement(rSqlObj, TABLETESTDAITEST_INSERT_2000, strSYSDATENOW, strSYSDATENOW);
	pDatabase->executeModification(rSqlObj);
	rSqlObj.clear();



	//	/*CommonSQL*/("SELECT STR_COLUMN FROM TABLE_TEST_DAITEST WHERE LINE_ID = 1000"),
	columnNames.clear();
	columnNames.push_back("LINE_ID");
	columnNames.push_back("TEST_TYPE");
	columnNames.push_back("INT_COLUMN");
	columnNames.push_back("STR_COLUMN");
	columnNames.push_back("BOOL_COLUMN");
	columnNames.push_back("REAL_COLUMN");
	columnNames.push_back("DATE_COLUMN");
	columnNames.push_back("TIMESTAMP_COLUMN");

	rSqlObj.clear();
	pDatabase->prepareSQLStatement(rSqlObj, TABLETESTDAITEST_SELECT_1001);
	pData = pDatabase->executeQuery(rSqlObj, columnNames);
	rSqlObj.clear();
	strValue = pData->getStringData(0, "LINE_ID");
	strValue = pData->getStringData(0, "TEST_TYPE");
	strValue = pData->getStringData(0, "INT_COLUMN");
	strValue = pData->getStringData(0, "STR_COLUMN");
	strValue = pData->getStringData(0, "BOOL_COLUMN");
	strValue = pData->getStringData(0, "REAL_COLUMN");
	strValue = pData->getStringData(0, "DATE_COLUMN");
	strValue = pData->getStringData(0, "TIMESTAMP_COLUMN");
	//pData->getDateData(0, "TIMESTAMP_COLUMN");
	//pData->getTimestampData(0, "TIMESTAMP_COLUMN");
	delete pData;
	pData = NULL;


	rSqlObj.clear();
	pDatabase->prepareSQLStatement(rSqlObj, TABLETESTDAITEST_DROP_5000);
	pDatabase->executeModification(rSqlObj);
	rSqlObj.clear();
	
	TA_Base_Core::DatabaseFactory::removeInstance();

	TA_Base_Core::DebugUtil::removeInstance();
	TA_Base_Core::RunParams::removeInstance();

	
}



void test_threadpool()
{
	//
	TA_Base_Core::DebugUtil::getInstance();
	TA_Base_Core::RunParams::getInstance().set(RPARAM_DBCONNECTIONFILE,"./OCConnectionStrings.csv");
	TA_Base_Core::DebugUtil::getInstance().setFile("log_dbSynch.log");
	TA_Base_Core::DebugUtil::getInstance().setMaxSize(2000000);
	TA_Base_Core::DebugUtil::getInstance().setLevel(TA_Base_Core::DebugUtil::DebugSQL);	
	TA_Base_Core::RunParams::getInstance().set("DataBase_tableTest.db@127.0.0.1",RPARAM_DBONLINE); 	 
	//

	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "test_threadpool");

	//
	int nNumberOfThreads = 2;
	bool bIsAutoRun = false;
	int nQueueSize = 0;
	TA_Base_Core::LFThreadPoolManager* pThreadPoolManager = NULL;
	ClassThreadPoolExecuteItemImp* pWorker1 = NULL;
	ClassThreadPoolExecuteItemImp* pWorker2 = NULL;

	pThreadPoolManager = new TA_Base_Core::LFThreadPoolManager(nNumberOfThreads, bIsAutoRun);


	pThreadPoolManager->start();
	nNumberOfThreads = pThreadPoolManager->getNumberOfThreads();
	nQueueSize = pThreadPoolManager->getQueueSize();
	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "nNumberOfThreads=%d,nQueueSize=%d", nNumberOfThreads, nQueueSize);

	pWorker1 = new ClassThreadPoolExecuteItemImp();
	pWorker1->newJob();
	pThreadPoolManager->queueWorkItem(pWorker1);

	pWorker2 = new ClassThreadPoolExecuteItemImp();
	pWorker2->newJob();
	pThreadPoolManager->queueWorkItem(pWorker2);

	while (1)
	{
		if (ClassThreadPoolExecuteItemImp::JOB_STATE_DONE == pWorker1->getJobState()
			&& ClassThreadPoolExecuteItemImp::JOB_STATE_DONE == pWorker2->getJobState())
		{
			break;
		}
		TA_Base_Core::Thread::sleep(1000);
	}
	
	pThreadPoolManager->removeWorkItem(pWorker1);
	delete pWorker1;
	pWorker1 = NULL;
	pThreadPoolManager->removeWorkItem(pWorker2);
	delete pWorker2;
	pWorker2 = NULL;
	//
	if (NULL != pThreadPoolManager)
	{
		delete pThreadPoolManager;
		pThreadPoolManager = NULL;
	}

	//
	TA_Base_Core::DebugUtil::removeInstance();
	TA_Base_Core::RunParams::removeInstance();

}













