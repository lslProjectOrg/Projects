#include "DataAccessTestManage.h"


#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "DataAccessTestWorker.h"



#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_Test) 


//////////////////////////////////////////////////////////////////////////
CDataAccessTestManager::CDataAccessTestManager(void)
{	
	BOOST_LOG_FUNCTION();	
	m_pWorker = NULL;
	m_pWorker = new CDataAccesssTestWorker();
	m_pWorker->start();
	
}

CDataAccessTestManager::~CDataAccessTestManager(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pWorker)
	{
		m_pWorker->terminateAndWait();
		delete m_pWorker;
		m_pWorker = NULL;
	}


}



NS_END(TA_Base_Test) 



