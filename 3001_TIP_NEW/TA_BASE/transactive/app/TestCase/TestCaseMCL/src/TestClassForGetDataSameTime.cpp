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
		m_pDriver = NULL;
		m_pConnection = NULL;
		m_nThread1Thread1Conditon = 0;

		m_connectionStr = "Mysql:tra_occ:IT271350_5:IT271350_5:192.168.123.43";
		m_databaseName = "tra_occ";
		m_userName = "IT271350_5";
		m_password = "IT271350_5";
		m_hostName  = "192.168.123.43";


		if (NULL == m_pDriver)
		{
			m_pDriver = (sql::Driver*)get_driver_instance();  // for mysql db connection
		}
		sql::ConnectOptionsMap connection_properties;
		connection_properties["hostName"] = m_hostName;
		connection_properties["userName"] = m_userName;
		connection_properties["password"] = m_password;
		connection_properties["metadataUseInfoSchema"] = true;
		TA_ASSERT(NULL != m_pDriver, "Mysql database Driver is null");
		m_pConnection = m_pDriver->connect(connection_properties); 
		TA_ASSERT(NULL != m_pConnection, "Cannot connect to the database server.");	
		m_pConnection->setSchema(m_databaseName);
	}
	catch (...)
	{
		LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "_InitTestData(): Unknown exception" );
	}


	if ( m_pConnection && !m_pConnection->isClosed())
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
	strSQL = "CREATE TABLE TABLE_TEST_DAITEST(LINE_ID int, TEST_TYPE VARCHAR(200), INT_COLUMN int, STR_COLUMN VARCHAR(200), BOOL_COLUMN int, REAL_COLUMN double, DATE_COLUMN DATETIME, TIMESTAMP_COLUMN TIMESTAMP)";
	isQuery = false;
	exec(m_pConnection, strSQL, isQuery, NULL);
	strSQL.clear();

	for (int nIndex = 0; nIndex < 2000; nIndex++)
	{
		strSQL.clear();
		std::string strTime = "2013-11-19 16:53:20";

		strSQL = "INSERT INTO TABLE_TEST_DAITEST(LINE_ID, TEST_TYPE, INT_COLUMN, STR_COLUMN, BOOL_COLUMN, REAL_COLUMN, DATE_COLUMN, TIMESTAMP_COLUMN) VALUES (1000, 'Test_Type_NormalValue', 1000000, 'str_normal_value', 1, 10000000.10001, '2013-11-19 16:53:20', '2013-11-19 16:53:20')";
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
	sql::ResultSet* pQueryGet_1 = NULL;

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
				fetchInitial(m_pConnection, pQueryGet_1, 1, 2000);
				break;
			}
		}//if (bGetData)
		TA_Base_Core::Thread::sleep(100);
	}//while (true)


	if (NULL != pQueryGet_1)
	{
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

void CTestClassForGetDataSameTime::fillDataBuffer(sql::Connection *pconnection, sql::ResultSet* pQuery, int numColumns, long lMaxRows )
{
	if ( NULL == pQuery )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Query is closed by timeout" );
		return;
	}

	sql::ResultSetMetaData* pRsMetaData = pQuery->getMetaData() ; // use of auto_prt, need not delete
	TA_ASSERT(NULL != pRsMetaData, "get result set MetaData pointer failed");
	int nFieldNumber = pRsMetaData->getColumnCount();
	TA_ASSERT(numColumns <= nFieldNumber, "column number is bigger than the SQL returns");

	try
	{
		long lOffset = 0;
		while( !pQuery->isAfterLast() && ( lOffset < lMaxRows ))
		{
			for ( int nColLoop = 0; nColLoop < nFieldNumber; ++nColLoop )
			{
				std::string strFieldName = pRsMetaData->getColumnName(nColLoop + 1);  // 1-based 
				std::string strValue = pQuery->getString(nColLoop + 1);
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, 
					"SQL field name:[%s]; value:[%s]", strFieldName.c_str(), strValue.c_str());
			}
			
			++lOffset;
			pQuery->next();
		}
	}
	catch( ... )
	{		
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Unknown exception, Unknown reason");
	}
}//CTestClass::fillDataBuffer



void CTestClassForGetDataSameTime::fetchInitial(sql::Connection *pconnection, sql::ResultSet* pQuery, int numColumns, long lMaxRows )
{
	if ( NULL == pQuery )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Query is closed by timeout" );
		return;
	}

	try
	{
		if ( pQuery->rowsCount() > 0 )
		{
			pQuery->first();
			fillDataBuffer(pconnection, pQuery, numColumns, lMaxRows);
		}
	}	
	catch (...)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "fetchInitial Unknown exception, Unknown reason");
	}

}//CTestClass::fetchInitial


void CTestClassForGetDataSameTime::exec(sql::Connection* pConnection, const std::string& strSql, bool isQuery, sql::ResultSet** ppQueryGet)
{
	if (NULL == m_pConnection)
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Mysql connection is null" );
		return;
	}
	LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL, "Begin execute the SQL statement: [[%s]]", strSql.c_str());

	sql::ResultSet* pQuery = NULL; 
	try
	{
		TA_THREADGUARD(m_lockConnection);
		// check the database connection first, if it already closed, open it
		if (m_pConnection->isClosed())
		{
			return;
		}		
		std::auto_ptr< sql::Statement > stmt(m_pConnection->createStatement());
		if (isQuery) //select *
		{
			pQuery = stmt->executeQuery(strSql);
		}
		else //like the insert command
		{
			LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "The SQL modification to execute is: %s", strSql.c_str());
			bool bRetCode = stmt->execute(strSql);
			if(bRetCode)
			{
				LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "the statement is not a insert like statement" );
			}	
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


























