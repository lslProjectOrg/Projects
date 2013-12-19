#include "SQLTypeAdapter.h"
#include <sstream>
#include "core/DataAccess/src/DataAccessException.h"


NS_BEGIN(TA_Base_Core)



SQLTypeAdapter::SQLTypeAdapter()
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(true)
{
}

SQLTypeAdapter::SQLTypeAdapter(const SQLTypeAdapter& other)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	_CopyData(other);
}

SQLTypeAdapter::SQLTypeAdapter(const std::string& str)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = str;
}

SQLTypeAdapter::SQLTypeAdapter(const char* str)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = str;
}

SQLTypeAdapter::SQLTypeAdapter(char c)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = stream2string(c);
}

SQLTypeAdapter::SQLTypeAdapter(short i)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = stream2string(i);
}

SQLTypeAdapter::SQLTypeAdapter(unsigned short i)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = stream2string(i);
}

SQLTypeAdapter::SQLTypeAdapter(int i)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = stream2string(i);
}

SQLTypeAdapter::SQLTypeAdapter(unsigned i)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = stream2string(i);
}

SQLTypeAdapter::SQLTypeAdapter(long i)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = stream2string(i);
}

SQLTypeAdapter::SQLTypeAdapter(unsigned long i)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = stream2string(i);
}

SQLTypeAdapter::SQLTypeAdapter(long long i)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = stream2string(i);
}

SQLTypeAdapter::SQLTypeAdapter(float i)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = stream2string(i);
}

SQLTypeAdapter::SQLTypeAdapter(double i)
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(false)
{
	m_strParams[0] = stream2string(i);
}

SQLTypeAdapter::SQLTypeAdapter(const SQLStatement& rhs)
{
	m_nDataCategory = enumBeginDbType;
	m_bEmpty = true;

	if (rhs.strCommonSQL.empty() && rhs.strOracleSQL.empty() && rhs.strMySQLSQL.empty() && rhs.strSQLiteSQL.empty())
	{
		m_nDataCategory = enumBeginDbType;
		m_bEmpty = true;
		return;
	}

	m_bEmpty = false;
	if (!rhs.strCommonSQL.empty())
	{
		m_nDataCategory = enumBeginDbType;
		m_strParams[enumBeginDbType] = rhs.strCommonSQL;
	}
	else
	{
		m_nDataCategory = enumSqliteDb;
		m_strParams[enumOracleDb] = rhs.strOracleSQL;
		m_strParams[enumMysqlDb] = rhs.strMySQLSQL;
		m_strParams[enumSqliteDb] = rhs.strSQLiteSQL;
	}
}

SQLTypeAdapter& SQLTypeAdapter::operator =(const SQLTypeAdapter& rhs)
{
	return assign(rhs);
}



SQLTypeAdapter& SQLTypeAdapter::assign(const SQLTypeAdapter& sta)
{
	_CopyData(sta);
	return (*this);
}

char SQLTypeAdapter::at(size_t i, size_t uIndex) const
{
	if (i < length(uIndex))	
	{
		return m_strParams[uIndex].at(i);
	}
	else		
	{
		throw DataAccessException(ER_DB_ERR_SQLTypeAdapter, "Not enough chars in SQLTypeAdapter");
	}
}

int SQLTypeAdapter::compare(const SQLTypeAdapter& other) const
{
	int nRet = 0;
	for (int i = 0; i < defSupportDBCount; i++)
	{
		nRet = m_strParams[i].compare(other.m_strParams[i]);
		if ( nRet ) return  nRet;
	}

	return  nRet;
}

void SQLTypeAdapter::clear()
{
	m_bEmpty = true;
	m_nDataCategory = enumBeginDbType;
	m_strParams[enumBeginDbType].clear();
	m_strParams[enumOracleDb].clear();
	m_strParams[enumMysqlDb].clear();	
	m_strParams[enumSqliteDb].clear();	

}


void  SQLTypeAdapter::_CopyData(const SQLTypeAdapter& other)
{
	m_bEmpty = other.m_bEmpty;
	m_nDataCategory = other.m_nDataCategory;
	for (int i = 0; i < defSupportDBCount; i++)
	{
		m_strParams[i] = other.m_strParams[i];
	}
}

const char* SQLTypeAdapter::data( int nIndex ) const
{
	return (enumBeginDbType == m_nDataCategory ? m_strParams[enumBeginDbType].c_str() : m_strParams[nIndex].c_str());
}

size_t SQLTypeAdapter::length( size_t nIndex ) const
{
	return (enumBeginDbType == m_nDataCategory ? m_strParams[enumBeginDbType].length() : m_strParams[nIndex].length());
}

bool SQLTypeAdapter::is_null( size_t nIndex ) const
{
	return m_strParams[nIndex].empty();
}

bool SQLTypeAdapter::empty() const
{
	return m_bEmpty;
}

int SQLTypeAdapter::getDataCategory() const
{
	return m_nDataCategory;
}

template <class T>
std::string SQLTypeAdapter::stream2string( const T& object )
{
	std::ostringstream str;
	str << object;
	return str.str();
}

NS_END(TA_Base_Core)



