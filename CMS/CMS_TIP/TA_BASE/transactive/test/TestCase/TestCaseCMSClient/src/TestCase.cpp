#include "TestCase.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>


#include "CommonData.h"
#include "SysTypes.h"
#include "ServerManage.h"
#include "ClientsManagerWorker.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

namespace boost 
{
	class thread;
	class thread_group;
}


// extern boost::condition_variable g_conditionMainRun;

NS_BEGIN(TA_Base_Test) 
std::string g_string_strServerAddress = "tcp://127.0.0.1:5555";
int g_n_ClientNUM = 500;






//////////////////////////////////////////////////////////////////////////

CTestCase::CTestCase(void)
{

}

CTestCase::~CTestCase(void)
{

}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void CTestCase::runTestCase_ForClient()
{
	BOOST_LOG_FUNCTION();
	CClientsManagerWorker* pClientsManagerWorker = new CClientsManagerWorker();
	pClientsManagerWorker->start();
}


void CTestCase::runTestCase_ForServer()
{
	BOOST_LOG_FUNCTION();
	CServerManager::getInstance();

}


NS_END(TA_Base_Test) 



