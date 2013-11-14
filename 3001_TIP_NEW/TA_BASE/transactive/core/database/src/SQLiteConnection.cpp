//////////////////////////////////////////////////////////////////////
///		@file		MysqlConnection.cpp
///		@author		zhilin,ouyang
///		@date		2010-11-25 15:44:41
///
///		@brief	    Implementation of the interface IDbConnection. 
///					use mysql database server to implement the   
///                 database connection operations.
//////////////////////////////////////////////////////////////////////
#if defined( WIN32 )
#pragma warning ( disable : 4786 )
#endif // defined( WIN32 )

#include "core/database/src/SQLiteConnection.h"

#include <algorithm>
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/database/src/SQLiteQueryAdapter.h"
#include "core/database/src/DBException.h"
#include "core/threads/src/Thread.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/CppSQLiteLib/src/CppSQLite3.h"


using TA_Base_Core::ThreadGuard;
using TA_Base_Core::DebugUtil;
using TA_Base_Core::Thread;

NS_BEGIN(TA_Base_Core)

const long QUERY_EXIST = 1;
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                         Construction/Destruction                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

/**
* MysqlConnection
* 
* Sole constructor that stores the database connection details and 
* allocates handles to perform transactions with the database.
* Note: this method can be unsuccessful, so checkIsValid() should be 
* called afterwards to check that construction was successful.
* 
* @param connectionStr databaseName:userName:password:hostName
* @param databaseName  the name of the database to connect to
* @param userName      the name of the user for the session
* @param password      the name of the password for the session
* @param hostName      the name of the host that the database is on
*/
SQLiteConnection::SQLiteConnection(const std::string& connectionStr, 
								 const std::string& databaseName, 
								 const std::string& userName, 
								 const std::string& password, 
								 const std::string& hostName /*= ""*/ )
								 : IDbConnection(
								 connectionStr
								 ,databaseName
								 ,userName
								 ,password
								 ,hostName)
								 ,m_pConnection(NULL)
{
	createConnection();
}


/**
* ~MysqlConnection
* 
* Sole standard destructor.  Frees the handles allocated in the constructor.
*/
SQLiteConnection::~SQLiteConnection()
{
	//clean and free IQueryAdapter
	{
		ThreadGuard guard( m_queryLock );

		for ( std::map<IQueryAdapter*, long>::iterator itLoop = m_mapActiveQuery.begin();
			m_mapActiveQuery.end() != itLoop; ++itLoop )
		{
			IQueryAdapter* pQueryAdapter = NULL;
			pQueryAdapter = itLoop->first;

			try
			{
				if ( NULL == pQueryAdapter )
				{
					LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, "Unexpected NULL pointer of MysqlQueryAdapter" );
					continue;
				}

				if ( !pQueryAdapter->cleanQuery() )
				{
					delete (pQueryAdapter);
					pQueryAdapter = NULL;
					LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugTrace, "MysqlQueryAdapter: 0x%p is deleted by ConnectionReaper in destructor of MysqlConnection", itLoop->first );
				}
				// else leave it to SimpleDb for deleting
			}
			catch (...)
			{
				LOG( SourceInfo, DebugUtil::ExceptionCatch, "Unknown", "cleanQuery: Unknown exception" );
			}
		}

		m_mapActiveQuery.clear();
	}


	// The connection is closed when the server is detached.
	try
	{
		//fix bug: when mysql db server offline, 
		//MysqlConnection::createConnection() will set m_pConnection=NULL, so we need to check m_pConnection's value here
		if (NULL != m_pConnection)
		{
			m_pConnection->close();
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "FAILOVERDBLOG: connection closed in destruction function.");	
			delete m_pConnection;
			m_pConnection = NULL;
		}
	}
	catch( const DBException& e )
	{
		LOG( SourceInfo, DebugUtil::ExceptionCatch, "TA_Base_Core::DBException", e.getSummary().c_str() );
	}
	catch (...)
	{
		LOG( SourceInfo, DebugUtil::ExceptionCatch, "Unknown", "~MysqlConnection(): Unknown exception" );
	}
}


