#ifndef __CLASS_CppSQLite3_EXECPTION_H__
#define __CLASS_CppSQLite3_EXECPTION_H__


#include "core/CppSQLiteLib/src/ProCommonDef.h"


NS_BEGIN(TA_Base_Core)

class CppSQLite3Exception
{
public:

    CppSQLite3Exception(
		const int nErrCode,
        char* szErrMess,
        bool bDeleteMsg=true);

    CppSQLite3Exception(const CppSQLite3Exception&  e);

    virtual ~CppSQLite3Exception();

    const int errorCode() { return mnErrCode; }

    const char* errorMessage() { return mpszErrMess; }

    static const char* errorCodeAsString(int nErrCode);

private:

    int mnErrCode;
    char* mpszErrMess;
};


NS_END(TA_Base_Core)

#endif //__CLASS_CppSQLite3_EXECPTION_H__
