#include "DataAccessException.h"
//#include "core/CppSQLiteLib/src/CppSQLiteLibCommonData.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_Core)


DataAccessException::DataAccessException(int nErrorCode, const std::string strReason) throw()
{
	m_strReason = strReason;
	m_nErrorCode = nErrorCode;
	m_strErrMessage.clear();
	m_strErrorString.clear();
}

DataAccessException::~DataAccessException() throw()
{
// Do nothing.
}


DataAccessException::DataAccessException(DataAccessException&  e)
{
	m_strErrorString = e.getErrorString();
	m_strReason = e.getReason();
	m_nErrorCode = e.getErrorCode();

}


std::string DataAccessException::errorCodeAsString(int nErrCode)
{
	switch (nErrCode)
	{	
	case ER_DB_ERR_UNKNOWN:
		return "ER_DB_ERR_UNKNOWN";
	case ER_DB_ERR_SQLFileHelper: 
		return "ER_DB_ERR_SQLFileHelper";
	case ER_DB_ERR_SQLCode:
		return "ER_DB_ERR_SQLCode";
	case ER_DB_ERR_SQLTypeAdapter:
		return "ER_DB_ERR_SQLTypeAdapter";
	default: 
		return "UNKNOWN_ERROR";
	}
}


std::string DataAccessException::getErrorMsg() 
{
	//std::ostringstream ostr;
	std::stringstream stringStream;
	
	m_strErrMessage.clear();
	m_strErrorString.clear();

	m_strErrorString = errorCodeAsString(m_nErrorCode);
	stringStream<<"ErrorCode="<<m_nErrorCode
		<<" "<<"ErrorString="<<m_strErrorString
		<<" "<<"Reason="<<m_strReason;

	m_strErrMessage = stringStream.str();	

	stringStream.clear();
	stringStream.str("");

	return m_strErrMessage;
}

std::string DataAccessException::getErrorString() 
{
	return m_strErrorString;
}

std::string DataAccessException::getReason() 
{
	return m_strReason;
}

int DataAccessException::getErrorCode() 
{
	return m_nErrorCode;
}

void DataAccessException::logInfo()
{
	LOG_ERROR<<"DataAccessException Info:"<<getErrorMsg();
}





NS_END(TA_Base_Core)