/**
* createConnection
* 
* creates the underlying framework to support a session
*/
void SQLiteConnection::createConnection()
{
	try
	{		
		if (NULL != m_pConnection)
		{		
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "FAILOVERDBLOG: connection already closed, create or reconnect to database.");		
			delete m_pConnection;
			m_pConnection = NULL;
		}	
		if (NULL == m_pConnection)
		{
			m_pConnection = new TA_Base_Core::CppSQLite3DB();

			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
				"SQLite Header Version: %s", CppSQLite3DB::SQLiteHeaderVersion() );
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
				"SQLite Library Version: %s", CppSQLite3DB::SQLiteLibraryVersion() );
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
				"SQLite Library Version Number: %d", CppSQLite3DB::SQLiteLibraryVersionNumber() );
						
			remove(m_databaseName.c_str());
			m_pConnection->open(m_databaseName.c_str());

		}				

	}
	catch(CppSQLite3Exception & e)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
			"SQLiteConnection exception: Error Code: %d, Error Msg: %s", 
			e.errorCode(), e.errorMessage());

		//const std::string status, const std::string reason, long nativeCode
		DBException db("SQLite_ERROR", e.errorMessage(), e.errorCode());
		throw db;
	}		
	catch(...)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "SQLite_ERROR: unknown database exception" );
		DBException db("SQLite_ERROR", "unknown database exception", 0);
		throw db;
	}	
}

/**
* getConnectionString
*
* Returns the connection string for this MysqlConnection.
*
* @return std::string  the connection string in the form
*                      databaseName:userName:password:hostName
*/
std::string SQLiteConnection::getConnectionString() const
{
	return m_connectionStr;
}

bool SQLiteConnection::isOpen()
{
	FUNCTION_ENTRY("isOpen");
	ThreadGuard guard( m_lock );
	
	if (NULL == m_pConnection)
	{
		FUNCTION_EXIT;
		return false;
	}
	else
	{
		FUNCTION_EXIT;
		return true;
	}

}

void SQLiteConnection::open()
{
	FUNCTION_ENTRY("open");
	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "FAILOVERDBLOG: Fetching thread guard.");
	ThreadGuard guard( m_lock );

	if (NULL != m_pConnection)
	{
		//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "FAILOVERDBLOG: Connection Exists.");
		m_usageCount++;
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "FAILOVERDBLOG: Current usage count is %d, use established connection", m_usageCount);
		FUNCTION_EXIT;
		return;
	}

	try  // connection closed, reconnect to the database server
	{
		//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "FAILOVERDBLOG: Connecting to DB.");
		createConnection();		
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "FAILOVERDBLOG: Current usage count is %d, use new connection", m_usageCount);
		m_usageCount = 1;
	}
	catch(const DBException& e)
	{
		throw e;
	}
	catch(...)
	{
		DBException db("SQLite_ERROR", "unknown database exception", 0);
		throw db;
	}	

	FUNCTION_EXIT;
}


/**
* close
*
* Ends the session with the database if there is one running.
*
* @return true if the session was ended, false otherwise
*/
void SQLiteConnection::close()
{
	FUNCTION_ENTRY("close");
	ThreadGuard guard( m_lock );
	if (NULL == m_pConnection)
	{
		LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "SQLiteConnection::close(): Database is already closed" );
		FUNCTION_EXIT;
		return;
	}

	if ( m_usageCount > 0 )
	{		
		m_usageCount--;
	}	


	try
	{
		if ( 0 == m_usageCount )
		{
			m_pConnection->close();//
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "FAILOVERDBLOG: connection closed in close() function.");
		}

	}
	catch(CppSQLite3Exception & e)
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
			"SQLiteConnection exception: Error Code: %d, Error Msg: %s", 
			e.errorCode(), e.errorMessage());

		//const std::string status, const std::string reason, long nativeCode
		DBException db("SQLite_ERROR", e.errorMessage(), e.errorCode());
		throw db;
	}	

	FUNCTION_EXIT;
}


