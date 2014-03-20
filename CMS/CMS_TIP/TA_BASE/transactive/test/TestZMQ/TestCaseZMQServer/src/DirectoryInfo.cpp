#include "DirectoryInfo.h"
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

#include "FileInfo.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CDirectoryInfo::CDirectoryInfo( void )
{
	BOOST_LOG_FUNCTION();

}

CDirectoryInfo::~CDirectoryInfo( void )
{
	BOOST_LOG_FUNCTION();

}

bool CDirectoryInfo::checkDirectory(const std::string& strFolderPath)
{
	bool bDirectoryExist = false;

	boost::filesystem::path fFullPath = boost::filesystem::system_complete(boost::filesystem::path(strFolderPath, boost::filesystem::native ));
	std::string strFullFolderPath = fFullPath.generic_string();

	if (boost::filesystem::exists(fFullPath) )
	{
		LOG_DEBUG<<"strFolderPath="<<strFolderPath<<" "<<"strFullFolderPath="<<strFullFolderPath<<" is exists";
		bDirectoryExist = true;
	}
	else
	{
		LOG_DEBUG<<"strFolderPath="<<strFolderPath<<" "<<"strFullFolderPath="<<strFullFolderPath<<" not exists";
		bDirectoryExist = false;
	}

	return bDirectoryExist;
}
int CDirectoryInfo::createDirectory(const std::string& strFolderPath)
{
	int nFunRes = 0;

	boost::filesystem::path fFullPath = boost::filesystem::system_complete(boost::filesystem::path(strFolderPath, boost::filesystem::native ));
	std::string strFullFolderPath = fFullPath.generic_string();

	if (!boost::filesystem::exists(fFullPath) )
	{
		boost::filesystem::create_directory(fFullPath);
	}

	if (boost::filesystem::exists(fFullPath) )
	{
		LOG_DEBUG<<"strFolderPath="<<strFolderPath<<" "<<"strFullFolderPath="<<strFullFolderPath<<" create directory ok!";
		nFunRes = 0;
	}
	else
	{
		LOG_ERROR<<"strFolderPath="<<strFolderPath<<" "<<"strFullFolderPath="<<strFullFolderPath<<" create directory error!";
		nFunRes = -1;
	}

	return nFunRes;
}

int CDirectoryInfo::getAllFileSystemItemInPath( const std::string& strFolderPath, MapTimeFileSystemItemT& mapTimeFileSystemItem )
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


int CDirectoryInfo::removeOldFile(time_t nStarTime, MapTimeFileSystemItemT& mapTimeFileSystemItem )
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	MapTimeFileSystemItemIterT iterMap;
	time_t nFileTime;
	CFileInfo* pFileSystemItem = NULL;

	iterMap = mapTimeFileSystemItem.begin();
	
	while (iterMap != mapTimeFileSystemItem.end())
	{
		nFileTime = iterMap->first;
		pFileSystemItem = iterMap->second;

		if (nFileTime <= nStarTime)
		{
			delete pFileSystemItem;
			pFileSystemItem = NULL;

			mapTimeFileSystemItem.erase(iterMap);
			iterMap = mapTimeFileSystemItem.begin();
		}
		else
		{
			iterMap++;
		}

	}

	return nFunRes;
}

int CDirectoryInfo::_GetAllFileNameInPath( const std::string& strFolderPath, LstFileSystemItemsInPathT& lstFileSystemItemsInPath )
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

				if (std::string::npos != strfileName.find(".zip")
					|| std::string::npos != strfileName.find(".7z")
					|| std::string::npos != strfileName.find(".tar")
					|| std::string::npos != strfileName.find(".bz2")
					|| std::string::npos != strfileName.find(".gz"))
				{
					//not process zip files
				} 
				else
				{
					CFileInfo* pFileSystemItem = NULL;
					pFileSystemItem = new CFileInfo(strFileFullPath);
					lstFileSystemItemsInPath.push_back(pFileSystemItem);
					pFileSystemItem = NULL;
				}//if

			}
		}//for
	}
	else
	{
		LOG_ERROR<<"error! Data Path not Find! strFilePathName="<<strFolderPath;
	}

	return nFunRes;
}


int CDirectoryInfo::_SortAllFileInPathByTime(LstFileSystemItemsInPathT& lstFileSystemItemsInPath, MapTimeFileSystemItemT& mapTimeFileSystemItem)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	LstFileSystemItemsInPathIterT   iterLst;
	CFileInfo* pFileSystemItem = NULL;
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


int CDirectoryInfo::freeData(MapTimeFileSystemItemT& mapTimeFileSystemItem )
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	CFileInfo* pFileSystemItem = NULL;
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
















