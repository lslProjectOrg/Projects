#ifndef __CLASS_CppSQLite3_DATABASE_H__
#define __CLASS_CppSQLite3_DATABASE_H__


#include "core/CppSQLiteLib/src/CppSQLiteLibCommonData.h"
#include "core/CppSQLiteLib/src/CppSQLite3Query.h"
#include "core/CppSQLiteLib/src/CppSQLite3Statement.h"
#include "core/CppSQLiteLib/src/CppSQLite3Table.h"

NS_BEGIN(TA_Base_Core)


class CppSQLite3DB
{
public:

    CppSQLite3DB();

    virtual ~CppSQLite3DB();

    void open(const char* szFile);

    void close();

	bool tableExists(const char* szTable);

    int execDML(const char* szSQL);

    CppSQLite3Query execQuery(const char* szSQL);
	CppSQLite3Query* execQueryEx(const char* szSQL);


    int execScalar(const char* szSQL, int nNullValue=0);

    CppSQLite3Table getTable(const char* szSQL);

    CppSQLite3Statement compileStatement(const char* szSQL);

    sqlite_int64 lastRowId();

    void interrupt() { sqlite3_interrupt(m_pDB); }

    void setBusyTimeout(int nMillisecs);

    static const char* SQLiteVersion() { return SQLITE_VERSION; }
    static const char* SQLiteHeaderVersion() { return SQLITE_VERSION; }
    static const char* SQLiteLibraryVersion() { return sqlite3_libversion(); }
    static int SQLiteLibraryVersionNumber() { return sqlite3_libversion_number(); }

	bool IsAutoCommitOn();

private:

    CppSQLite3DB(const CppSQLite3DB& db);
    CppSQLite3DB& operator=(const CppSQLite3DB& db);

    sqlite3_stmt* compile(const char* szSQL);

    void checkDB();

    sqlite3* m_pDB;
    int m_nBusyTimeoutMs;
};

NS_END(TA_Base_Core)

#endif //__CLASS_CppSQLite3_DATABASE_H__
