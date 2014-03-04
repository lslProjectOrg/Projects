#include "InstrumentBarInfoRequest.h"

#include "CFDServerUtilityFun.h"
#include <boost/algorithm/string.hpp>
#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CInstrumentBarInfoRequest::CInstrumentBarInfoRequest(void)
{
	BOOST_LOG_FUNCTION();

	m_strHistoryDataDirectory.clear();
	m_strSaveDataDirectoryBAR.clear();
	m_strSaveDataDirectoryTIK.clear();
	m_strCurrentAnalierFileName.clear();
	m_strDbType.clear();
	m_nDBType = enumSqliteDb;
	m_strStartTime.clear();
	m_nStartTime = 0;

	m_lstBarTime.clear();
	_AddBarTimeNormal();
}

CInstrumentBarInfoRequest::~CInstrumentBarInfoRequest(void)
{
	BOOST_LOG_FUNCTION();
	m_lstBarTime.clear();
}


CInstrumentBarInfoRequest& CInstrumentBarInfoRequest::operator=(const CInstrumentBarInfoRequest& instrumentBarInfoRequest )
{
	BOOST_LOG_FUNCTION();
	m_strCurrentAnalierFileName = instrumentBarInfoRequest.m_strCurrentAnalierFileName;
	m_strHistoryDataDirectory = instrumentBarInfoRequest.m_strHistoryDataDirectory;
	m_strSaveDataDirectoryBAR = instrumentBarInfoRequest.m_strSaveDataDirectoryBAR;
	m_strSaveDataDirectoryTIK = instrumentBarInfoRequest.m_strSaveDataDirectoryTIK;

	m_strDbType = instrumentBarInfoRequest.m_strDbType;
	m_nDBType = instrumentBarInfoRequest.m_nDBType;
	m_strStartTime = instrumentBarInfoRequest.m_strStartTime;
	m_nStartTime = instrumentBarInfoRequest.m_nStartTime;
	m_lstBarTime.clear();
	_AddBarTimeNormal();
	return *this;
}

void CInstrumentBarInfoRequest::logInfo()
{
	BOOST_LOG_FUNCTION();

	LOG_INFO<<"m_strHistoryDataDirectory="<<m_strHistoryDataDirectory;
	LOG_INFO<<"m_strSaveDataDirectoryBAR="<<m_strSaveDataDirectoryBAR;
	LOG_INFO<<"m_strSaveDataDirectoryTIK="<<m_strSaveDataDirectoryTIK;

	LOG_INFO<<"m_strDbType="<<m_strDbType;
	LOG_INFO<<"m_nDBType="<<m_nDBType;
	LOG_INFO<<"m_sttStartTime="<<m_strStartTime;

	LOG_INFO<<"Calculator TIME_BASE_S_5S="<<TA_Base_App::TIME_BASE_S_5S;
	LOG_INFO<<"Calculator TIME_BASE_S_1MIN="<<TA_Base_App::TIME_BASE_S_1MIN;
	LOG_INFO<<"Calculator TIME_BASE_S_5MIN="<<TA_Base_App::TIME_BASE_S_5MIN;
	LOG_INFO<<"Calculator TIME_BASE_S_15MIN="<<TA_Base_App::TIME_BASE_S_15MIN;
	LOG_INFO<<"Calculator TIME_BASE_S_30MIN="<<TA_Base_App::TIME_BASE_S_30MIN;
	LOG_INFO<<"Calculator TIME_BASE_S_1HOUR="<<TA_Base_App::TIME_BASE_S_1HOUR;
	LOG_INFO<<"Calculator TIME_BASE_S_1DAY="<<TA_Base_App::TIME_BASE_S_1DAY;
	LOG_INFO<<"Calculator TIME_BASE_S_1MON="<<TA_Base_App::TIME_BASE_S_1MON;
	LOG_INFO<<"Calculator TIME_BASE_S_1YEAR="<<TA_Base_App::TIME_BASE_S_1YEAR;

	return;
}

void CInstrumentBarInfoRequest::_AddBarTimeNormal()
{
	BOOST_LOG_FUNCTION();

	m_lstBarTime.push_back(TA_Base_App::TIME_BASE_S_5S);//seconds
	m_lstBarTime.push_back(TA_Base_App::TIME_BASE_S_1MIN);//seconds
	m_lstBarTime.push_back(TA_Base_App::TIME_BASE_S_5MIN);//seconds
	m_lstBarTime.push_back(TA_Base_App::TIME_BASE_S_15MIN);//seconds
	m_lstBarTime.push_back(TA_Base_App::TIME_BASE_S_30MIN);//seconds
	m_lstBarTime.push_back(TA_Base_App::TIME_BASE_S_1HOUR);//seconds
	m_lstBarTime.push_back(TA_Base_App::TIME_BASE_S_1DAY);//seconds
	m_lstBarTime.push_back(TA_Base_App::TIME_BASE_S_1MON);//seconds
	m_lstBarTime.push_back(TA_Base_App::TIME_BASE_S_1YEAR);//seconds

	return;
}





void CInstrumentBarInfoRequest::getLstBarTime( std::list<int>& lstBarTime )
{
	std::list<int>::iterator iterList;
	iterList = m_lstBarTime.begin();
	while (iterList != m_lstBarTime.end())
	{
		int nSeconds = *iterList;
		lstBarTime.push_back(nSeconds);
		iterList++;
	}
}

void CInstrumentBarInfoRequest::initData()
{
	CCFDServerUtilityFun* pUtilityFun = new CCFDServerUtilityFun();
	boost::algorithm::trim(m_strDbType);
	boost::algorithm::to_upper(m_strDbType);
	//defSQLiteDBName defMysqlDBName
	if (std::string::npos != m_strDbType.find(defSQLiteDBName))
	{
		m_strDbType = defSQLiteDBName;
		m_nDBType = TA_Base_App::enumSqliteDb;
	}
	else if (std::string::npos != m_strDbType.find(defMysqlDBName))
	{
		m_strDbType = defMysqlDBName;
		//qt not mysql driver need complie
		m_nDBType = enumMysqlDb;
	}
	else
	{
		m_strDbType = defSQLiteDBName;
		m_nDBType = TA_Base_App::enumSqliteDb;
	}


	m_nStartTime = pUtilityFun->strToDateTime(m_strStartTime);

	if (NULL != pUtilityFun)
	{
		delete pUtilityFun;
		pUtilityFun = NULL;
	}
}

NS_END(TA_Base_App)
















