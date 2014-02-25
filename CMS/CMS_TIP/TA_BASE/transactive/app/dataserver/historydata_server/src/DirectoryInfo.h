#ifndef __CLASS_DIRECTORY_INFO_H__
#define __CLASS_DIRECTORY_INFO_H__


#include "HistoryDataServerCommonData.h"

#include <list>
#include <map>
#include <time.h>

NS_BEGIN(TA_Base_App)

class CFileInfo;

class CDirectoryInfo
{
public:
	typedef std::list<CFileInfo*>				LstFileSystemItemsInPathT;
	typedef std::list<CFileInfo*>::iterator   LstFileSystemItemsInPathIterT;

	typedef std::map<time_t, CFileInfo*>					MapTimeFileSystemItemT;
	typedef std::map<time_t, CFileInfo*>::iterator		MapTimeFileSystemItemIterT;
	typedef std::map<time_t, CFileInfo*>::value_type		MapTimeFileSystemItemValueTypeT;
public:
	CDirectoryInfo(void);	
	~CDirectoryInfo(void);

public:
	bool checkDirectory(const std::string& strFolderPath);
	int createDirectory(const std::string& strFolderPath);
	int getAllFileSystemItemInPath(const std::string& strFolderPath, MapTimeFileSystemItemT& mapTimeFileSystemItem);
	int removeOldFile(time_t nStarTime, MapTimeFileSystemItemT& mapTimeFileSystemItem );
	int freeData(MapTimeFileSystemItemT& mapTimeFileSystemItem );

private:
	int _GetAllFileNameInPath( const std::string& strFolderPath, LstFileSystemItemsInPathT& lstFileSystemItemsInPath);
	int _SortAllFileInPathByTime(LstFileSystemItemsInPathT& lstFileSystemItemsInPath, MapTimeFileSystemItemT& mapTimeFileSystemItem);
};


NS_END(TA_Base_App)


#endif // __CLASS_DIRECTORY_INFO_H__









