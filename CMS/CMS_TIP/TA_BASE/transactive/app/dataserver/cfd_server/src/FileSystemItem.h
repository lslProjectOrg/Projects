#ifndef __CLASS_FILE_SYSTEM_ITEM_H__
#define __CLASS_FILE_SYSTEM_ITEM_H__ 


#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/filesystem.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/exception.hpp"
#include "boost/tokenizer.hpp"

NS_BEGIN(TA_Base_App)

class CCFDServerUtilityFun;

class CFileSystemItem
{
public:
	CFileSystemItem(const std::string& strFilePath);	
	~CFileSystemItem(void);

public:
	time_t getFileNameTime();
	std::string getFileFullPath();
private:
	std::string _GetFileNameTimeStrValue(const std::string& strFileName);

private:
	boost::filesystem::path m_fileSystemPath;
private:
	std::string m_strFileFullPath;//C://Sample//20121220.csv
	std::string m_strFullFileName;//20121220.csv
	std::string m_strFileName;//20121220
	std::string m_strFileExten;//csv
	std::string m_strFileNameTimeStrValue;//2012-12-20 09:00:00
	time_t      m_nFileNameTimeIntValue;

private:
	CCFDServerUtilityFun*			m_pUtilityFun;

};


NS_END(TA_Base_App)


#endif // __CLASS_FILE_SYSTEM_ITEM_H__









