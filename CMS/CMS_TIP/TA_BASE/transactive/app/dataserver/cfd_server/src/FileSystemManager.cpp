#include "FileSystemManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <list> 
#include <time.h>

#include <boost/filesystem.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/exception.hpp"
#include "boost/tokenizer.hpp"

#include "FileSystemItem.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CFileSystemManager::CFileSystemManager( void )
{
	BOOST_LOG_FUNCTION();

}

CFileSystemManager::~CFileSystemManager( void )
{
	BOOST_LOG_FUNCTION();

}


int CFileSystemManager::getAllFileSystemItemInPath( const std::string& strFolderPath, MapTimeFileSystemItemT& mapTimeFileSystemItem )
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	LstFileSystemItemsInPathT lstFileSystemItemsInPathTmp;

	//get all
	_GetAllFileNameInPath(strFolderPath, lstFileSystemItemsInPathTmp);

	//sort
	_SortAllFileInPathByTime(lstFileSystemItemsInPathTmp, mapTimeFileSystemItem);
	lstFileSystemItemsInPathTmp.clear();

	return nFunRes;
}


int CFileSystemManager::_GetAllFileNameInPath( const std::string& strFolderPath, LstFileSystemItemsInPathT& lstFileSystemItemsInPath )
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	boost::filesystem::path fFullPath = boost::filesystem::system_complete(boost::filesystem::path(strFolderPath, boost::filesystem::native ));
	std::string strFullFolderPath = fFullPath.generic_string();
	LOG_DEBUG<<"strFolderPath="<<strFolderPath<<" "<<"strFullFolderPath="<<strFullFolderPath;

	if ( boost::filesystem::exists(fFullPath) )
	{
		boost::filesystem::directory_iterator item_begin(fFullPath);
		boost::filesystem::directory_iterator item_end;

		for (; item_begin != item_end; item_begin++)
		{
			if (boost::filesystem::is_directory(*item_begin))
			{
				std::string strSubDir = item_begin->path().generic_string();
				strSubDir.append("//");
				//boost::filesystem::path sub_dir(strSubDir);
				_GetAllFileNameInPath(strSubDir, lstFileSystemItemsInPath);
			}
			else
			{	
				std::string strFileFullPath = item_begin->path().generic_string();//C://Sample//20121220.csv
				//boost::filesystem::directory_entry
				std::string strfileName = item_begin->path().filename().generic_string();//20121220.csv

				CFileSystemItem* pFileSystemItem = NULL;
				pFileSystemItem = new CFileSystemItem(strFileFullPath);
				lstFileSystemItemsInPath.push_back(pFileSystemItem);
				pFileSystemItem = NULL;
			}
		}//for
	}
	else
	{
		LOG_ERROR<<"error! Data Path not Find! strFilePathName="<<strFolderPath;
	}

	return nFunRes;
}


int CFileSystemManager::_SortAllFileInPathByTime(LstFileSystemItemsInPathT& lstFileSystemItemsInPath, MapTimeFileSystemItemT& mapTimeFileSystemItem)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	LstFileSystemItemsInPathIterT   iterLst;
	CFileSystemItem* pFileSystemItem = NULL;
	time_t nFileNameTImeValue = 0;

	iterLst = lstFileSystemItemsInPath.begin();
	while (iterLst != lstFileSystemItemsInPath.end())
	{
		pFileSystemItem = (*iterLst);
		nFileNameTImeValue = pFileSystemItem->getFileNameTime();

		mapTimeFileSystemItem.insert(MapTimeFileSystemItemValueTypeT(nFileNameTImeValue, pFileSystemItem));//auto sort

		iterLst++;
	}//while
	lstFileSystemItemsInPath.clear();

	return nFunRes;
}


int CFileSystemManager::freeData(MapTimeFileSystemItemT& mapTimeFileSystemItem )
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CFileSystemItem* pFileSystemItem = NULL;
	MapTimeFileSystemItemIterT iterMap;
	iterMap = mapTimeFileSystemItem.begin();
	while (iterMap != mapTimeFileSystemItem.end())
	{
		pFileSystemItem = (iterMap->second);

		delete pFileSystemItem;
		pFileSystemItem = NULL;

		iterMap++;
	}
	mapTimeFileSystemItem.clear();

	return nFunRes;
}

NS_END(TA_Base_App)
















