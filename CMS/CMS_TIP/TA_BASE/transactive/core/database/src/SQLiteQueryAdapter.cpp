//////////////////////////////////////////////////////////////////////
///		@file		MysqlQueryAdapter.h
///		@author		zhilin,ouyang
///		@date		2010-11-24 14:44:41
///
///		@brief	    Implementation of the interface IQueryAdapter. 
///					use mysql database server to implement the query  
///                 operations.
//////////////////////////////////////////////////////////////////////
#include "core/database/src/SQLiteQueryAdapter.h"
#include <time.h>
#include "core/utilities/src/DebugUtil.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/database/src/DBException.h"
#include "core/CppSQLiteLib/src/CppSQLite3.h"


NS_BEGIN(TA_Base_Core)

SQLiteQueryAdapter::SQLiteQueryAdapter( CppSQLite3Query* pQuery )
{
	m_pQuery = pQuery;
	m_bConnectionClosed = false;
	m_tmLatestActionTime = 0;

	TA_ASSERT( NULL != m_pQuery, "Can't initialize with NULL pointer of CppSQLite3Query" );
	_UpdateLatestActionTime();
}


SQLiteQueryAdapter::~SQLiteQueryAdapter()
{
	cleanQuery();
}

bool SQLiteQueryAdapter::hasMore()
{
	bool bHasMore = false;
	bool bIsEof = false;

	ThreadGuard guardQuery( m_queryLock );

	_UpdateLatestActionTime();

	if ( NULL == m_pQuery )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Query is closed by timeout" );
		TA_THROW( TA_Base_Core::DatabaseException("Query is closed by timeout") );
	}

	try
	{
		bIsEof = m_pQuery->eof();
	}
	catch (CppSQLite3Exception& e)
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, 
			"Query check eof, errorCode: %d, errorMessage:%s", e.errorCode(), e.errorMessage() );
		bIsEof = true;
	}
	
	bHasMore = !bIsEof;

	return bHasMore;
}

void SQLiteQueryAdapter::fetchInitial( int numColumns, Buffer& data, long lMaxRows )
{
	bool bIsEof = false;

	ThreadGuard guardQuery( m_queryLock );

	_UpdateLatestActionTime();
	data.clear();

	if ( NULL == m_pQuery )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Query is closed by timeout" );
		TA_THROW( TA_Base_Core::DatabaseException("Query is closed by timeout") );
	}

	try
	{
		bIsEof = m_pQuery->eof();
		if (!bIsEof)
		{
			_FillDataBuffer(numColumns, data, lMaxRows);
		}
	}
	catch( DBException & e )
	{
		std::ostringstream osMsg;
		osMsg << "Status:" << e.getStatus() << " Summary:" << e.getSummary() 
			<< " Reason:" << e.getReason() <<" NativeCode: "<< e.getNativeCode() << std::endl; 

		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Database exception: %s", osMsg.str().c_str());
	}
	catch (...)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Unknown exception, Unknown reason");
	}
}

void SQLiteQueryAdapter::fetchMore( int numColumns, Buffer& data, long lMaxRows )
{
	TA_ASSERT( numColumns <= FETCH_MAX_COLS, "Number of columns is out of range");
	TA_ASSERT( (lMaxRows > 0) && (lMaxRows <= FETCH_MAX_ROWS), "Number of rows is out of range"   );

	ThreadGuard guardQuery( m_queryLock );

	_UpdateLatestActionTime();
	data.clear();

	if ( NULL == m_pQuery )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Query is closed by timeout" );
		TA_THROW( TA_Base_Core::DatabaseException("Query is closed by timeout") );
	}

	try
	{
		if ( m_pQuery->eof() )
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "no rows returns from SQL");
			return;
		}

		_FillDataBuffer( numColumns, data, lMaxRows );
	}
	catch(const DBException& e)
	{
		std::ostringstream osMsg;
		osMsg << "Status:" << e.getStatus() << " Summary:" << e.getSummary() 
			<< " Reason:" << e.getReason() <<" NativeCode: "<< e.getNativeCode() << std::endl; 

		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Database exception: %s", osMsg.str().c_str());
	}
	catch( ... )
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Unknown exception, Unknown reason");
	}
}

void SQLiteQueryAdapter::_FillDataBuffer( int numColumns, Buffer& data, long lMaxRows )
{
	int nFieldNumber = 0;

	ThreadGuard guardQuery( m_queryLock );

	data.clear();

	if ( NULL == m_pQuery )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Query is closed by timeout" );
		TA_THROW( TA_Base_Core::DatabaseException("Query is closed by timeout") );
	}

	nFieldNumber = m_pQuery->numFields();
	TA_ASSERT(numColumns <= nFieldNumber, "column number is bigger than the SQL returns");

	try
	{
		long lOffset = 0;
		while( !m_pQuery->eof() && ( lOffset < lMaxRows ))
		{
			Row dataRow;
			for ( int nColLoop = 0; nColLoop < numColumns; ++nColLoop )
			{
				std::string strFieldName = m_pQuery->fieldName(nColLoop);  // 1-based 
				std::string strValue = m_pQuery->fieldValue(nColLoop);
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDatabase, 
					"SQL field name:[%s]; value:[%s]", 
					strFieldName.c_str(), strValue.c_str());
				dataRow.push_back(strValue);
			}
			data.push_back(dataRow);
			m_pQuery->nextRow();
			++lOffset;
		}
	}
	catch(CppSQLite3Exception& e)
	{
		//	DBException( const std::string status, const std::string reason, long nativeCode )
		TA_THROW( DBException ("SQLite_ERROR", e.errorMessage(), e.errorCode()) );
	}
	catch( ... )
	{		
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Unknown exception, Unknown reason");
	}
}

bool SQLiteQueryAdapter::cleanQuery()
{
	FUNCTION_ENTRY("cleanQuery");

	ThreadGuard guardQuery( m_queryLock );

	if ( NULL == m_pQuery )
	{
		FUNCTION_EXIT;
		return false;
	}

	try
	{
		m_pQuery->finalize();
	}
	catch ( CppSQLite3Exception& e )
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugError, 
			"cleanQuery exception: %s", e.errorMessage());
		// Continue to delete query, it must be deleted.
	}
	
	if (NULL != m_pQuery)
	{
		delete m_pQuery;
		m_pQuery = NULL;
	}        
	
	FUNCTION_EXIT;
	return true;
}

bool SQLiteQueryAdapter::isActive( unsigned long ulMaxTimeoutInSecs )
{
	bool bIsEOF = false;
	bool bIsActive = false;

	FUNCTION_ENTRY("isActive");

	ThreadGuard guardQuery( m_queryLock );

	if ( NULL == m_pQuery )
	{
		FUNCTION_EXIT;
		return false;
	}
	time_t tmNoneActiveTime = time( NULL ) - m_tmLatestActionTime;
	if ( ulMaxTimeoutInSecs <= static_cast<unsigned long>(tmNoneActiveTime) )
	{
		FUNCTION_EXIT;
		return false;
	}
	bIsEOF = m_pQuery->eof();
	bIsActive = !bIsEOF;

	FUNCTION_EXIT;
	return bIsActive;
}

void SQLiteQueryAdapter::_UpdateLatestActionTime()
{
	// update latest action time in public member function
	m_tmLatestActionTime = time( NULL );
}

NS_END(TA_Base_Core)