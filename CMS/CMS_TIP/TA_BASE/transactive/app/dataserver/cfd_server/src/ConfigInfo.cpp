#include "ConfigInfo.h"

#include "ConfigInfoHelper.h"

#include <boost/algorithm/string.hpp>

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


static const std::string DEF_VALUE_strConfigFileName = "./cfg/cfdServerConfigfile.cfg";
static const std::string DEF_VALUE_strHistoryDataDirectory = "C://TestData//HisData";
static const std::string DEF_VALUE_strSaveDataDirectoryBAR = "C://TestData//SaveDataBAR";
static const std::string DEF_VALUE_strSaveDataDirectoryTIK = "C://TestData//SaveDataTIK";
static const std::string DEF_VALUE_DbType = "SQLITE";//MYSQL
static const std::string DEF_VALUE_StartTime = "2013-01-01 10:00:00";//file time 2013-01-01 09:00:00

//hisdata=C://TestData//HisData
static const std::string DEF_VALUE_PARAM_HisDataDirectory = "hisdata";
//savedatabar=C://TestData//SaveDataBar
static const std::string DEF_VALUE_PARAM_SaveDataDirectoryBAR = "savedatabar";
//savedatatik=C://TestData//SaveDataTIK
static const std::string DEF_VALUE_PARAM_SaveDataDirectoryTIK = "savedatatik";
//defMysqlDBName  "MYSQL"  defSQLiteDBName "SQLITE"
static const std::string DEF_VALUE_PARAM_DbType = "dbtype";//dbtype=SQLITE
//YYYY-MM-DD HH:MM:SS  2013-01-01 10:00:00
static const std::string DEF_VALUE_PARAM_StartTime = "starttime";//dbtype=SQLITE


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
	m_strHistoryDataDirectory.clear();
	m_strSaveDataDirectoryBAR.clear();
	m_strSaveDataDirectoryTIK.clear();
	m_strStartTime.clear();

	m_strConfigFileName = DEF_VALUE_strConfigFileName;
	m_strHistoryDataDirectory = DEF_VALUE_strHistoryDataDirectory;
	m_strSaveDataDirectoryBAR = DEF_VALUE_strSaveDataDirectoryBAR;
	m_strSaveDataDirectoryTIK = DEF_VALUE_strSaveDataDirectoryTIK;
	m_strDbType = DEF_VALUE_DbType;
	m_strStartTime = DEF_VALUE_StartTime;
	_LoadDefaultConfigInfo();
}

CConfigInfo::~CConfigInfo(void)
{
	BOOST_LOG_FUNCTION();
}

void CConfigInfo::_LoadDefaultConfigInfo()
{
	BOOST_LOG_FUNCTION();
	std::string strParamValue;

	CConfigInfoHelper* pConfigInfoHelper = NULL;
	pConfigInfoHelper = new CConfigInfoHelper(m_strConfigFileName);


	strParamValue = pConfigInfoHelper->getStringParamValue(DEF_VALUE_PARAM_HisDataDirectory);
	if (!strParamValue.empty())
	{
		m_strHistoryDataDirectory = strParamValue;
	}

	strParamValue =pConfigInfoHelper->getStringParamValue(DEF_VALUE_PARAM_SaveDataDirectoryBAR);
	if (!strParamValue.empty())
	{
		m_strSaveDataDirectoryBAR = strParamValue;
	}	

	strParamValue =pConfigInfoHelper->getStringParamValue(DEF_VALUE_PARAM_SaveDataDirectoryTIK);
	if (!strParamValue.empty())
	{
		m_strSaveDataDirectoryTIK = strParamValue;
	}	

	strParamValue =pConfigInfoHelper->getStringParamValue(DEF_VALUE_PARAM_DbType);
	if (!strParamValue.empty())
	{
		m_strDbType = strParamValue;
		boost::algorithm::trim(m_strDbType);
		boost::algorithm::to_upper(m_strDbType);
	}	

	strParamValue =pConfigInfoHelper->getStringParamValue(DEF_VALUE_PARAM_StartTime);
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

std::string CConfigInfo::getHistoryDataDirectory()
{
	return m_strHistoryDataDirectory;
}

std::string CConfigInfo::getSaveDataDirectoryBAR()
{
	return m_strSaveDataDirectoryBAR;
}
std::string CConfigInfo::getSaveDataDirectoryTIK()
{
	return m_strSaveDataDirectoryTIK;
}

std::string CConfigInfo::getDbType()
{
	return m_strDbType;
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
















