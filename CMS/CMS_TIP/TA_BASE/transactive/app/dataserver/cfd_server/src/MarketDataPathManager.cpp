#include "MarketDataPathManager.h"


#include "MarketDataFileManager.h"
#include "FileSystemItem.h"
#include "FileSystemManager.h"


#include "BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_App) 


static const int DEF_VALUE_INT_LoadFilesToMemoryMaxSize = 30;

//////////////////////////////////////////////////////////////////////////

CMarketDataPathManager::CMarketDataPathManager(void)
{
	m_pFileSystemManager = new CFileSystemManager();
	m_nLoadFilesToMemoryMaxSize = DEF_VALUE_INT_LoadFilesToMemoryMaxSize;
	m_nLoadFilesToMemoryMaxIndex = 0;
}

CMarketDataPathManager::~CMarketDataPathManager(void)
{
	BOOST_LOG_FUNCTION();

	

	if (NULL != m_pFileSystemManager)
	{
		delete m_pFileSystemManager;
		m_pFileSystemManager = NULL;
	}

}


int CMarketDataPathManager::analieAllFiles()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CFileSystemManager::MapTimeFileSystemItemT mapTimeFileSystemItemTmp;

	LOG_INFO<<"analies Directory ="<<m_InstrumentBarInfoRequest.m_strHistoryDataDirectory;
	m_pFileSystemManager->getAllFileSystemItemInPath(m_InstrumentBarInfoRequest.m_strHistoryDataDirectory, 
		mapTimeFileSystemItemTmp);
	m_pFileSystemManager->removeOldFile(m_InstrumentBarInfoRequest.m_nStartTime, mapTimeFileSystemItemTmp);

	_AnalieAllFilesTypeMarketData(mapTimeFileSystemItemTmp);


	m_pFileSystemManager->freeData(mapTimeFileSystemItemTmp);
	mapTimeFileSystemItemTmp.clear();


	return nFunRes;
}



int CMarketDataPathManager::_AnalieAllFilesTypeMarketData(CFileSystemManager::MapTimeFileSystemItemT& mapTimeFileSystemItemTmp)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CFileSystemManager::MapTimeFileSystemItemIterT iterMap;
	CFileSystemItem* pFileSystemItem = NULL;
	CMarketDataFileManager*  pMarketDataFileManager = NULL;



	iterMap = mapTimeFileSystemItemTmp.begin();
	while (iterMap != mapTimeFileSystemItemTmp.end())
	{
		pFileSystemItem = iterMap->second;

		m_InstrumentBarInfoRequest.m_strCurrentAnalierFileName = pFileSystemItem->getFileFullPath();

		m_nLoadFilesToMemoryMaxIndex++;
		if (m_nLoadFilesToMemoryMaxIndex > m_nLoadFilesToMemoryMaxSize)
		{
			if (NULL != pMarketDataFileManager)
			{
				LOG_INFO<<m_nLoadFilesToMemoryMaxIndex<<">"<<m_nLoadFilesToMemoryMaxSize<<"  reset";

				delete pMarketDataFileManager;
				pMarketDataFileManager = NULL;
			}
			m_nLoadFilesToMemoryMaxIndex = 0;
		}

		if (NULL == pMarketDataFileManager)
		{
			pMarketDataFileManager = new CMarketDataFileManager(CMarketDataFileManager::AnalierType_Dispatch_MarkketData);
		}


		pMarketDataFileManager->setInstrumentBarInfoRequest(m_InstrumentBarInfoRequest);
		nFunRes = pMarketDataFileManager->analierFile();

		iterMap++;
	}

	//free
	if (NULL != pMarketDataFileManager)
	{
		delete pMarketDataFileManager;
		pMarketDataFileManager = NULL;
	}

	return nFunRes;
}

int CMarketDataPathManager::setInstrumentBarInfoRequest(const CInstrumentBarInfoRequest& instrumentBarInfoRequest )
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	
	m_InstrumentBarInfoRequest = instrumentBarInfoRequest;

	m_nAnalierType = CMarketDataFileManager::AnalierType_Dispatch_MarkketData;

	_ProcessRequest();

	return nFunRes;
}



int CMarketDataPathManager::_ProcessRequest()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;

	LOG_INFO<<"check HisData Directory ="<<m_InstrumentBarInfoRequest.m_strHistoryDataDirectory;
	if (m_pFileSystemManager->checkDirectory(m_InstrumentBarInfoRequest.m_strHistoryDataDirectory))
	{
		//hisData Directory exists
		if (m_InstrumentBarInfoRequest.m_nDBType == enumSqliteDb)
		{
			LOG_INFO<<"check and create savedataBAR Directory ="<<m_InstrumentBarInfoRequest.m_strSaveDataDirectoryBAR;
			m_pFileSystemManager->createDirectory(m_InstrumentBarInfoRequest.m_strSaveDataDirectoryBAR);

			LOG_INFO<<"check and create savedataTIK Directory ="<<m_InstrumentBarInfoRequest.m_strSaveDataDirectoryTIK;
			m_pFileSystemManager->createDirectory(m_InstrumentBarInfoRequest.m_strSaveDataDirectoryTIK);

		}
	}
	else
	{
		nFunRes = -1;
		LOG_ERROR<<"HisData Directory="<<m_InstrumentBarInfoRequest.m_strHistoryDataDirectory<<" not exists!!";
	}

	return nFunRes;
}


NS_END(TA_Base_App) 



