#ifndef __CLASS_FILE_SYSTEM_MANAGER_H__
#define __CLASS_FILE_SYSTEM_MANAGER_H__ 


#include "CFDServerCommonData.h"

#include <list>
#include <map>
#include <time.h>

NS_BEGIN(TA_Base_App)

class CFileSystemItem;

class CFileSystemManager
{
public:
	typedef std::list<CFileSystemItem*>				LstFileSystemItemsInPathT;
	typedef std::list<CFileSystemItem*>::iterator   LstFileSystemItemsInPathIterT;

	typedef std::map<time_t, CFileSystemItem*>					MapTimeFileSystemItemT;
	typedef std::map<time_t, CFileSystemItem*>::iterator		MapTimeFileSystemItemIterT;
	typedef std::map<time_t, CFileSystemItem*>::value_type		MapTimeFileSystemItemValueTypeT;
public:
	CFileSystemManager(void);	
	~CFileSystemManager(void);

public:
	int getAllFileSystemItemInPath(const std::string& strFolderPath, MapTimeFileSystemItemT& mapTimeFileSystemItem);
	int freeData(MapTimeFileSystemItemT& mapTimeFileSystemItem );

private:
	int _GetAllFileNameInPath( const std::string& strFolderPath, LstFileSystemItemsInPathT& lstFileSystemItemsInPath);
	int _SortAllFileInPathByTime(LstFileSystemItemsInPathT& lstFileSystemItemsInPath, MapTimeFileSystemItemT& mapTimeFileSystemItem);
};


NS_END(TA_Base_App)


#endif // __CLASS_FILE_SYSTEM_MANAGER_H__









