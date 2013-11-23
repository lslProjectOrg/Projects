// SynchEnginer.cpp : Defines the entry point for the console application.
//
#include "vld.h"
#include <signal.h>
//
#include "CommonDef.h"
#include "CommonData.h"
#include "UtilityFun.h"

#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "ServerConnectionListener.h"
#include "core/threads/src/Thread.h"


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


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////


int main( int argc, char* argv[] )
{ 	   
	signal(SIGINT, usr_signal);
#ifndef WIN32
	signal(SIGHUP, usr_signal);	//close putty
#endif
	TA_Base_Core::RunParams::getInstance();
	TA_Base_App::CUtilityFun::getInstance();

	//bool bSet = TA_Base_Core::RunParams::getInstance().parseCmdLine(argc, argv);
	TA_Base_Core::RunParams::getInstance().set(RPARAM_DBCONNECTIONFILE,"./cfg/OCConnectionStrings.csv");
	//TA_Base_Core::DebugUtil::getInstance().setFile("./output/log_CFDServer.log");
	TA_Base_Core::DebugUtil::getInstance().setFile("log_CFDServer.log");
	TA_Base_Core::DebugUtil::getInstance().setMaxSize(2000000);
	TA_Base_Core::DebugUtil::getInstance().setLevel(TA_Base_Core::DebugUtil::DebugTrace);			 


	TA_Base_App::CServerConnectionListener::getInstance();
	std::string strListenPort = "5000";
	TA_Base_App::CServerConnectionListener::getInstance().setListenPort(strListenPort);


	while (true)
	{
		if (TA_Base_App::CServerConnectionListener::getInstance().isFinishWork())
		{
			break;
		}
		TA_Base_Core::Thread::sleep(TA_Base_App::DEF_INT_LongMonitorThreadSleep);
		break;
	}


	TA_Base_App::CServerConnectionListener::removeInstance();

	TA_Base_App::CUtilityFun::removeInstance();
	TA_Base_Core::DebugUtil::removeInstance();
	TA_Base_Core::RunParams::removeInstance();
	return 0;
}












