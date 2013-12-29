/*
manager MarketData history Data in one File
Analie each line  to MarketData class
*/
#ifndef __CLASS_MARKET_DATA_FILE_ANALIER__HH__
#define __CLASS_MARKET_DATA_FILE_ANALIER__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>



NS_BEGIN(TA_Base_App) 

class CMarketDataDispatcher;
class CCFDServerUtilityFun;

class CMarketDataFileAnalier 
{
public:
	CMarketDataFileAnalier();
	~CMarketDataFileAnalier(void);

public:
	void setFileName(const std::string& strFileName);
	int analierFile();

private:
	std::string m_strFileName;
	CMarketDataDispatcher*			m_pMarketDataDispatcher;
	CCFDServerUtilityFun*           m_pUtilityFun;

};

NS_END(TA_Base_App) 


#endif //__CLASS_MARKET_DATA_FILE_ANALIER__HH__


