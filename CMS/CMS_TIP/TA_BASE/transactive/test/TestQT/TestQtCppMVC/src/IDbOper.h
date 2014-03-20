#ifndef __DEF_I_DB_OPER__HH__
#define __DEF_I_DB_OPER__HH__

#include "ProjectCommonData.h"

#include <QtSql/QtSql>
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>


#include "DBOperParam.h"

NS_BEGIN(TA_Base_App) 

class IDbOper 
{
public:
	IDbOper(void)
	{

	}

	virtual ~IDbOper(void) 
	{
		return;
	}
public:
	virtual int checkAndInitDBTable(std::list<int>& lstTableKey) = 0;

};

NS_END(TA_Base_App) 


#endif //__DEF_I_DB_OPER__HH__


