// SynchEnginer.cpp : Defines the entry point for the console application.
//
#include "vld.h"
#include <signal.h>
//
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"




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
	bool bSet = TA_Base_Core::RunParams::getInstance().parseCmdLine(argc, argv);


	
	return 0;
}












