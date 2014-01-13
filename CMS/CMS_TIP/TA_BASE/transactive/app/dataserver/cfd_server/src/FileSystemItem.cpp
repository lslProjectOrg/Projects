#include "FileSystemItem.h"
#include "CFDServerUtilityFun.h"
#include <sstream>

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CFileSystemItem::CFileSystemItem( const std::string& strFilePath )
{
	BOOST_LOG_FUNCTION();
	m_pUtilityFun = new CCFDServerUtilityFun();

	boost::filesystem::path fFileFullPath = boost::filesystem::system_complete(boost::filesystem::path(strFilePath, boost::filesystem::native ));
	m_strFileFullPath = fFileFullPath.generic_string();//C://Sample//20121220.csv
	m_strFullFileName = fFileFullPath.filename().generic_string();//20121220.csv
	m_strFileName = fFileFullPath.stem().generic_string();//20121220
	m_strFileExten = fFileFullPath.extension().generic_string();//.csv

	m_strFileNameTimeStrValue = _GetFileNameTimeStrValue(m_strFileName);//2012-12-10 09:00:00
	m_nFileNameTimeIntValue = m_pUtilityFun->strToDateTime(m_strFileNameTimeStrValue);//2012-12-10 09:00:00
}

CFileSystemItem::~CFileSystemItem( void )
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}

}

time_t CFileSystemItem::getFileNameTime()
{
	BOOST_LOG_FUNCTION();

	return m_nFileNameTimeIntValue;
}

std::string CFileSystemItem::getFileFullPath()
{
	BOOST_LOG_FUNCTION();

	return m_strFileFullPath;
}

std::string CFileSystemItem::_GetFileNameTimeStrValue(const std::string& strFileName)
{
	BOOST_LOG_FUNCTION();
	//20121210
	//2012-12-10 09:00:00
	std::string strFileNameTmp = strFileName;
	std::string strYear;
	std::string strMonth;
	std::string strDay;
	std::string strHour;
	std::string strMinute;
	std::string strSecond;
	std::ostringstream sreaamTmp;
	std::string strFileNameTimeStrValueRes;
	int numberTest = 0;

	numberTest = strFileNameTmp.find_first_not_of("0123456789");
	if ((8 != strFileNameTmp.size()) || (-1 != numberTest))
	{
		LOG_ERROR<<"error file Name strFileName="<<strFileName;
		strFileNameTimeStrValueRes.clear();
		return strFileNameTimeStrValueRes;
	}

	strYear = strFileName.substr(0, 4);//2012
	strMonth = strFileName.substr(4, 2);//12
	strDay = strFileName.substr(6, 2);//12
	strHour = "09";
	strMinute = "00";
	strSecond = "00";

	sreaamTmp.str("");
	sreaamTmp<<strYear<<"-"<<strMonth<<"-"<<strDay<<" "<<strHour<<":"<<strMinute<<":"<<strSecond;
	strFileNameTimeStrValueRes = sreaamTmp.str();

	return strFileNameTimeStrValueRes;
}





NS_END(TA_Base_App)
















