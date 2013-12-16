#include "CommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>


#include "TestCase.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

boost::mutex g_mutexMainRun;
boost::condition_variable g_conditionMainRun;

int g_runAsServer = 0;
int g_runAsClient = 1;

int main( int argc, char* argv[] )
{
	TA_Base_Test::CBoostLogger BoostLogger;
	BOOST_LOG_FUNCTION();


	TA_Base_Test::CTestCase* pTestCase = new TA_Base_Test::CTestCase();

	if (1 == g_runAsServer)
	{
		pTestCase->runTestCase_ForServer();
	}
	else
	{
		pTestCase->runTestCase_ForClient();
	}
	//

	//sleep
	{	
		boost::mutex::scoped_lock lock(g_mutexMainRun);
		g_conditionMainRun.wait(lock);
	}

	delete pTestCase;
	pTestCase = NULL;


	return 0;
}










