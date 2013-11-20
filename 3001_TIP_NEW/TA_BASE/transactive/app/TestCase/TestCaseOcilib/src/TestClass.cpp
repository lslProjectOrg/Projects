#pragma warning(disable:4786)

#include "TestClass.h"

//////////////////////////////////////////////////////////////////////////
//system
#include <iostream>
#include <sstream>
#include <time.h>
#include <vector>	
#include <list>  
#include <time.h>
#include <sys/timeb.h>
#include <limits.h>
#include <float.h>

#include "pthread.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//TestClass
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ocilib.h"




#include "Logger.h"


CTestClass::CTestClass ( const std::string & strTestClassName )
{

}
CTestClass::~CTestClass()
{

}

/***********************************************************
* Test: invoke the test thread methods and do the multi-thread
*      
* Expected: all the test operations can work well
***********************************************************/
void CTestClass::RunTestCase()
{
#ifdef WIN32   
	//HANDLE hThread[5] = {NULL};
	//hThread[0] = (HANDLE)_beginthread(runThread, 0, this);
	//::WaitForMultipleObjects(1, hThread, true, INFINITE);
#else
#endif	

	int status = 0;
	pthread_t thread_1;


	status = pthread_create(&thread_1, NULL, runThread, this); 

	status = pthread_join(thread_1, NULL);


	status = 0;


}

//////////////////////////////////////////////////////////////////////////

threadrounte CTestClass::runThread(void* pParam)
{	
	CTestClass* pThis = (CTestClass*)pParam;

	pThis->fun_testclass_fun_OCILIB_ConPool();

	return NULL;

}




//////////////////////////////////////////////////////////////////////////


void _GetALLData(OCI_Connection* pConnection, OCI_Statement* pStatement)
{
	OCI_Connection* m_connection = pConnection;
	OCI_Statement* m_statement = pStatement;
	OCI_Resultset* m_resultset = NULL;
	int nFieldNumber = 0;
	int nRecordCount = 0; 
	int nRecordIndex = 0;
	int nOCILIBRes = 0;


	m_resultset = OCI_GetResultset(m_statement);
	//get data

	nOCILIBRes = OCI_FetchNext(m_resultset);

	nFieldNumber = OCI_GetColumnCount(m_resultset);
	nRecordCount = OCI_GetRowCount(m_resultset);
	_SysLog(SourceFLInfo, DebugDebug, "[nRecordCount=%d] [nFieldNumber=%d]", nRecordCount, nFieldNumber);

	//nRecordIndex begin with 1   nFieldIndex begin with 1
	nRecordIndex = 1;
	while (1 == nOCILIBRes)
	{
		//nRecordIndex begin with 1   nFieldIndex begin with 1

		for ( int nFieldIndex = 1; nFieldIndex <= nFieldNumber; ++nFieldIndex )
		{
			OCI_Column *col = OCI_GetColumn(m_resultset, nFieldIndex);
			std::string strFieldName = OCI_GetColumnName(col);
			const char *pStrValue =   OCI_GetString(m_resultset, nFieldIndex);
			_SysLog(SourceFLInfo, DebugDebug, "[nRecordIndex=%d][nFieldIndex=%d] SQL field name:[%s]; value:[%s]", 
				nRecordIndex, nFieldIndex, strFieldName.c_str(), pStrValue);
		}

		nOCILIBRes = OCI_FetchNext(m_resultset);
		nRecordIndex++;

	}//while

	m_resultset = NULL;
	nFieldNumber = 0;
}




//////////////////////////////////////////////////////////////////////////



/************************************************************************/
/*                                                                      */
/************************************************************************/

