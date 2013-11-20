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
#include "core/ocl/src/ocl.h"

#include "Logger.h"



//////////////////////////////////////////////////////////////////////////
using namespace ocl;



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

	pThis->fun_testclass_fun_two();

	return NULL;

}




//////////////////////////////////////////////////////////////////////////





/************************************************************************/
/*                                                                      */
/************************************************************************/


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CTestClass::fillDataBuffer(ocl::OraConnection *pconnection, ocl::OraQuery* pQuery, int numColumns, long lMaxRows )
{
	int nFieldNumber = 0;
	int nRecordCount = 0; 
	int nRecordIndex = 0;
	int nOCILIBRes = 0;

	nFieldNumber = pQuery->fieldCount();
	nRecordCount = pQuery->recordCount();
	_SysLog(SourceFLInfo, DebugDebug, "[nRecordCount=%d] [nFieldNumber=%d]", nRecordCount, nFieldNumber);

	try
	{
		//nRecordIndex begin with 1   nFieldIndex begin with 1
		nRecordIndex = 1;

		while( !pQuery->isEOF() && ( nRecordIndex <= lMaxRows ))
		{
			//Row dataRow;
			for ( int nFieldIndex = 0; nFieldIndex < numColumns; ++nFieldIndex )
			{
				ocl::CRString strFieldName = pQuery->field(nFieldIndex).name();
				std::string strValue = pQuery->field(nFieldIndex).getString();

				_SysLog(SourceFLInfo, DebugDebug, "[nRecordIndex=%d][nFieldIndex=%d] SQL field name:[%s]; value:[%s]", 
					nRecordIndex, nFieldIndex+1, strFieldName.c_str(), strValue.c_str());

			}
			pQuery->next();
			++nRecordIndex;
		}//while

	}
	catch( ocl::OraException & e )
	{
		_SysLog(SourceFLInfo, DebugDebug, "OCI_ERROR  [errormsg=%s] [errorcode=%d]", e.message().c_str(), e.errorCode());
	}
	catch( ... )
	{
		_SysLog(SourceFLInfo, DebugDebug, "fillDataBuffer UNKNOWN ERROR");
	}
}//CTestClass::fillDataBuffer



void CTestClass::fetchInitial(ocl::OraConnection *pconnection, ocl::OraQuery* pQuery, int numColumns, long lMaxRows )
{
	try
	{
		if ( pQuery->isRowsReturn() )
		{
			pQuery->moveTo(1);
			fillDataBuffer(pconnection, pQuery, numColumns, lMaxRows);
		}
	}
	catch( ocl::OraException & e )
	{
		_SysLog(SourceFLInfo, DebugDebug, "OCI_ERROR  [errormsg=%s] [errorcode=%d]", e.message().c_str(), e.errorCode());
	}
	catch(...)
	{
		_SysLog(SourceFLInfo, DebugDebug, "fetchInitial UNKNOWN ERROR");
	}


}//CTestClass::fetchInitial

