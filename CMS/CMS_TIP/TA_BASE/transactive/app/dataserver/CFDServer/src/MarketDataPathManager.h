/*
manager MarketData history Data All Files in one Path. sort by time
*/
#ifndef __CLASS_MARKET_DATA_PATH_MANAGER__HH__
#define __CLASS_MARKET_DATA_PATH_MANAGER__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "InstrumentBarInfoRequest.h"
#include "InstrumentMarketDataRequest.h"
#include "MarketDataFileManager.h"

NS_BEGIN(TA_Base_App) 

class CMarketDataFileManager;

class CFileSystemManager;

class CMarketDataPathManager
{
public:
	CMarketDataPathManager(void);
	~CMarketDataPathManager(void);
public:
	int setInstrumentBarInfoRequest(const CInstrumentBarInfoRequest& instrumentBarInfoRequest);
	int setInstrumentMarketDataRequest(const CInstrumentMarketDataRequest& instrumentMarketDataRequest);

	int analieAllFiles();
private:
	int _SetAnalieType(CMarketDataFileManager::enAnalierType nAnalierType);
	int _AnalieAllFilesTypeMarketDataLine();
	int _AnalieAllFilesTypeMarketData();
private:
	CMarketDataFileManager*  m_pMarketDataFileManager;
	CFileSystemManager*    m_pFileSystemManager;
	CInstrumentBarInfoRequest m_InstrumentBarInfoRequest;
	CInstrumentMarketDataRequest m_InstrumentMarketDataRequest;
	CMarketDataFileManager::enAnalierType  m_nAnalierType;
};

NS_END(TA_Base_App) 


#endif //__CLASS_MARKET_DATA_PATH_MANAGER__HH__


