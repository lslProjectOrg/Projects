#pragma warning(disable:4786)


#include "TestClassForGetDataSameTime.h"
#include "CommonDef.h"
#include "CommonData.h"
//////////////////////////////////////////////////////////////////////////
//system
#include "pthread.h"
//////////////////////////////////////////////////////////////////////////
//TA
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/threads/src/Thread.h"
#include "core/database/src/DataTypeEnums.h"

#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "core/synchronisation/src/ThreadGuard.h"
//

//
#include "core/threads/src/Thread.h"
#include "core/ocl/src/ocl.h"

//


//////////////////////////////////////////////////////////////////////////
//project

//////////////////////////////////////////////////////////////////////////
//TestClass




//////////////////////////////////////////////////////////////////////////
using namespace TA_Base_Core;


CTestClassForGetDataSameTime::CTestClassForGetDataSameTime ( const std::string & name ) : TestCase ( name )
{

}
CTestClassForGetDataSameTime::~CTestClassForGetDataSameTime()
{
}


/////////////////////////////////////////////////////////
// Test Framework Methods
/////////////////////////////////////////////////////////
void CTestClassForGetDataSameTime::setUpSuite( const std::string & inFile )
{

}


void CTestClassForGetDataSameTime::tearDownSuite()
{

}


void CTestClassForGetDataSameTime::setUp( const std::string & INFile , const std::string & OUTFile )
{

}


void CTestClassForGetDataSameTime::tearDown( const std::string & INFile )
{
	//deleteAlarms();
}

TestSuite * CTestClassForGetDataSameTime::suite ()
{
	TestSuite* testSuite = new TestSuite( "CTestClassForGetDataSameTime" );  

	
	testSuite->addTest (
		new TestCaller <CTestClassForGetDataSameTime> (
		"RunThisTestCase", ADDR_OF(&CTestClassForGetDataSameTime::RunThisTestCase ), this ) );

	return testSuite;
}

/////////////////////////////////////////////////////////
// Test Methods implementations
/////////////////////////////////////////////////////////

/*******************************************************
* Test: insert, update, select, delete one alarm row in 
*       the database.
* Expected: all the test operations can work well
********************************************************/


/***********************************************************
* Test: invoke the test thread methods and do the multi-thread
*      
* Expected: all the test operations can work well
***********************************************************/


void CTestClassForGetDataSameTime::RunThisTestCase()
{
	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "begin CTestClassForGetDataSameTime RunThisTestCase ");
	
#ifdef WIN32   
	//HANDLE hThread[5] = {NULL};
	//hThread[0] = (HANDLE)_beginthread(runThread, 0, this);
	//::WaitForMultipleObjects(1, hThread, true, INFINITE);
#else
#endif	
	_InitTestData();

	int status = 0;
	pthread_t thread_1;
	pthread_t thread_2;
	pthread_t thread_3;
	pthread_t thread_4;
	pthread_t thread_5;


	status = pthread_create(&thread_1, NULL, runThread, this); 
	status = pthread_create(&thread_2, NULL, runThread_2, this); 
	status = pthread_create(&thread_3, NULL, runThread, this); 
	status = pthread_create(&thread_4, NULL, runThread, this); 
	status = pthread_create(&thread_5, NULL, runThread, this); 

	status = pthread_join(thread_1, NULL);
	status = pthread_join(thread_2, NULL);
	status = pthread_join(thread_3, NULL);
	status = pthread_join(thread_4, NULL);
	status = pthread_join(thread_5, NULL);
	status = 0;
	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "end CTestClassForGetDataSameTime RunThisTestCase ");

	_UnInitTestData();

}

//////////////////////////////////////////////////////////////////////////




/************************************************************************/
/*                                                                      */
/************************************************************************/


threadrounte CTestClassForGetDataSameTime::runThread(void* pParam)
{	
	CTestClassForGetDataSameTime* pThis = (CTestClassForGetDataSameTime*)pParam;

	pThis->_Test_Exec_GetData_1();
	
	return NULL;

}


threadrounte CTestClassForGetDataSameTime::runThread_2(void* pParam)
{	
	CTestClassForGetDataSameTime* pThis = (CTestClassForGetDataSameTime*)pParam;

	pThis->_Test_Exec_GetData_1();

	return NULL;

}


//////////////////////////////////////////////////////////////////////////