void CTestClass::exec(ocl::OraConnection *pconnection, const std::string& strOracleSQL, bool isQuery)
{
	int prefetch = 100;
	std::string strSql = strOracleSQL;
	OraQuery* query = NULL;

	//LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL, "Begin execute the SQL statement: [[%s]]", strSql.c_str());


	try
	{
		query = new OraQuery(); // Wrap it to OCIQueryAdapter
		query->setConnection(*pconnection);
		if (isQuery) //select *
		{
			query->setCached(false);  // noncached mode
			query->setFetchRows(prefetch);
			query->setSQL(strSql);
			query->prepare(); 
			query->open();   // query result rows
		}
		else //like the insert command
		{
			//LOG ( SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug, "The SQL modification to execute is: %s", strSql.c_str());
			query->setCommandType(ocl::ctSQL);
			query->setCommandText(strSql);
			pconnection->startTransaction();  //  start the transaction must after seting the command text.
			query->execute(); 
			query->close();
			if (pconnection->inTransaction()) // returns true 
			{
				pconnection->commit();          // ends current transaction 
			}
			else
			{
				//LOG( SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugError, "[ocl-error], transaction is broken" );
			}

			delete query;
			query = NULL;
		}
	}
	catch(ocl::OraException & e)
	{
		_SysLog(SourceFLInfo, DebugDebug, "exec OCI_ERROR  [errormsg=%s] [errorcode=%d]", e.message().c_str(), e.errorCode());
		delete query; 
		query = NULL;  //it will cause cursor leak if don't delete it.
	}
	catch(ocl::CRException &e)
	{
		_SysLog(SourceFLInfo, DebugDebug, "exec OCI_ERROR  [errormsg=%s] ", e.message().c_str());

		delete query; 
		query = NULL;  //it will cause cursor leak if don't delete it.

	}
	catch(...)
	{
		_SysLog(SourceFLInfo, DebugDebug, "exec UNKNOWN ERROR");
	}	

	//isQuery == true
	//just save the pointer, here
	if ( NULL != query )
	{
		std::string strColumnName;
		int numColumns = 0;
		int nRecordCount = 0;
		std::list<std::string> lstColumns;

		strColumnName = "line_id";
		lstColumns.push_back(strColumnName);

		strColumnName = "test_type";
		lstColumns.push_back(strColumnName);

		strColumnName = "int_column";
		lstColumns.push_back(strColumnName);

		strColumnName = "str_column";
		lstColumns.push_back(strColumnName);

		strColumnName = "bool_column";
		lstColumns.push_back(strColumnName);

		strColumnName = "real_column";
		lstColumns.push_back(strColumnName);

		strColumnName = "date_column";
		lstColumns.push_back(strColumnName);

		strColumnName = "timestamp_column";
		lstColumns.push_back(strColumnName);


		numColumns = (int)lstColumns.size();
		nRecordCount = query->recordCount();

		fetchInitial(pconnection, query, numColumns, nRecordCount);

		query->close();
		query->unprepare();
		delete query;
		query = NULL;

		lstColumns.clear();

	}//if ( NULL != query )

}//CTestClass::exec


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void CTestClass::fun_testclass_fun_two()
{ 
	_SysLog(SourceFLInfo, DebugDebug, "begin fun_testclass_fun_two");

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
	ocl::OraConnection *m_connection = NULL;
	ocl::OraQuery* m_pQuery = NULL;
	std::string strOracleSQL = "select COL3 from TEST_LONG where ID=1";
	bool isQuery = true;


	OraEnvironment::setInitMode(OraEnvironment::imObject);
	OraEnvironment::setInitMode(OraEnvironment::imThreaded);
	OraEnvironment::setThreaded(true);

	//connect to server

	m_connection = new ocl::OraConnection();
	m_connection->setConnectMode(cmNormal);
	m_connection->setUsername(m_userName); 
	m_connection->setPassword(m_password); 
	m_connection->setServer(m_databaseName); 
	m_connection->open();


	if (NULL != m_connection && false == m_connection->isConnected())
	{
		//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Connection lost");

		_SysLog(SourceFLInfo, DebugDebug, "Connection lost %s", m_connectionStr.c_str());

		delete m_connection;
		m_connection = NULL;		
		return;	
	}


	//--------------------------------------------------------
	//--------------------------------------------------------
	strOracleSQL = strSql_DropTable;
	isQuery = false;
	exec(m_connection, strOracleSQL, isQuery);

	strOracleSQL = strSql_CreateTable;
	isQuery = false;
	exec(m_connection, strOracleSQL, isQuery);
	//--------------------------------------------------------
	//--------------------------------------------------------

	//Test_Type_NormalValue
	strOracleSQL = strSql_Test_Type_NormalValue_insert;
	isQuery = false;
	exec(m_connection, strOracleSQL, isQuery);
	strOracleSQL = strSql_Test_Type_NormalValue_select;
	isQuery = true;
	exec(m_connection, strOracleSQL, isQuery);

	//test_Type_MAX_FLOAT
	strOracleSQL = strSql_test_Type_MAX_FLOAT_insert;
	isQuery = false;
	exec(m_connection, strOracleSQL, isQuery);
	strOracleSQL = strSql_test_Type_MAX_FLOAT_select;
	isQuery = true;
	exec(m_connection, strOracleSQL, isQuery);

	//-----------------------------------------------

	//-----------------------------------------------

	//--------------------------------------------------------
	//--------------------------------------------------------
	strOracleSQL = strSql_DropTable;
	isQuery = false;
	//exec(m_connection, strOracleSQL, isQuery);
	//--------------------------------------------------------
	//--------------------------------------------------------

	if (NULL != m_connection)
	{
		m_connection->close();
		delete m_connection;
		m_connection = NULL;		
	}


	_SysLog(SourceFLInfo, DebugDebug, "end fun_testclass_fun_two");

}//save_fun_testclass_fun_bigint_ocl_Test_2
//==================== add test case for bigint ocl end




