/**
* closeAll
*
* Ends all sessions with the database if there are any running.
*
* @return true if the sessions were ended, false otherwise
*/
void SQLiteConnection::closeAll()
{
	FUNCTION_ENTRY("closeAll");
	{
		ThreadGuard guard( m_lock );
		// If m_usageCount is 1, close() will close everything.
		m_usageCount = 1;
	}


	try
	{
		close();
	}
	catch(DBException & e)
	{
		e.getNativeCode();
		//throw e;
	}	

	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "FAILOVERDBLOG: connection closed in closeAll() function.");
	FUNCTION_EXIT;
}


/**
* exec
*
* Executes the given statement on the database.  This method
* handles both queries (e.g. "SELECT...") and non-queries (e.g."INSERT...")
*
* @param statement    the statement to execute
* @param isQuery      a bool that is true if the statement is a query
* @param numColumns   the number of columns to retrieve
* @param bindType     the type of binding required
* @return true if the statement was executed, false otherwise
*/
void SQLiteConnection::exec( const SQLStatement& rSqlObj, IQueryAdapter*& pIQueryAdapter, bool isQuery /*= false*/, int prefetch /*= 10*/)
{
	FUNCTION_ENTRY("exec");		

	if ( NULL != pIQueryAdapter )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "pIQueryAdapter should not be other value except NULL" );
		cleanQuery( pIQueryAdapter );
	}
	if (NULL == m_pConnection)
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "SQLite connection is null" );
		TA_THROW( DBException("SQLite_ERROR", "SQLite connection is gone away", ER_MYSQL_GONE_AWAY) );
	}
	std::string strSql;
	_GetSQL(strSql, rSqlObj);

	TA_ASSERT(!strSql.empty(), "SQL statement is empty");
	if ( strSql.size() > DEFMAXLOGLEN)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL,"Begin execute the LargeSQL statement: [[");
		LOGLARGESTRING(SourceInfo, DebugUtil::DebugSQL, strSql.c_str());
	}
	else
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugSQL, "Begin execute the SQL statement: [[%s]]", strSql.c_str());
	}

	CppSQLite3Query* pQuery = NULL;
	try
	{
		ThreadGuard guard( m_lock );
		// check the database connection first, if it already closed, open it
		if (NULL == m_pConnection)
		{
			open();
		}		

		if (isQuery) //select *
		{
			pQuery = m_pConnection->execQueryEx(strSql.c_str());
		}
		else //like the insert command
		{
			LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "The SQL modification to execute is: %s", strSql.c_str());
			int nRowsChanged = 0;
			nRowsChanged = m_pConnection->execDML(strSql.c_str());
			if (0 == nRowsChanged)
			{
				LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, 
					"the statement is not a insert like statement" );
			}
			else
			{
				LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, 
					"the statement exec, and %s Rows Changed", nRowsChanged);
			}

		}
	}
	catch(const DBException& e)
	{
		if (NULL != pQuery)
		{
			delete pQuery;
			pQuery = NULL;
		}
		throw e;
	}
	catch(CppSQLite3Exception & e)
	{
		if (NULL != pQuery)
		{
			delete pQuery;
			pQuery = NULL;
		}
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Failed to exec SQLID: %s, SQL: %s", rSqlObj.strSQLID.c_str(), strSql.c_str() );
		DBException db("SQLite_ERROR", e.errorMessage(), e.errorCode());
		throw db;
	}	
	catch(...)
	{
		if (NULL != pQuery)
		{
			delete pQuery;
			pQuery = NULL;
		}
		LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Failed to exec SQLID: %s, SQL: %s", rSqlObj.strSQLID.c_str(), strSql.c_str() );
		DBException db("SQLiteConnection_ERROR", "unknown database exception", 0);
		throw db;
	}	

	//just save the pointer, here
	if (NULL != pQuery)
	{
		ThreadGuard guard( m_queryLock );
		pIQueryAdapter = new SQLiteQueryAdapter( pQuery );
		m_mapActiveQuery[pIQueryAdapter] = QUERY_EXIST;
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugTrace, 
			"SQLiteQueryAdapter: 0x%p is created for connection: 0x%p", pIQueryAdapter, this );

	}


	FUNCTION_EXIT;
}

