// SynchEnginer.cpp : Defines the entry point for the console application.
//
#include "vld.h"
#include <signal.h>

#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"


#include "core/database/src/IDatabase.h"
#include "core/database/src/DatabaseFactory.h"
//#include "core/exceptions/src/DbSyncException.h"


bool g_b_main_continue = true;
void usr_signal(int SigNo)
{
	if (SigNo == SIGINT)
	{
		g_b_main_continue = false;
	}
	
#ifndef WIN32
	if (SigNo == SIGHUP)
	{
	}
#endif


}



int main( int argc, char* argv[] )
{ 	   
	signal(SIGINT, usr_signal);
#ifndef WIN32
	signal(SIGHUP, usr_signal);	//close putty
#endif

	TA_Base_Core::DebugUtil::getInstance();
	bool bSet = TA_Base_Core::RunParams::getInstance().parseCmdLine(argc, argv);

	

	TA_Base_Core::RunParams::getInstance().set(RPARAM_DBCONNECTIONFILE,"./OCConnectionStrings.csv");
	TA_Base_Core::DebugUtil::getInstance().setFile("log_dbSynch.log");
	TA_Base_Core::DebugUtil::getInstance().setMaxSize(2000000);
	TA_Base_Core::DebugUtil::getInstance().setLevel(TA_Base_Core::DebugUtil::DebugSQL);	

	//Event_Ad,Read,Sqlite,tableTest,noUser,noPWD,127.0.0.1,Mysql,tra_occ,IT271350_5,IT271350_5,192.168.123.43,
	//Oracle,TRANSACT,IT271350_5,IT271350_5,192.168.123.43
 	//
	//TA_Base_Core::RunParams::getInstance().set("DataBase_tableTest@127.0.0.1",RPARAM_DBONLINE); 	 
	//TA_Base_Core::RunParams::getInstance().set("DataBase_tra_occ@192.168.123.43",RPARAM_DBOFFLINE); 	 
	TA_Base_Core::RunParams::getInstance().set("DataBase_TRANSACT@192.168.123.43",RPARAM_DBONLINE); 	 

	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "error");



	TA_Base_Core::EDataTypes TestEDataType = TA_Base_Core::Event_Ad;
	TA_Base_Core::EDataActions TestEDataAction = TA_Base_Core::Read;
	TA_Base_Core::IDatabase* pDatabase = NULL;
	TA_Base_Core::IData* pData = NULL;
	unsigned long ulongPkeyValue = 0;

	pDatabase = TA_Base_Core::DatabaseFactory::getInstance().getDatabase(TestEDataType, TestEDataAction);

	TA_Base_Core::SQLStatement rSqlObj;
	std::vector<std::string> columnNames;

	columnNames.push_back("PKEY");
	rSqlObj.strCommonSQL = "select PKEY from recipient_info";

	pData = pDatabase->executeQuery(rSqlObj, columnNames);
	ulongPkeyValue = pData->getUnsignedLongData(0, "PKEY");


	TA_Base_Core::DatabaseFactory::removeInstance();

	TA_Base_Core::DebugUtil::removeInstance();
	TA_Base_Core::RunParams::removeInstance();

	return 0;
}

