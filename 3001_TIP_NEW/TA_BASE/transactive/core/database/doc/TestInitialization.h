#include "core/utilities/src/RunParams.h"
#include "core/uuid/src/TAuuid.h"
#include "core/utilities//src/DebugUtil.h"
#include "core/utilities/src/TA_String.h"



#pragma  once;
class TestInitialization 
{
public:
	// 	TestInitialization* getInstance()
	// 	{
	// 
	// 	}

	static void Init(std::string DBtype)
	{
		Init();

		if (DBtype=="MYSQL")
		{	 
			activeDB1();
		}
		else
		{
			activeDB2();
		}
	}

	static void Init()
	{
		if (m_isInitialized==false)
		{
			TA_Base_Core::RunParams::getInstance().set(RPARAM_DBCONNECTIONFILE,"OCConnectionStrings.csv");
			activeDB1();
			TA_Base_Core::DebugUtil::getInstance().setFile("log_dai.log");
			TA_Base_Core::DebugUtil::getInstance().setMaxSize(2000000);

			TA_Base_Core::DebugUtil::getInstance().setLevel(TA_Base_Core::DebugUtil::DebugSQL);			 
			m_isInitialized=true;
		}
	}

	static void activeDB1() //MYSQL
	{	 		 
		TA_Base_Core::RunParams::getInstance().set("DataBase_tra_occ@192.168.123.43",RPARAM_DBONLINE); 	 
		TA_Base_Core::RunParams::getInstance().set("DataBase_TRANSACT",RPARAM_DBOFFLINE); 	 
	}

	static void activeDB2() //ORACLE
	{			
		TA_Base_Core::RunParams::getInstance().set("DataBase_tra_occ@192.168.123.43",RPARAM_DBOFFLINE); 	 
		TA_Base_Core::RunParams::getInstance().set("DataBase_TRANSACT",RPARAM_DBONLINE); 	 
	}
	virtual ~TestInitialization();
private:
	TestInitialization();

	static bool m_isInitialized;


};


__declspec(selectany) bool TestInitialization::m_isInitialized = false;