//SQLFunction.c

#include "SQLFunction.h"

CSQLFunction::CSQLFunction()
{
	m_pConnPool = new CConnPool();
}
CSQLFunction::~CSQLFunction()
{
	if (NULL != m_pConnPool)
	{
		delete m_pConnPool;
		m_pConnPool = NULL;
	}
}

int CSQLFunction::ExecateSQL(Connection *conn, char *sqlStr)
{
	if(!m_pConnPool->checkerr(conn->errorHandle , OCIStmtPrepare(conn->stmtementHandle, 
		conn->errorHandle, (const OraText *)sqlStr, (ub4) strlen((char *)sqlStr),
		(ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT)))
		return FALSE;

	if(!m_pConnPool->checkerr(conn->errorHandle, OCIStmtExecute(conn->serviceHandle, 
		conn->stmtementHandle, conn->errorHandle,
		(ub4)1, (ub4)0, (OCISnapshot *) NULL, 
		(OCISnapshot *) NULL, (ub4)OCI_DEFAULT)))
		return FALSE;
	if(!m_pConnPool->checkerr(conn->errorHandle, OCITransCommit (conn->serviceHandle,
		conn->errorHandle,
		OCI_DEFAULT)))
		return FALSE;
	return TRUE;
}

int CSQLFunction::SingleQuerySQL(Connection *conn,char *sqlStr,char *format,...)
{
	int i;
	char *p;

	OCIDefine *defnp1 = (OCIDefine *) NULL;
	if(!m_pConnPool->checkerr(conn->errorHandle, OCIStmtPrepare(conn->stmtementHandle, 
		conn->errorHandle, (const OraText *)sqlStr, (ub4) strlen((char *)sqlStr), 
		(ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT)))
		return FALSE;

	va_list par_str;
	va_start(par_str,format);
	p = format;
	i = 0;
	while(*p)
	{
		if(*p=='%')
		{
			switch(*(p+1))
			{
			case 's':
				{
					char **buf = va_arg(par_str,char**);
					int len = *((int*)buf);
					memset((char*)buf, 0, len);
					//bzero((char*)buf,len);
					if(!m_pConnPool->checkerr(conn->errorHandle,OCIDefineByPos(conn->stmtementHandle, &defnp1,
						conn->errorHandle,(ub4) ++i,(dvoid*) buf, 
						(sb4) len, (ub2)SQLT_STR, (dvoid*) 0, 
						(ub2 *) 0, (ub2 *) 0, OCI_DEFAULT)))
						return FALSE;
				}
				break;
			case 'f':
				{
					if(!m_pConnPool->checkerr(conn->errorHandle,OCIDefineByPos(conn->stmtementHandle, &defnp1,
						conn->errorHandle,(ub4) ++i,(dvoid*)va_arg(par_str,float*), 
						(sb4) sizeof(float), (ub2)SQLT_INT, (dvoid*) 0, 
						(ub2 *) 0, (ub2 *) 0, OCI_DEFAULT)))
						return FALSE;
				}
				break;
			case 'd':
				{
					if(!m_pConnPool->checkerr(conn->errorHandle,OCIDefineByPos(conn->stmtementHandle, &defnp1,
						conn->errorHandle,(ub4) ++i,(dvoid*)va_arg(par_str,int*), 
						(sb4) sizeof(int), (ub2)SQLT_INT, (dvoid*) 0, 
						(ub2 *) 0, (ub2 *) 0, OCI_DEFAULT)))
						return FALSE;
				}
				break;
			}
		}
		p++;
	}
	va_end(par_str);

	if(!m_pConnPool->checkerr(conn->errorHandle,OCIStmtExecute(conn->serviceHandle, conn->stmtementHandle,
		conn->errorHandle, (ub4)1, (ub4)0,
		(OCISnapshot *) NULL, (OCISnapshot *) NULL,
		(ub4)OCI_DEFAULT)))
		return FALSE;

	return TRUE;
}