void SQLiteConnection::fetchBLOB(const std::string &strSQL, 
								const std::string &colomnName, 
								DataClobArray& data )
{
	CppSQLite3Query* pQuery = NULL;

	FUNCTION_ENTRY("fetchBLOB");

	try
	{
		if (NULL == m_pConnection)
		{
			open();
		}
		TA_ASSERT(NULL != m_pConnection, "Connection is NULL");
		pQuery = m_pConnection->execQueryEx(strSQL.c_str());
		
		CppSQLite3Binary blob;

		int nBinaryLength = 0;
		size_t nIndex       = 0;
		const size_t nFetchRows  = 10;    // think whether it's enough?		

		while( pQuery &&!pQuery->eof() && nIndex < nFetchRows )
		{

			blob.setEncoded((unsigned char*)pQuery->fieldValue(colomnName.c_str()));
			nBinaryLength = blob.getBinaryLength();
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDatabase,
				"Retrieved binary Length: %d", nBinaryLength);

			if (nBinaryLength > 0)  // have data 
			{
				DataClob dataclob;
				const unsigned char* pBinData = blob.getBinary();
				unsigned char ucharValue = 0;
				int nDIdx = 0;
				for (nDIdx = 0; nDIdx < nBinaryLength; nDIdx++)
				{
					ucharValue = pBinData[nDIdx];
					dataclob.push_back(ucharValue);
				}
				data.push_back(dataclob);
			}

			pQuery->nextRow(); 
			nIndex++;
		}
		// we finished use it, and need to clean it.
		if (NULL != pQuery)
		{
			pQuery->finalize();
			delete pQuery;
			pQuery = NULL;
		}
	}
	catch(CppSQLite3Exception & e )
	{
		DBException db("SQLite3_ERROR", e.errorMessage(), e.errorCode());
		throw db;
	}		
	catch(...)
	{
		DBException db("SQLiteConnection_ERROR", "unknown database exception", 0);
		throw db;
	}

	FUNCTION_EXIT;
}


void SQLiteConnection::updateBLOB(
	const std::string &strTableName, 
	const std::string &colomnName,
	const std::vector<unsigned char>& data, 
	const std::string &strWhereClause
	)
{
	FUNCTION_ENTRY("updateBLOB");
	std::stringstream SqlStatement;
	try
	{
		TA_ASSERT(strTableName.length() > 0, "MysqlConnection::updateBLOB::Invalid table name.");
		TA_ASSERT(colomnName.length() > 0, "MysqlConnection::updateBLOB::Invalid ColumnLabel parameter");

		if (NULL == m_pConnection)
		{
			open();
		}

		CppSQLite3Buffer bufSQL;
		CppSQLite3Binary blob;
		
		if (data.size() > 0)  // have data to insert
		{
			int nBinDataSize = data.size();
			nBinDataSize += 1;
			unsigned char* pBinData = new unsigned char[nBinDataSize];
			memset(pBinData, 0, nBinDataSize);

			unsigned char uCharValue = 0;
			nBinDataSize = data.size();
			for (int nDIdx = 0; nDIdx < nBinDataSize; nDIdx++)
			{
				uCharValue = data[nDIdx];
				pBinData[nDIdx]= uCharValue;
			}
			
			blob.setBinary(pBinData, nBinDataSize);

			if (NULL != pBinData)
			{
				delete[] pBinData;
				pBinData = NULL;
			}
		}
		LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "[mysql-info], Not a update like statement." );


		if ( strWhereClause.length() > 0 )
		{
			bufSQL.format("UPDATE %s SET %s = %Q WHERE", 
				strTableName.c_str(), colomnName.c_str(), blob.getEncoded(), strWhereClause.c_str());	
		}
		else
		{
			bufSQL.format("UPDATE %s SET %s = %Q", 
				strTableName.c_str(), colomnName.c_str(), blob.getEncoded());
		}
	}
	catch(  CppSQLite3Exception & e)
	{
		LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Failed to exec SQL statement '%s'",
			SqlStatement.str().c_str() );

		DBException db("SQLite_ERROR", e.errorMessage(), e.errorCode());
		throw db;
	}		
	catch(...)
	{
		DBException db("SQLiteConnection_ERROR", "unknown database exception", 0);
		throw db;
	}	

	FUNCTION_EXIT;
}

