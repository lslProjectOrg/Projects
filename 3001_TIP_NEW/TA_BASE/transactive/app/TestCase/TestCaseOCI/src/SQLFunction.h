#ifndef _SQL_FUNCTION_H_
#define _SQL_FUNCTION_H_

#include "ConnPool.h"

class CSQLFunction
{
public:
	CSQLFunction();
	~CSQLFunction();


	int ExecateSQL(Connection *conn,char *sqlStr);
	int SingleQuerySQL(Connection *conn,char *sqlStr,char *format,...);

protected:
private:
	CConnPool* m_pConnPool;
};


#endif