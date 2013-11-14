/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/database/src/SimpleDb.cpp $
* @author:  Ripple
* @version: $Revision: #7 $
*
* Last modification: $DateTime: 2013/11/06 10:02:15 $
* Last modified by:  $Author: weikun.lin $
*
* <description>
*
*/
//
// SimpleDb.cpp
//
// Wrapper for the DbServerLocator class which (hopefully) takes all the pain
// out of database access. Note that it is probably not very efficient.
//
// Author: Bruce Fountain 24-Jan-2001
//
#include "core/database/src/SimpleDb.h"

#include "core/database/src/CommonType.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/threads/src/Thread.h"
#include "core/database/src/DbServerLocator.h"
#include "core/database/src/IQueryAdapter.h"
#include "core/database/src/DBException.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/DatabaseException.h"



using TA_Base_Core::DebugUtil;
using TA_Base_Core::Thread;


namespace TA_Base_Core
{
	const unsigned long SimpleDb::MAX_ROWS = FETCH_MAX_ROWS;
	const unsigned long SimpleDb::MAX_COLS = FETCH_MAX_COLS;

	SimpleDb::SimpleDb(const char* pszDbType, const char* connection, const char* user, const char* passwd, const char* hostname) : 
		m_pCurQuery(NULL), m_nDBType(enumOracleDb)
	{
		FUNCTION_ENTRY("SimpleDb");

		LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug,
			"SimpleDb::SimpleDb(): dbType=%s, connection=%s, user=%s, passwd=%s, hostname=%s", 
			pszDbType, connection, user, passwd, hostname);

		// Create a string from the database connection details.
		// This will be used to identify the OCIConnection object.
		m_connectionStr = pszDbType;
		m_connectionStr += ":";
		m_connectionStr += connection;
		m_connectionStr += ":";
		m_connectionStr += user;
		m_connectionStr += ":";
		m_connectionStr += passwd;
		m_connectionStr += ":";
		m_connectionStr += hostname;

		parseDBType(pszDbType);

		// The final parameter is false to indicate that the connection is
		// not being created as part of auto-reconnect.
		//DbServerLocator::getInstance().setDbServerType( m_nDBType );
		DbServerLocator::getInstance().createConnection(m_connectionStr, pszDbType, connection, user, passwd, hostname, false );

