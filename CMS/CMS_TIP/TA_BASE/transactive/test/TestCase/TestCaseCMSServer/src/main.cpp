#include "CommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>


#include "TestCaseForCS.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

boost::mutex g_mutexMainRun;
boost::condition_variable g_conditionMainRun;



int main( int argc, char* argv[] )
{
	TA_Base_Test::CBoostLogger BoostLogger;
	BOOST_LOG_FUNCTION();


	TA_Base_Test::TestCaseForCS* pTEST_TestCaseCS = new TA_Base_Test::TestCaseForCS();
	pTEST_TestCaseCS->runTestCase_ForServer();
	//pTEST_TestCaseCS->runTestCase_ForClient();

	//sleep
	{	
		boost::mutex::scoped_lock lock(g_mutexMainRun);
		g_conditionMainRun.wait(lock);
	}

	delete pTEST_TestCaseCS;
	pTEST_TestCaseCS = NULL;


	return 0;
}