void CTestClass::fun_testclass_fun_one()
{ 
	const std::string m_connectionStr = "Oracle,TRANSACT,IT271350_5,IT271350_5,192.168.123.43";
	const std::string m_databaseName = "TRANSACT";
	const std::string m_userName = "IT271350_5";
	const std::string m_password = "IT271350_5";
	const std::string m_hostName = "192.168.123.43";
	int prefetch = 100;
	std::string strSql = "SELECT * FROM recipient_info";
	bool isQuery = true;
	int nFieldNumber = 0;
	int nRecordCount = 0; 
	
	OCI_Connection* m_connection = NULL;
	OCI_Statement* m_statement = NULL;
	OCI_Resultset* m_resultset = NULL;

	OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT);

	//connect to server

	m_connection = OCI_ConnectionCreate(m_databaseName.c_str(), m_userName.c_str(), m_password.c_str(), OCI_SESSION_DEFAULT);
	m_statement = OCI_StatementCreate(m_connection);


	//exec sql
	OCI_ExecuteStmt(m_statement, strSql.c_str());

	m_resultset = OCI_GetResultset(m_statement);

	//get data
	nFieldNumber = OCI_GetColumnCount(m_resultset);

	while (OCI_FetchNext(m_resultset))
	{
		printf("%s - %s\n", OCI_GetString(m_resultset, 1), OCI_GetString(m_resultset,2));
		for ( int nColLoop = 1; nColLoop <= nFieldNumber; ++nColLoop )
		{
			OCI_Column *col = OCI_GetColumn(m_resultset, nColLoop);
			std::string strFieldName = OCI_GetColumnName(col);
			const char *pStrValue =   OCI_GetString(m_resultset, nColLoop);
			printf("SQL field name:[%s]; value:[%s]\n", strFieldName.c_str(), pStrValue);
			//std::string strValue = OCI_GetString(m_resultset, nColLoop);
			
			//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "SQL field name:[%s]; value:[%s]", strFieldName.c_str(), pStrValue);

		}

	}

	//clean
	OCI_StatementFree(m_statement);
	OCI_ConnectionFree(m_connection);
	OCI_Cleanup();	

	
}



