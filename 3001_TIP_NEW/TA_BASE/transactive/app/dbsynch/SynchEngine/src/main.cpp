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
	//test_sqlitedb();
	test_threadpool();

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



	rSqlObj.clear();
	pDatabase->prepareSQLStatement(rSqlObj, TABLETESTDAITEST_INSERT_2000);
	pDatabase->executeModification(rSqlObj);
	rSqlObj.clear();


	std::vector<std::string> columnNames;
	columnNames.push_back("STR_COLUMN");

	//	/*CommonSQL*/("SELECT STR_COLUMN FROM TABLE_TEST_DAITEST WHERE LINE_ID = 1000"),
	rSqlObj.clear();
	pDatabase->prepareSQLStatement(rSqlObj, TABLETESTDAITEST_SELECT_1000);
	pData = pDatabase->executeQuery(rSqlObj, columnNames);
	rSqlObj.clear();
	strValue = pData->getStringData(0, "STR_COLUMN");
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