		FUNCTION_EXIT;
	}

	SimpleDb::~SimpleDb()
	{
		FUNCTION_ENTRY("~SimpleDb");

		try
		{
			cleanQuery();
			//todo..., think it's not necessary to close so often. 
			//Close(); //TODO: need to implement it in connection class
		}
		catch( const DBException& e )
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "TA_Base_Core::DBException: %s", e.getSummary().c_str() );
		}

		FUNCTION_EXIT;
	}

	void SimpleDb::Open()
	{
		FUNCTION_ENTRY("Open");

		DbServerLocator::getInstance().open( m_connectionStr );

		FUNCTION_EXIT;
	}

	void SimpleDb::Close()
	{
		FUNCTION_ENTRY("Close");

		cleanQuery();
		//Cannot release connection on this class, it should be in charge by OCIAccess;
		//OCIAccess::getInstance().close( m_connectionStr );
		LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "SimpleDb::SimpleDb(): close database = [%s]", m_connectionStr.c_str());

		FUNCTION_EXIT;
	}

	void SimpleDb::CloseAll()
	{
		FUNCTION_ENTRY("CloseAll");

		cleanQuery();

		DbServerLocator::getInstance().closeAll( m_connectionStr );

		FUNCTION_EXIT;
	}

	void SimpleDb::cleanQuery()
	{
		FUNCTION_ENTRY( "cleanQuery" );
		//OCIConnection::cleanUpQuery(m_currentQuery);
		if ( NULL != m_pCurQuery )
		{
			DbServerLocator::getInstance().cleanQuery( m_connectionStr, m_pCurQuery );
		}
		FUNCTION_EXIT;
	}	

	void SimpleDb::Exec(const SQLStatement& rSqlObj)
	{
		FUNCTION_ENTRY("Exec");

		//TA_ASSERT( !statement.empty(), "SQL statement is empty" );
		try
		{
			// Delegate to DbServerLocator
			DbServerLocator::getInstance().exec( m_connectionStr, rSqlObj );
		}
		catch(DBException & e)
		{
			std::ostringstream osMsg;
			osMsg << "Status:" << e.getStatus() << " Summary:" << e.getSummary() << " Reason:" << e.getReason() << std::endl; 

			LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Database exception: %s", osMsg.str().c_str());
			
			int nErrCode = e.getNativeCode();
			if (ER_MYSQL_CANNT_CONNECT == nErrCode ||
				ER_MYSQL_GONE_AWAY == nErrCode ||
				ER_ORA_IMMIDATE_SHUTDOWN == nErrCode ||
				ER_ORA_INITORSHUTDOWN == nErrCode ||
				ER_ORA_NOT_AVALIABLE == nErrCode ||
				ER_ORA_TNS_CLOSE == nErrCode)
			{
				// if go here, it means the db gone bad, rethrow and try another one
				//db error
				throw e;				
			}
			else
			{
				//sql error
				throw e;				
			}
		}
		FUNCTION_EXIT;
	}

	void SimpleDb::Exec(const SQLStatement& rSqlObj, int numColumns,
		Buffer& data, int maxrows /*=FETCH_MAX_ROWS*/)
	{
		FUNCTION_ENTRY("Exec");

		TA_ASSERT( (maxrows > 0) && (maxrows <= FETCH_MAX_ROWS), "Number of rows is out of range"   );

		try
		{
			data.clear();
			cleanQuery();
			m_pCurQuery = DbServerLocator::getInstance().exec( m_connectionStr, rSqlObj, maxrows); 

			if ( NULL == m_pCurQuery )
			{
				LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Query is not got" );
			}
			else
			{
				m_pCurQuery->fetchInitial( numColumns, data, maxrows );
			}
		}
		catch(DBException & e)
		{
			std::ostringstream osMsg;
			osMsg << "Status:" << e.getStatus() << " Summary:" << e.getSummary() << " Reason:" << e.getReason() << std::endl; 

			LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Database exception: %s", osMsg.str().c_str());
			
			int nErrCode = e.getNativeCode();
			if (ER_MYSQL_CANNT_CONNECT == nErrCode ||
				ER_MYSQL_GONE_AWAY == nErrCode ||
				ER_ORA_IMMIDATE_SHUTDOWN == nErrCode ||
				ER_ORA_INITORSHUTDOWN == nErrCode ||
				ER_ORA_NOT_AVALIABLE == nErrCode ||
				ER_ORA_TNS_CLOSE == nErrCode)
			{
				// if go here, it means the db gone bad, rethrow and try another one
				throw e;
			}	
			else
			{
				// if go here : sql error
				throw e;
			}	
		}

		FUNCTION_EXIT;
	}

	void SimpleDb::FetchMore(int numColumns, Buffer& data, int maxrows /*=FETCH_MAX_ROWS*/ )
	{
		FUNCTION_ENTRY("FetchMore");

		if ( NULL == m_pCurQuery )
		{
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Query is closed by timeout" );
			// Or throw exception?
			return;
		}
		m_pCurQuery->fetchMore( numColumns, data, maxrows );

		FUNCTION_EXIT;
	}


	void SimpleDb::readBLOB( const std::string& fieldName, const std::string& tableName,
		const std::string& whereClause, std::vector<unsigned char>& data )
	{
		FUNCTION_ENTRY("readBLOB");

		TA_ASSERT( fieldName.length() > 0, "No field name has been specified" );
		TA_ASSERT( tableName.length() > 0, "No table name has been specified" );

		DbServerLocator::getInstance().readBLOB( m_connectionStr, fieldName, tableName, whereClause, data );

		FUNCTION_EXIT;
	}


	void SimpleDb::writeBLOB( const std::string& fieldName, const std::string& tableName,
		const std::string& whereClause, std::vector<unsigned char>& data )
	{
		FUNCTION_ENTRY("writeBLOB");

		TA_ASSERT( fieldName.length() > 0, "No field name has been specified" );
		TA_ASSERT( tableName.length() > 0, "No table name has been specified" );

		DbServerLocator::getInstance().writeBLOB( m_connectionStr, fieldName, tableName, whereClause, data );

		FUNCTION_EXIT;
	}


	void SimpleDb::ParseConnectStr(const std::string& str,                  // IN
		std::vector<std::string>& components,    // OUT
		char delimiter)                          // IN
	{
		DbServerLocator::getInstance().parseConnectStr( str, components, delimiter );		
	}

	bool SimpleDb::IsOpen()
	{
		return DbServerLocator::getInstance().isOpen(m_connectionStr);
	}

	int SimpleDb::GetDbServerType()
	{
		return m_nDBType;
	}

	void SimpleDb::parseDBType(const std::string& strDBName)
	{
		if(strDBName.compare(defOracleDBName) == 0) //case sensitive
		{
			m_nDBType = static_cast<int>(enumOracleDb);
		}
		else if (strDBName.compare(defMysqlDBName) == 0)
		{
			m_nDBType = static_cast<int>(enumMysqlDb);
		}
		else if (strDBName.compare(defSqliteDBName) == 0)
		{
			m_nDBType = static_cast<int>(enumSqliteDb);
		}
		else   
		{    // should never reach here, if reach here, then raise a exception
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError,
				"the DB type string in database config file wrong(should be: Oracle or Mysql or Sqlite): %s", strDBName.c_str() );
			TA_ASSERT(false, "DB Type string error");  // make a exception deliberately
		}
	}

	bool SimpleDb::IsNeedChange(const std::string& strCon)
	{
		return strCon.compare(m_connectionStr);
	}

	bool SimpleDb::HasMore() const
	{
		//TA_ASSERT(m_currentQuery != NULL, "m_currentQuery should not be NULL");
		//return (!m_currentQuery->isEOF());
		if ( NULL == m_pCurQuery )
		{
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Query is closed by timeout" );
			// Or throw exception?
			return false;
		}

		return m_pCurQuery->hasMore();
	}
}
