#ifndef __CLASS_CppSQLite3_BUFFER_H__
#define __CLASS_CppSQLite3_BUFFER_H__

#include "core/CppSQLiteLib/src/CppSQLiteLibCommonData.h"


NS_BEGIN(TA_Base_Core)


class CppSQLite3Buffer
{
public:

    CppSQLite3Buffer();

    ~CppSQLite3Buffer();

    const char* format(const char* szFormat, ...);

    operator const char*() { return mpBuf; }

    void clear();

private:

    char* mpBuf;
};

NS_END(TA_Base_Core)

#endif //__CLASS_CppSQLite3_BUFFER_H__
