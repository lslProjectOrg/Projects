//////////////////////////////////////////////////////////////////////
///		@file		MysqlQueryAdapter.h
///		@author		zhilin,ouyang
///		@date		2010-11-24 14:44:41
///
///		@brief	    Implementation of the interface IQueryAdapter. 
///					use mysql database server to implement the query  
///                 operations.
//////////////////////////////////////////////////////////////////////
#ifndef MYSQLQUERYADAPTER_H
#define MYSQLQUERYADAPTER_H
#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "IQueryAdapter.h"




NS_BEGIN(TA_Base_Core)
class CppSQLite3Query;

class SQLiteQueryAdapter : public IQueryAdapter
{
public:
	SQLiteQueryAdapter( CppSQLite3Query* pQuery );
	~SQLiteQueryAdapter();

	bool hasMore();

	void fetchInitial( int numColumns, Buffer& data, long lMaxRows );

	void fetchMore( int numColumns, Buffer& data, long lMaxRows );

	bool cleanQuery();

	bool isActive( unsigned long ulMaxTimeoutInSecs );

private:
	void _FillDataBuffer( int numColumns, Buffer& data, long lMaxRows );
	void _UpdateLatestActionTime();

private:
	// Disable default constructor, copy constructor, operator =
	SQLiteQueryAdapter();
	const SQLiteQueryAdapter& operator=( const SQLiteQueryAdapter& refSrc );
	SQLiteQueryAdapter( const SQLiteQueryAdapter& refSrc );

private:

	bool                                  m_bConnectionClosed;	
	time_t                                m_tmLatestActionTime;
	CppSQLite3Query*                       m_pQuery;
	TA_Base_Core::ReEntrantThreadLockable m_queryLock;
};

NS_END(TA_Base_Core)


#endif /// MYSQLQUERYADAPTER_H