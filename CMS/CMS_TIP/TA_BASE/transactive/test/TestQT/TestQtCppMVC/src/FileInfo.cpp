#include "FileInfo.h"
#include "ProjectUtilityFun.h"
#include <sstream>

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)

static const int DEF_VALUE_INT_MAXLINELEN = 10240;


CFileInfo::CFileInfo( const std::string& strFilePath )
{
	BOOST_LOG_FUNCTION();
	m_pUtilityFun = new CProjectUtilityFun();

	m_fFileFullPath = boost::filesystem::system_complete(boost::filesystem::path(strFilePath, boost::filesystem::native ));
	m_strFileFullPath = m_fFileFullPath.generic_string();//C://Sample//20121220.csv
	m_strFullFileName = m_fFileFullPath.filename().generic_string();//20121220.csv
	m_strFileName = m_fFileFullPath.stem().generic_string();//20121220
	m_strFileExten = m_fFileFullPath.extension().generic_string();//.csv

	m_strFileNameTimeStrValue = _GetFileNameTimeStrValue(m_strFileName);//2012-12-10 09:00:00
	m_nFileNameTimeIntValue = m_pUtilityFun->strToDateTime(m_strFileNameTimeStrValue);//2012-12-10 09:00:00
}

CFileInfo::~CFileInfo( void )
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}

}

time_t CFileInfo::getFileNameTime()
{
	BOOST_LOG_FUNCTION();

	return m_nFileNameTimeIntValue;
}

std::string CFileInfo::getFileFullPath()
{
	BOOST_LOG_FUNCTION();

	return m_strFileFullPath;
}

std::string CFileInfo::_GetFileNameTimeStrValue(const std::string& strFileName)
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
		//LOG_ERROR<<"error file Name strFileName="<<strFileName;
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

int CFileInfo::getAllLinesInFile( LstLineT& lstLine )
{
	int nFunRes = 0;
	char* pszInBuff = NULL;
	boost::filesystem::ifstream inputDataStream(m_fFileFullPath); 

	if (inputDataStream.fail())
	{
		LOG_ERROR<<"Failed to open file: "<<m_fFileFullPath.generic_string();
		nFunRes = -1;
		return nFunRes;
	}


	pszInBuff = new char[DEF_VALUE_INT_MAXLINELEN];
	memset(pszInBuff, 0, DEF_VALUE_INT_MAXLINELEN);

	// Read in all lines and add them to a list
	while ( inputDataStream.getline ( pszInBuff, DEF_VALUE_INT_MAXLINELEN ) )
	{
		std::string strLine = pszInBuff;
		//check line is not empty or commont line
		if (false == strLine.empty() && ';' != strLine[0])
		{
			lstLine.push_back(strLine);
		}	
		memset(pszInBuff, 0, DEF_VALUE_INT_MAXLINELEN);
	}//while

	// close file
	if (inputDataStream.is_open())
	{
		inputDataStream.close();
	}

	if (NULL != pszInBuff)
	{
		delete []pszInBuff;
		pszInBuff = NULL;
	}

	return nFunRes;
}





NS_END(TA_Base_App)
















