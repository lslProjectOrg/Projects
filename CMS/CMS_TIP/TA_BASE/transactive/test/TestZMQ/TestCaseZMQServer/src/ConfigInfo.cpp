#include "ConfigInfo.h"

#include "ConfigInfoHelper.h"

#include <boost/algorithm/string.hpp>

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


static const std::string DEF_VALUE_strConfigFileName = "./cfg/HistoryDataServerConfigfile.cfg";

static const std::string DEF_VALUE_StartTime = "2013-01-01 10:00:00";//file time 2013-01-01 09:00:00

static const std::string DEF_VALUE_PARAM_StartTime = "starttime";//starttime=2013-01-01 10:00:00


CConfigInfo* CConfigInfo::m_pInstance = 0;
boost::mutex CConfigInfo::m_mutexInstance;

CConfigInfo& CConfigInfo::getInstance()
{	
	boost::mutex::scoped_lock lock(m_mutexInstance);	
	if (NULL == m_pInstance)
	{
		m_pInstance = new CConfigInfo();
	}
	return (*m_pInstance);
}

void CConfigInfo::removeInstance()
{
	boost::mutex::scoped_lock lock(m_mutexInstance);	
	delete m_pInstance;
	m_pInstance = NULL;

}



CConfigInfo::CConfigInfo(void)
{
	BOOST_LOG_FUNCTION();
	m_strConfigFileName.clear();


	m_strConfigFileName = DEF_VALUE_strConfigFileName;
	loadDefaultConfigInfo();
}

CConfigInfo::~CConfigInfo(void)
{
	BOOST_LOG_FUNCTION();
}

void CConfigInfo::loadDefaultConfigInfo()
{
	BOOST_LOG_FUNCTION();
	std::string strParamValue;

	CConfigInfoHelper* pConfigInfoHelper = NULL;
	pConfigInfoHelper = new CConfigInfoHelper(m_strConfigFileName);


	strParamValue = pConfigInfoHelper->getStringParamValue(DEF_VALUE_PARAM_StartTime);
	if (!strParamValue.empty())
	{
		m_strStartTime = strParamValue;
		boost::algorithm::trim(m_strStartTime);
	}	


	if (NULL != pConfigInfoHelper)
	{
		delete pConfigInfoHelper;
		pConfigInfoHelper = NULL;
	}
}

std::string CConfigInfo::getStartTime()
{
	return m_strStartTime;
}


void CConfigInfo::logInfo()
{
	BOOST_LOG_FUNCTION();

	return;
}


NS_END(TA_Base_App)
