void CTestClassForGetDataSameTime::_InitTestData()
{
	TA_THREADGUARD(m_lockConnection);

	try
	{
		m_pConnection = NULL;
		m_nThread1Thread1Conditon = 0;

		m_connectionStr = "Oracle:TRANSACT:IT271350_5:IT271350_5:192.168.123.43";
		m_databaseName = "TRANSACT";
		m_userName = "IT271350_5";
		m_password = "IT271350_5";
		m_hostName  = "192.168.123.43";


		ocl::OraEnvironment::setInitMode(ocl::OraEnvironment::imObject);
		ocl::OraEnvironment::setInitMode(ocl::OraEnvironment::imThreaded);
		ocl::OraEnvironment::setThreaded(true);
		m_pConnection = new ocl::OraConnection();

		m_pConnection->setConnectMode(ocl::cmNormal);
		m_pConnection->setUsername(m_userName); 
		m_pConnection->setPassword(m_password); 
		m_pConnection->setServer(m_databaseName); 
		m_pConnection->open();

	}
	catch (...)
	{
		LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "_InitTestData(): Unknown exception" );
	}


	if ( m_pConnection && !m_pConnection->isConnected())
	{
		LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "m_pConnection is Closed!" );
	}

	_InitTestTable();

}

void CTestClassForGetDataSameTime::_UnInitTestData()
{
	_UnInitTestTable();
	try
	{
		//fix bug: when mysql db server offline, 
		//MysqlConnection::createConnection() will set m_pConnection=NULL, so we need to check m_pConnection's value here
		if (NULL != m_pConnection)
		{
			m_pConnection->close();
			delete m_pConnection;
			m_pConnection = NULL;
		}
	}
	catch (...)
	{
		LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "~_UnInitTestData(): Unknown exception" );
	}
}


void CTestClassForGetDataSameTime::_InitTestTable()
{
	bool isQuery = false;
	std::string strSQL;
		
	strSQL.clear();
	strSQL = "DROP TABLE TABLE_TEST_DAITEST";
	//pDatabase->prepareSQLStatement(rSqlObj, TABLETESTDAITEST_DROP_5000);
	//strSQL = rSqlObj.strCommonSQL;
	isQuery = false;
	exec(m_pConnection, strSQL, isQuery, NULL);
	strSQL.clear();


	strSQL.clear();
	//pDatabase->prepareSQLStatement(rSqlObj, TABLETESTDAITEST_CREATE_6000);
	strSQL = "CREATE TABLE TABLE_TEST_DAITEST(LINE_ID NUMBER, TEST_TYPE VARCHAR2(200), INT_COLUMN NUMBER, STR_COLUMN VARCHAR2(200), BOOL_COLUMN NUMBER, REAL_COLUMN NUMBER(20,8),  DATE_COLUMN DATE, TIMESTAMP_COLUMN TIMESTAMP)";
	isQuery = false;
	exec(m_pConnection, strSQL, isQuery, NULL);
	strSQL.clear();

	for (int nIndex = 0; nIndex < 5000; nIndex++)
	{
		strSQL.clear();
		std::string strTime = "2013-11-19 16:53:20";
		strSQL = "INSERT INTO TABLE_TEST_DAITEST(LINE_ID, TEST_TYPE, INT_COLUMN, STR_COLUMN, BOOL_COLUMN, REAL_COLUMN, DATE_COLUMN, TIMESTAMP_COLUMN) VALUES (1000, 'Test_Type_NormalValue', 1000000, 'str_normal_value', 1, 10000000.10001, sysdate, sysdate)";
				
		//pDatabase->prepareSQLStatement(rSqlObj, TABLETESTDAITEST_INSERT_2000, strTime, strTime);
		isQuery = false;
		exec(m_pConnection, strSQL, isQuery, NULL);
		strSQL.clear();
	}


}

void CTestClassForGetDataSameTime::_UnInitTestTable()
{
	bool isQuery = false;
	std::string strSQL;

	strSQL.clear();
	strSQL = "DROP TABLE TABLE_TEST_DAITEST";
	isQuery = false;
	exec(m_pConnection, strSQL, isQuery, NULL);
	strSQL.clear();
}

void CTestClassForGetDataSameTime::_Test_Exec_GetData_1()
{
	ocl::OraQuery* pQueryGet_1 = NULL;

	{
		TA_THREADGUARD(m_lockConnection);


		bool isQuery = false;
		std::string strSQL;
		strSQL.clear();
		//pDatabase->prepareSQLStatement(rSqlObj, TABLETESTDAITEST_SELECT_1001);
		strSQL = "SELECT LINE_ID, TEST_TYPE, INT_COLUMN, STR_COLUMN, BOOL_COLUMN, REAL_COLUMN, DATE_COLUMN, TIMESTAMP_COLUMN FROM TABLE_TEST_DAITEST WHERE LINE_ID = 1000";
		
		isQuery = true;
		exec(m_pConnection, strSQL, isQuery, &pQueryGet_1);
		strSQL.clear();

		m_nThread1Thread1Conditon++;
	}

	//sync
	TA_Base_Core::Thread::sleep(1000);

	//sync
	while (true)
	{
		bool bGetData = false;
		{
			TA_THREADGUARD(m_lockConnection);
			if (5 == m_nThread1Thread1Conditon)
			{
				bGetData = true;
			}
		}

		if (bGetData)
		{
			if (NULL != pQueryGet_1)
			{
				//
				std::vector<std::string> columnNames;
				_GetColumnNames(columnNames);
				TA_Base_Core::Thread::sleep(50000);

				fetchInitial(m_pConnection, pQueryGet_1, columnNames.size(),5000);
				break;
			}
		}//if (bGetData)
		TA_Base_Core::Thread::sleep(100);
	}//while (true)



	if (NULL != pQueryGet_1)
	{
		pQueryGet_1->close();
		delete pQueryGet_1;
		pQueryGet_1 = NULL;
	}

}





