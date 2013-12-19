#ifndef _CLASS_SQL_FILE_HELPER_H__
#define _CLASS_SQL_FILE_HELPER_H__


#include "core/DataAccess/src/DataAccessCommonData.h"

#include "boost/unordered_map.hpp"
#include "core/DataAccess/src/SQLStatement.h"

NS_BEGIN(TA_Base_Core)

class SQLFileHelper
{
private:	
	typedef boost::unordered_map<long, std::string> SQLHashTable;
	typedef boost::unordered_map<long, std::string>::value_type SQLValueType; 

	typedef boost::unordered_map<std::string, int> strSQLHashIndexT;
	typedef boost::unordered_map<std::string, int>::value_type strSQLHashIndexValT;
	typedef boost::unordered_map<std::string, int>::iterator strSQLIndexIterT;


public:
	// SQLFileHelper Constructors and destructor
	SQLFileHelper();
	~SQLFileHelper();

public:
	std::string getSQLString(long uSQLID);	
	void  getSQLString(const std::string& strSQLKey, SQLStatement& rSqlStatements);
private:
	void   _InitSQLHashIndex();
private:
	int                       m_nArrSize;
	strSQLHashIndexT          m_hSQLHashIndex;
	SQLHashTable              m_SQLHashTable;
};

NS_END(TA_Base_Core)

#endif  /// _CLASS_SQL_FILE_HELPER_H__
