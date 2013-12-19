#ifndef __CLASS_DATA_ACCESS_EXECPTION_H__
#define __CLASS_DATA_ACCESS_EXECPTION_H__

#include <string>
#include <exception>
#include "core/DataAccess/src/DataAccessCommonData.h"


NS_BEGIN(TA_Base_Core)


class DataAccessException : public std::exception
{
public:
	DataAccessException(int nErrorCode,	const std::string strReason) throw();
	DataAccessException(DataAccessException& e);
	virtual ~DataAccessException() throw();

public:
	std::string getErrorMsg();
	std::string getErrorString();
	std::string getReason();
	int getErrorCode();
	void logInfo();

public:
	std::string errorCodeAsString(int nErrCode);

private:
	int				m_nErrorCode;
	std::string     m_strReason;

	std::string		m_strErrMessage;
	std::string     m_strErrorString;

};


NS_END(TA_Base_Core)

#endif //__CLASS_DATA_ACCESS_EXECPTION_H__