void CTestClass::fun_testclass_fun_two()
{ 
	_SysLog(SourceFLInfo, DebugDebug, "begin  fun_testclass_fun_two() ");


	std::string strSql_DropTable = "drop table table_test_daitest";
	//std::string strSql_CreateTable = "CREATE TABLE table_test_daitest(line_id NUMBER, test_type VARCHAR2(200), int_column NUMBER, str_column VARCHAR2(200), bool_column NUMBER, real_column REAL, date_column DATE, timestamp_column TIMESTAMP)";
	std::string strSql_CreateTable = "CREATE TABLE table_test_daitest(line_id NUMBER, test_type VARCHAR2(200), int_column NUMBER, str_column VARCHAR2(200), bool_column NUMBER, real_column NUMBER(38,8), date_column DATE, timestamp_column TIMESTAMP)";
	//Test_Type_NormalValue
	std::string strSql_Test_Type_NormalValue_insert =	"insert into table_test_daitest(line_id, test_type, int_column, str_column, bool_column, real_column, date_column, timestamp_column) values(1070, 'Test_Type_NormalValue',1000000, 'str_normal_value', 1, 10000000.10001, sysdate, sysdate)";
	std::string strSql_Test_Type_NormalValue_select =	"select line_id, test_type, int_column, str_column, bool_column, real_column, TO_CHAR(date_column,'YYYYMMDDHH24MISS'), TO_CHAR(timestamp_column,'YYYYMMDDHH24MISSFF3') from table_test_daitest where line_id = 1070";
	//test_Type_MAX_FLOAT
	//std::string strSql_test_Type_MAX_FLOAT_insert =	"insert into table_test_daitest(line_id, test_type, int_column, str_column, bool_column, real_column, date_column, timestamp_column) values(1080, 'test_Type_MAX_FLOAT', 2147483647, '01234567890123456789', 1, 3.402823e+038, sysdate, sysdate)";
	//std::string strSql_test_Type_MAX_FLOAT_insert =	"insert into table_test_daitest(line_id, test_type, int_column, str_column, bool_column, real_column, date_column, timestamp_column) values(1080, 'test_Type_MAX_FLOAT', 2147483647, '01234567890123456789', 1, 3.402823e+028, sysdate, sysdate)";
	std::string strSql_test_Type_MAX_FLOAT_insert =	"insert into table_test_daitest(line_id, test_type, int_column, str_column, bool_column, real_column, date_column, timestamp_column) values(1080, 'test_Type_MAX_FLOAT', 42147483647, '01234567890123456789', 1, 3.402823e+028, sysdate, sysdate)";
	std::string strSql_test_Type_MAX_FLOAT_select =	"select line_id, test_type, int_column, str_column, bool_column, real_column, TO_CHAR(date_column,'YYYYMMDDHH24MISS'), TO_CHAR(timestamp_column,'YYYYMMDDHH24MISSFF3') from table_test_daitest where line_id = 1080";


	std::string m_connectionStr = "Oracle,TRANSACT,IT271350_5,IT271350_5,192.168.123.43";
	std::string m_databaseName = "TRANSACT";
	std::string m_userName = "IT271350_5";
	std::string m_password = "IT271350_5";
	std::string m_hostName = "192.168.123.43";
	std::string strOracleSQL = "SELECT * FROM recipient_info";
	bool isQuery = true;
	int nFieldNumber = 0;
	int nRecordCount = 0; 
	int nOCILibRes = 0;

	OCI_Connection* m_connection = NULL;
	OCI_Statement* m_statement = NULL;
	OCI_Resultset* m_resultset = NULL;

	nOCILibRes = OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT);

	//connect to server
	m_connection = OCI_ConnectionCreate(m_databaseName.c_str(), m_userName.c_str(), m_password.c_str(), OCI_SESSION_DEFAULT);
	m_statement = OCI_StatementCreate(m_connection);




	//--------------------------------------------------------
	//--------------------------------------------------------
	//exec sql
	strOracleSQL = strSql_DropTable;
	nOCILibRes = OCI_ExecuteStmt(m_statement, strOracleSQL.c_str());

	strOracleSQL = strSql_CreateTable;
	nOCILibRes = OCI_ExecuteStmt(m_statement, strOracleSQL.c_str());


	//Test_Type_NormalValue
	strOracleSQL = strSql_Test_Type_NormalValue_insert;
	nOCILibRes = OCI_ExecuteStmt(m_statement, strOracleSQL.c_str());

	strOracleSQL = strSql_Test_Type_NormalValue_select;
	nOCILibRes = OCI_ExecuteStmt(m_statement, strOracleSQL.c_str());
	_GetALLData(m_connection, m_statement);

	//test_Type_MAX_FLOAT
	strOracleSQL = strSql_test_Type_MAX_FLOAT_insert;
	nOCILibRes = OCI_ExecuteStmt(m_statement, strOracleSQL.c_str());

	strOracleSQL = strSql_test_Type_MAX_FLOAT_select;
	nOCILibRes = OCI_ExecuteStmt(m_statement, strOracleSQL.c_str());
	_GetALLData(m_connection, m_statement);

	strOracleSQL = strSql_DropTable;
	isQuery = false;
	//	nOCILibRes = OCI_ExecuteStmt(m_statement, strOracleSQL.c_str());

	//--------------------------------------------------------
	//--------------------------------------------------------

	//clean
	nOCILibRes = OCI_StatementFree(m_statement);
	nOCILibRes = OCI_ConnectionFree(m_connection);
	nOCILibRes = OCI_Cleanup();


	_SysLog(SourceFLInfo, DebugDebug, "end  fun_testclass_fun_two() ");

}//save_fun_testclass_fun_bigint_ocl_Test_2




#define MAX_THREADS 2

