/*
manager MarketData history Data in one File
*/
#ifndef __CLASS_MARKET_DATA_FILE_MANAGER__HH__
#define __CLASS_MARKET_DATA_FILE_MANAGER__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>



NS_BEGIN(TA_Base_App) 

class CMarketDataFileAnalier;

class CMarketDataFileManager 
{
public:
	CMarketDataFileManager();
	~CMarketDataFileManager(void);
public:
	void setFileName(const std::string& strFileName);
	int analierFile();
private:
	std::string m_strFileName;
	CMarketDataFileAnalier*			m_pMarketDataFileAnalier;
};

NS_END(TA_Base_App) 


#endif //__CLASS_MARKET_DATA_FILE_MANAGER__HH__


