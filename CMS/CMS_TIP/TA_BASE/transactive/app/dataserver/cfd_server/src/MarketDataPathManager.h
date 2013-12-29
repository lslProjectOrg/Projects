/*
manager MarketData history Data All Files in one Path. sort by time
*/
#ifndef __CLASS_MARKET_DATA_PATH_MANAGER__HH__
#define __CLASS_MARKET_DATA_PATH_MANAGER__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>



NS_BEGIN(TA_Base_App) 

class CMarketDataFileManager;

class CMarketDataPathManager
{
public:
	CMarketDataPathManager(void);
	~CMarketDataPathManager(void);
public:
	int setPathName(const std::string& strFilePathName);
	int analieAllFiles();
private:
	std::string m_strFilePathName;
	CMarketDataFileManager*  m_pMarketDataFileManager;
};

NS_END(TA_Base_App) 


#endif //__CLASS_MARKET_DATA_PATH_MANAGER__HH__


