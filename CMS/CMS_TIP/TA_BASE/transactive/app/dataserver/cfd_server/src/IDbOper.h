#ifndef __DEF_I_DB_OPER__HH__
#define __DEF_I_DB_OPER__HH__

#include "CFDServerCommonData.h"

#include <QtSql/QtSql>
#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>

#include "InstrumentBarInfoRequest.h"
#include "InstrumentBarInfo.h"
#include "InstrumentTickInfo.h"
#include "BarCalculator.h"

#include "DBOperParam.h"

NS_BEGIN(TA_Base_App) 

class IDbOper 
{
public:
	typedef std::list<CInstrumentBarInfo*>							  LstInstrumentBarInfoT;
	typedef std::list<CInstrumentBarInfo*>::iterator				  LstInstrumentBarInfoIterT;

	typedef std::list<CInstrumentTickInfo*>							  LstInstrumentTickInfoT;
	typedef std::list<CInstrumentTickInfo*>::iterator				  LstInstrumentTickInfoIterT;


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
	virtual int transactionInsertDataBar(unsigned int nTableKey, LstInstrumentBarInfoT* pListBar) = 0;
	virtual int transactionInsertDataTick(LstInstrumentTickInfoT* pListTick) = 0;

};

NS_END(TA_Base_App) 


#endif //__DEF_I_DB_OPER__HH__