void CTestClass::worker(OCI_Thread *thread, void *data)
{
	std::string m_connectionStr = "Oracle,TRANSACT,IT271350_5,IT271350_5,192.168.123.43";
	std::string m_databaseName = "TRANSACT";
	std::string m_userName = "IT271350_5";
	std::string m_password = "IT271350_5";
	std::string m_hostName = "192.168.123.43";
	int prefetch = 100;
	std::string strSql = "SELECT * FROM recipient_info";
	bool isQuery = true;
	int nFieldNumber = 0;
	int nRecordCount = 0; 

	OCI_Connection* m_connection = NULL;
	OCI_Statement* m_statement = NULL;
	OCI_Resultset* m_resultset = NULL;


	//connect to server
	m_connection = OCI_PoolGetConnection((OCI_Pool *)data, NULL);

	m_statement = OCI_StatementCreate(m_connection);


	//exec sql
	OCI_ExecuteStmt(m_statement, strSql.c_str());

	m_resultset = OCI_GetResultset(m_statement);

	//OCI_ConnectionFree(m_connection);

	//get data
	nFieldNumber = OCI_GetColumnCount(m_resultset);

	while (OCI_FetchNext(m_resultset))
	{
		printf("%s - %s\n", OCI_GetString(m_resultset, 1), OCI_GetString(m_resultset,2));
		for ( int nColLoop = 1; nColLoop <= nFieldNumber; ++nColLoop )
		{
			OCI_Column *col = OCI_GetColumn(m_resultset, nColLoop);
			std::string strFieldName = OCI_GetColumnName(col);
			const char *pStrValue =   OCI_GetString(m_resultset, nColLoop);
			printf("SQL field name:[%s]; value:[%s]\n", strFieldName.c_str(), pStrValue);
		}
	}//while

	//clean
	m_resultset = NULL;
	OCI_StatementFree(m_statement);
	m_statement = NULL;
	OCI_ConnectionFree(m_connection);
	//m_connection = NULL;


	OCI_Connection* m_connection2 = NULL;
	m_connection2 = OCI_PoolGetConnection((OCI_Pool *)data, NULL);
	if (m_connection2 == m_connection)
	{
		int a = 0;
	}

	
}

void CTestClass::fun_testclass_fun_OCILIB_ConPool()
{
	const std::string m_connectionStr = "Oracle,TRANSACT,IT271350_5,IT271350_5,192.168.123.43";
	const std::string m_databaseName = "TRANSACT";
	const std::string m_userName = "IT271350_5";
	const std::string m_password = "IT271350_5";
	const std::string m_hostName = "192.168.123.43";
	int prefetch = 100;
	std::string strSql = "SELECT * FROM recipient_info";
	bool isQuery = true;
	int nFieldNumber = 0;
	int nRecordCount = 0; 

	OCI_Connection* m_connection = NULL;
	OCI_Statement* m_statement = NULL;
	OCI_Resultset* m_resultset = NULL;

	OCI_Thread *th[MAX_THREADS];
	OCI_ConnPool *pool;
	int n_MAX_CONN = 10;


	int nIndexTh;

	if (!OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT | OCI_ENV_THREADED))
	{
		printf("OCI_Initialize(OCI_ENV_DEFAULT | OCI_ENV_THREADED)error!\n");
		return;
	}
		

	/* create pool */

	pool = OCI_PoolCreate(m_databaseName.c_str(), m_userName.c_str(), m_password.c_str(), 
		OCI_POOL_CONNECTION, OCI_SESSION_DEFAULT, 
		0, n_MAX_CONN, 1);

	/* create threads */

	for (nIndexTh = 0; nIndexTh < MAX_THREADS; nIndexTh++)
	{
		th[nIndexTh] = OCI_ThreadCreate();
		OCI_ThreadRun(th[nIndexTh], worker, pool);
	}

	/* wait for threads and cleanup */

	for (nIndexTh = 0; nIndexTh < MAX_THREADS; nIndexTh++)
	{
		OCI_ThreadJoin(th[nIndexTh]);
		OCI_ThreadFree(th[nIndexTh]);
	}

	OCI_PoolFree(pool);

	OCI_Cleanup();


	return;
}




