//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CTestClassForGetDataSameTime::_GetColumnNames(std::vector<std::string>& lstColumns)
{	
	std::string strColumnName;
	lstColumns.clear();

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
}

void CTestClassForGetDataSameTime::fillDataBuffer(ocl::OraConnection *pconnection, ocl::OraQuery* pQuery, int numColumns, long lMaxRows )
{
	if ( NULL == pQuery )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Query is closed by timeout" );
		return;
	}

	int nFieldNumber = pQuery->fieldCount();
	int nRecordCount = pQuery->recordCount() ;
	//TA_ASSERT(numColumns <= nFieldNumber, "column number is bigger than the SQL returns");

	try
	{
		long lOffset = 0;
		while( !pQuery->isEOF() && ( lOffset < lMaxRows ))
		{
			for ( int nColLoop = 0; nColLoop < nFieldNumber; ++nColLoop )
			{
				ocl::CRString strFieldName = pQuery->field(nColLoop).name();
				std::string strValue = pQuery->field(nColLoop).getString();
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
					"SQL field name:[%s]; value:[%s]", strFieldName.c_str(), strValue.c_str());
			}
			pQuery->next();
			++lOffset;
		}
	}
	catch (ocl::OraException& e)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Exception, Errorcode=%d, Reason=%s",
			e.errorCode(), e.message().c_str());
	}
	catch( ... )
	{		
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Unknown exception, Unknown reason");
	}
}//CTestClass::fillDataBuffer



void CTestClassForGetDataSameTime::fetchInitial(ocl::OraConnection *pconnection, ocl::OraQuery* pQuery, int numColumns, long lMaxRows )
{
	if ( NULL == pQuery )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Query is closed by timeout" );
		return;
	}

	try
	{
		if ( pQuery->isRowsReturn() )
		{
			pQuery->moveTo(1);
			fillDataBuffer(pconnection, pQuery, numColumns, lMaxRows);
		}
	}	
	catch (...)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "fetchInitial Unknown exception, Unknown reason");
	}

}//CTestClass::fetchInitial


void CTestClassForGetDataSameTime::exec(ocl::OraConnection* pConnection, const std::string& strSql, bool isQuery, ocl::OraQuery** ppQueryGet)
{
	if (NULL == m_pConnection)
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Oracle connection is null" );
		return;
	}
	LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL, "Begin execute the SQL statement: [[%s]]", strSql.c_str());

	ocl::OraQuery* pQuery = NULL;
	int prefetch = 5000;
	try
	{
		TA_THREADGUARD(m_lockConnection);
		// check the database connection first, if it already closed, open it
		if (!m_pConnection->isConnected())
		{
			return;
		}	

		pQuery = new ocl::OraQuery(); // Wrap it to OCIQueryAdapter
		pQuery->setConnection(*m_pConnection);
		if (isQuery) //select *
		{
			pQuery->setCached(false);  // noncached mode
			pQuery->setFetchRows(prefetch);
			pQuery->setSQL(strSql);
			pQuery->prepare(); 
			pQuery->open();   // query result rows
		}
		else //like the insert command
		{
			LOG ( SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug, "The SQL modification to execute is: %s", strSql.c_str());
			pQuery->setCommandType(ocl::ctSQL);
			pQuery->setCommandText(strSql);
			m_pConnection->startTransaction();  //  start the transaction must after seting the command text.
			pQuery->execute(); 
			pQuery->close();
			if (m_pConnection->inTransaction()) // returns true 
			{
				m_pConnection->commit();          // ends current transaction 
			}
			else
			{
				LOG( SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugError, "[ocl-error], transaction is broken" );
			}

			delete pQuery;
			pQuery = NULL;
		}
				
	}
	catch(...)
	{
		LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Failed to exec SQL: %s", strSql.c_str() );
		delete pQuery; pQuery = NULL;
	}	
	//sql::SQLString
	//just save the pointer, here
	if ( NULL != pQuery && NULL != ppQueryGet)
	{
		(*ppQueryGet) = pQuery;
		pQuery = NULL;
	}
	else
	{
		//(*ppQueryGet) = NULL;
		pQuery = NULL;
	}
	

}//CTestClass::exec


























