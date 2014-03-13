/*
manager MarketData history Data All Files in one Path. sort by time
*/
#ifndef __CLASS_MARKET_DATA_PATH_MANAGER__HH__
#define __CLASS_MARKET_DATA_PATH_MANAGER__HH__

#include "CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "InstrumentBarInfoRequest.h"
#include "MarketDataFileManager.h"
#include "FileSystemManager.h"

NS_BEGIN(TA_Base_App) 

class CMarketDataFileManager;


class CMarketDataPathManager
{
public:
	CMarketDataPathManager(void);
	~CMarketDataPathManager(void);
public:
	int setInstrumentBarInfoRequest(const CInstrumentBarInfoRequest& instrumentBarInfoRequest);

	int analieAllFiles();
private:
	int _AnalieAllFilesTypeMarketData(CFileSystemManager::MapTimeFileSystemItemT& mapTimeFileSystemItemTmp);
	int _ProcessRequest();
private:
	CFileSystemManager*    m_pFileSystemManager;
	CInstrumentBarInfoRequest m_InstrumentBarInfoRequest;
	CMarketDataFileManager::enAnalierType  m_nAnalierType;

private:
	unsigned int       m_nLoadFilesToMemoryMaxSize;
	unsigned int       m_nLoadFilesToMemoryMaxIndex;

};

NS_END(TA_Base_App) 


#endif //__CLASS_MARKET_DATA_PATH_MANAGER__HH__


