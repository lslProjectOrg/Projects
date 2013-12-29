#include "MarketDataPathManager.h"


#include "MarketDataFileManager.h"
#include "FileSystemItem.h"
#include "FileSystemManager.h"


#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////

CMarketDataPathManager::CMarketDataPathManager(void)
{
	m_strFolderPath.clear();
	m_pMarketDataFileManager = new CMarketDataFileManager();
	m_pFileSystemManager = new CFileSystemManager();

}

CMarketDataPathManager::~CMarketDataPathManager(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pMarketDataFileManager)
	{
		delete m_pMarketDataFileManager;
		m_pMarketDataFileManager = NULL;
	}

	if (NULL != m_pFileSystemManager)
	{
		delete m_pFileSystemManager;
		m_pFileSystemManager = NULL;
	}

}
int CMarketDataPathManager::setPathName(const std::string& strFolderPath)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	m_strFolderPath = strFolderPath;
	return nFunRes;
}

int CMarketDataPathManager::analieAllFiles()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CFileSystemManager::MapTimeFileSystemItemT mapTimeFileSystemItemTmp;
	CFileSystemManager::MapTimeFileSystemItemIterT iterMap;
	CFileSystemItem* pFileSystemItem = NULL;
	std::string strOneFileName;

	LOG_DEBUG<<"analies Folder m_strFolderPath="<<m_strFolderPath;

	m_pFileSystemManager->getAllFileSystemItemInPath(m_strFolderPath, mapTimeFileSystemItemTmp);

	iterMap = mapTimeFileSystemItemTmp.begin();
	while (iterMap != mapTimeFileSystemItemTmp.end())
	{
		pFileSystemItem = iterMap->second;

		//std::string strOneFileName = "C:\\LSL\\SVNWork\\CMS\\PUBLIC\\MarketData\\sample\\sample.csv";
		strOneFileName = pFileSystemItem->getFileFullPath();
		m_pMarketDataFileManager->setFileName(strOneFileName);
		nFunRes = m_pMarketDataFileManager->analierFile();

		iterMap++;
	}


	m_pFileSystemManager->freeData(mapTimeFileSystemItemTmp);
	mapTimeFileSystemItemTmp.clear();

	return nFunRes;
}





NS_END(TA_Base_App) 



