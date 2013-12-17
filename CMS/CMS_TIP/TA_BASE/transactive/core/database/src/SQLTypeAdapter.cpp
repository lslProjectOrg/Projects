//////////////////////////////////////////////////////////////////////
///		@file		SQLTypeAdapter.cpp
///		@author		zhilin,ouyang
///		@date		2010-12-8 14:44:41
///
///		@brief	    to converts many different data types to strings 
///                 suitable for use in SQL queries. This class provides 
///                 implicit conversion between many C++ types and
///                 SQL-formatted string representations of that data 
///                 without losing important type information.              
//////////////////////////////////////////////////////////////////////
#include "core/database/src/CommonType.h"
#include "SQLTypeAdapter.h"
#include "core/exceptions/src/SQLException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/database/src/SQLUtilityFun.h"


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
: m_nDataCategory(enumBeginDbType)
, m_bEmpty(true)
{
	if (rhs.strCommonSQL.empty() && rhs.strOracleSQL.empty() && rhs.strMySQLSQL.empty() && rhs.strSQLiteSQL.empty())
	{
		m_nDataCategory = enumBeginDbType;
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
		m_nDataCategory = enumOracleDb;
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
		TA_THROW(BadIndex("Not enough chars in SQLTypeAdapter", int(i),	int(length(uIndex))));
	}
}

int SQLTypeAdapter::compare(const SQLTypeAdapter& other) const
{
	int nRet = 0;
	for (int i = 0; i < defSupportDBCount; i++)
	{
		nRet = m_strParams[i].compare(other.m_strParams[i]);
		if ( nRet )
		{
			return  nRet;
		}
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

NS_END(TA_Base_Core)



