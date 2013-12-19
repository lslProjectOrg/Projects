#ifndef __CLASS_CppSQLite3_STATEMENT_H__
#define __CLASS_CppSQLite3_STATEMENT_H__


#include "core/CppSQLiteLib/src/CppSQLiteLibCommonData.h"
#include "core/CppSQLiteLib/src/CppSQLite3Query.h"


NS_BEGIN(TA_Base_Core)


class CppSQLite3Statement
{
public:

    CppSQLite3Statement();

    CppSQLite3Statement(const CppSQLite3Statement& rStatement);

    CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM);

    virtual ~CppSQLite3Statement();

    CppSQLite3Statement& operator=(const CppSQLite3Statement& rStatement);

    int execDML();

    CppSQLite3Query execQuery();

    void bind(int nParam, const char* szValue);
    void bind(int nParam, const int nValue);
    void bind(int nParam, const double dwValue);
    void bind(int nParam, const unsigned char* blobValue, int nLen);
    void bindNull(int nParam);

	int bindParameterIndex(const char* szParam);
    void bind(const char* szParam, const char* szValue);
    void bind(const char* szParam, const int nValue);
    void bind(const char* szParam, const double dwValue);
    void bind(const char* szParam, const unsigned char* blobValue, int nLen);
    void bindNull(const char* szParam);

	void reset();

    void finalize();

private:

    void checkDB();
    void checkVM();

    sqlite3* mpDB;
    sqlite3_stmt* mpVM;
};


NS_END(TA_Base_Core)

#endif