/**
* incrementExecutionCount
*
* Increment the count of threads that are using this connection right now.
* Note: The count does not include threads that have this connection open, 
* but are not executing on it right now.
*/
void SQLiteConnection::incrementExecutionCount()
{
	FUNCTION_ENTRY("incrementExecutionCount");
	ThreadGuard guard( m_countLock );
	m_executionCount++;
	LOG_GENERIC(SourceInfo, DebugUtil::DebugTrace, "MysqlConnection::incrementExecutionCount(), Execution count: %d" ,m_executionCount );
	FUNCTION_EXIT;
}


/**
* decrementExecutionCount
*
* Decrement the count of threads that are using this connection right now.
* Note: The count does not include threads that have this connection open,
* but are not executing on it right now.
*/
void SQLiteConnection::decrementExecutionCount()
{
	FUNCTION_ENTRY("decrementExecutionCount");
	ThreadGuard guard( m_countLock );
	if (m_executionCount > 0)
	{
		m_executionCount--;
	}
	LOG_GENERIC(SourceInfo, DebugUtil::DebugTrace, "MysqlConnection::decrementExecutionCount(), Execution count: %d" ,m_executionCount );
	FUNCTION_EXIT;
}

bool SQLiteConnection::isAnyQueryActive( unsigned long ulMaxTimeoutInSecs )
{
	FUNCTION_ENTRY("isAnyQueryActive");

	ThreadGuard guard( m_queryLock );

	for ( std::map<IQueryAdapter*, long>::iterator itLoop = m_mapActiveQuery.begin(); 
		m_mapActiveQuery.end() != itLoop; ++itLoop )
	{
		if ( NULL == itLoop->first )
		{
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, "Unexpected NULL pointer of OCIQueryAdapter" );
			continue;
		}

		if ( (itLoop->first)->isActive( ulMaxTimeoutInSecs ))
		{
			FUNCTION_EXIT;
			return true;
		}
	}

	FUNCTION_EXIT;
	return false;
}

void SQLiteConnection::cleanQuery( IQueryAdapter*& pQuery )
{
	FUNCTION_ENTRY("cleanQuery");

	if ( NULL == pQuery )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, "Invalid input parameter: NULL pointer of MysqlConnection" );
		return;
	}

	ThreadGuard guard( m_queryLock );

	std::map<IQueryAdapter*, long>::iterator itExistQuery = m_mapActiveQuery.find( pQuery );
	if ( m_mapActiveQuery.end() != itExistQuery )
	{
		if ( NULL == itExistQuery->first )
		{
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, "Unexpected NULL pointer of MysqlConnection" );
			return;
		}

		// Destructor will clean query
		delete (itExistQuery->first);
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugTrace, "MysqlConnection: 0x%p is deleted by SimpleDb", pQuery );

		m_mapActiveQuery.erase( itExistQuery );
	}
	else
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, "Specified query is not found in this connection, the old connection has been reaped" );
		if ( !pQuery->cleanQuery() )
		{
			delete pQuery;
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugTrace, "MysqlConnection: 0x%p is deleted by SimpleDb through it has been reaped", pQuery );
		}
		// else leave it to ConnectionReaper for deleting
	}

	// Set pointer to NULL
	pQuery = NULL;

	FUNCTION_EXIT;
}

void  SQLiteConnection::_GetSQL(std::string& strSql, const SQLStatement& rSqlObj)
{
	if (!rSqlObj.strCommon.empty())
	{
		strSql = rSqlObj.strCommon;
	}
	else if (!rSqlObj.strSqlite.empty())
	{
		strSql = rSqlObj.strSqlite;
	}
	else
	{
		strSql.clear();
	}
}

NS_END(TA_Base_Core)