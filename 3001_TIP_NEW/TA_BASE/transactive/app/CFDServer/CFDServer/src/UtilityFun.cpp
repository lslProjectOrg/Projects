#include "UtilityFun.h"

#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/synchronisation/src/ThreadGuard.h"

#ifdef  SYS_USE_BOOST

#include "boost/thread.hpp"
#include "boost/thread/xtime.hpp"
#include "boost/thread/tss.hpp"

#include "boost/date_time/posix_time/posix_time.hpp" 
#include "boost/date_time/c_time.hpp" 
#include "boost/date_time/date.hpp" 
#include "boost/date_time/time.hpp" 



#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/exception.hpp"
#include "boost/filesystem.hpp"


#include "boost/lexical_cast.hpp" 
#include "boost/tokenizer.hpp"

using namespace boost::posix_time; 
using namespace boost::gregorian; 

#include "boost/algorithm/string.hpp"

#endif //SYS_USE_BOOST

//////////////////////////////////////////////////////////////////////////
///using namespace
//////////////////////////////////////////////////////////////////////////



NS_BEGIN(TA_Base_App)


CUtilityFun* CUtilityFun::m_pInstance = 0;
TA_Base_Core::NonReEntrantThreadLockable CUtilityFun::m_instanceLock;


CUtilityFun& CUtilityFun::getInstance()
{
 	FUNCTION_ENTRY("CUtilityFun::getInstance");
	TA_THREADGUARD(m_instanceLock);
	if (NULL == m_pInstance)
	{
		m_pInstance = new CUtilityFun();
	}

  	FUNCTION_EXIT;
	return (*m_pInstance);
}

void CUtilityFun::removeInstance()
{
	DEF_DELETE(m_pInstance);
}



CUtilityFun::CUtilityFun( void )
{

}

CUtilityFun::~CUtilityFun( void )
{

}




// trim from start(left)
std::string& CUtilityFun::mysys_ltrim(std::string& s)
{
	s.erase(s.begin(),std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end(right)
std::string& CUtilityFun::mysys_rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both end
std::string& CUtilityFun::mysys_trim(std::string& s)
{	
	return mysys_ltrim(mysys_rtrim(s));
}

char* CUtilityFun::mysys_strlwr(char* pszSrc)
{
	char *pszOri = pszSrc;
	for (; *pszSrc != '\0'; pszSrc++)	
		*pszSrc = tolower(*pszSrc);

	return pszOri;
}

char* CUtilityFun::mysys_strtolwr(char* pszSrc)
{
	char *pszOri = pszSrc;
	for (; *pszSrc != '\0'; pszSrc++)
	{
		if (*pszSrc < 0x41 || *pszSrc > 0x5A)
			continue;

		*pszSrc |= 0x20;
	}

	return pszOri;
}

char* CUtilityFun::mysys_strupper(char* pszSrc)
{
	char *pszOri = pszSrc;
	for (; *pszSrc != '\0'; pszSrc++)	
		*pszSrc = toupper(*pszSrc);	

	return pszOri;
}

char* CUtilityFun::mysys_strtoupper(char* pszSrc)
{
	char *pszOri = pszSrc;
	for (; *pszSrc != '\0'; pszSrc++)
	{
		if (*pszSrc < 0x61 || *pszSrc > 0x7A)
			continue;

		*pszSrc &= 0xDF;
	}

	return pszOri;
}


int  CUtilityFun::mysys_StdStrToLwr(std::string &strSrc)
{
	int nFunRes = 0;
	size_t nLen = 0;
	char *pTmp = NULL;

	if (strSrc.empty())
	{
		return nFunRes;
	}

	nLen = strSrc.length();

	pTmp = (char *)mysys_malloc(nLen + 16);

	memcpy(pTmp, strSrc.c_str(), nLen);

	strSrc = mysys_strtolwr(pTmp);

	if (NULL != pTmp)
	{
		mysys_free(pTmp);
		pTmp = NULL;
	}

	return nFunRes;
}

int  CUtilityFun::mysys_StdStrToUpper(std::string &strSrc)
{
	int nFunRes = 0;
	size_t nLen = 0;
	char *pTmp = NULL;

	if (strSrc.empty())
	{
		return nFunRes;
	}

	nLen = strSrc.length();

	pTmp = (char *)mysys_malloc(nLen + 16);

	memcpy(pTmp, strSrc.c_str(), nLen);

	strSrc = mysys_strupper(pTmp);

	if (NULL != pTmp)
	{
		mysys_free(pTmp);
		pTmp = NULL;
	}

	return nFunRes;
}

std::string CUtilityFun::mysys_int2str(unsigned nVal)
{
	std::string strRet;
	unsigned nDigit = 0;
	while (nVal > 10)
	{
		nDigit = nVal - (nVal / 10) * 10;
		nDigit |= 0x30;

		strRet.insert(0, 1, nDigit);
		nVal = nVal / 10;
	}
	nDigit = nVal | 0x30;
	strRet.insert(0, 1, nDigit);

	return strRet;
}

void CUtilityFun::mysys_int2str(int uVal, char sz[])
{
	int nDigit = 0;
	int nIndex = 0;
	int nSign  = uVal;

	if ( nSign < 0 ) 
	{
		uVal = -uVal;
		sz[nIndex++] = '-';
	}

	while (uVal > 10)
	{
		nDigit = uVal % 10;
		nDigit |= 0x30;
		sz[nIndex++] = nDigit;
		uVal /= 10;
	}
	nDigit = uVal | 0x30;
	sz[nIndex++] = nDigit;

	sz[nIndex] = 0;
	int nSwap = 0;
	int i = 1;
	while ( i <= nIndex / 2 )
	{
		nSwap = sz[i];
		sz[i] = sz[nIndex - i];
		sz[nIndex - i] = nSwap;
		i++;
	}
}


int CUtilityFun::mysys_str2Int32(const std::string& strInt32)
{
	int nRet = 0;
	long nLongRet = strtoul(strInt32.c_str(), NULL, 10);
	nRet = atoi(strInt32.c_str());
	return nRet;
}

int CUtilityFun::mysys_stricmp(const char* pzLeft, const char* pzRight)
{
	const char* ch1 = pzLeft;
	const char* ch2 = pzRight;
	int res = 0;

	if (strlen(ch1) != strlen(ch2))
	{
		res = 1;
		return res;
	}

	while (true) {
		res = toupper(*ch1) - toupper(*ch2);
		if (res)
			break;
		else
			if (!*ch1)
				break;

		ch1++;             
		ch2++;
	}
	return res;
}

int CUtilityFun::mysys_strnicmp(const char *pzLeft, const char *pzRight, size_t count)
{
	const char* ch1 = pzLeft;
	const char* ch2 = pzRight;
	int res;

	while (count > 0) {
		res = toupper(*ch1) - toupper(*ch2);
		if (res)
			return res;
		else
			if (!*ch1)
				return 0;

		ch1++;             
		ch2++;
		count--;
	}
	return 0;
}

std::string  CUtilityFun::mysys_sysDecToHex(int i)
{
	char tmp[3] = {0}; 
	unsigned char t;
	for (unsigned long j = 0;j < 2;j++) 
	{ 
		t = (i >> (j << 2)) & 0xF;
		if (t < 10)
		{
			tmp[1 - j] = 48 + t;
		}
		else
		{
			tmp[1 - j] = 55 + t;
		}
	} 

	return std::string(tmp);
}

int  CUtilityFun::mysys_sysHexToDec(char cLeft, char cRight)
{
	int  nRetVal    = 0;
	int nRightValue = 0;
	int nLeftValue  = 0;

	if (cRight > 0x40)
		nRightValue = cRight - 55;
	else
		nRightValue = cRight & 0xF;

	if ( cLeft > 0x40)
	{
		nLeftValue = (cLeft - 55) << 4;
	}
	else
	{
		nLeftValue = (cLeft & 0xF) << 4;
	}

	nRetVal = nLeftValue + nRightValue;

	return nRetVal;
}



int CUtilityFun::mysys_getVisableStr(char* Str)
{
	unsigned char Prechar = ' ';
	unsigned char *Temp = NULL;
	unsigned int Len = 0,i = 0;

	if (Str == NULL)
	{
		return -1;
	}

	if (Str[0] == 0)
	{
		return 0;
	}

	Temp = (unsigned char*)Str;

	while (Temp[i] != '\0')
	{
		//  if (Temp[i] < 0x21 || Temp[i] > 0x7e)
		if (Temp[i] < 0x21 /*|| Temp[i] > 0x7e*/)
		{
			if (Prechar == ' ')
				;
			else
			{
				Prechar = ' ';
				Str[Len++] = Prechar ;
			}
		}
		else
		{
			Prechar = Temp[i];
			Str[Len++] = Prechar;
		}
		i++;
	}

	if (Len > 0 && Str[Len - 1] == ' ')
	{
		Str[Len - 1] = '\0';
		return (int) (Len - 1);
	}
	else
	{
		Str[Len] = '\0';
		return Len;
	}
}



int CUtilityFun::mysys_getVisableStdStr(std::string& strSrc)
{
	int nFunRes = 0;
	char* pszStrCopy = NULL;
	int   nszLen= 0;

	nszLen = strSrc.length();
	if (0 == nszLen)
	{
		nFunRes = -1;
		return nFunRes;
	}

	nszLen += 16;

	pszStrCopy = (char *)mysys_malloc(nszLen);
	mysys_strncpy(pszStrCopy, strSrc.c_str(), nszLen);
	strSrc.clear();

	mysys_getVisableStr(pszStrCopy);

	strSrc = pszStrCopy;
	if (NULL != pszStrCopy)
	{
		mysys_free(pszStrCopy);
		pszStrCopy = NULL;
	}
	return nFunRes;
}

char* CUtilityFun::mysys_strncpy(char* dest, const char* src, size_t length)
{
	strncpy(dest, src, length);
	dest[length - 1] = '\0';
	return dest;
}


void CUtilityFun::mysys_free(void *pSrc)
{
	if (NULL == pSrc)
	{
		return;
	}
	else
	{
		free(pSrc);
	}
}

void* CUtilityFun::mysys_malloc(size_t nSize)
{
	void* ptr = malloc(nSize);
	if (ptr != NULL)
	{
		memset(ptr, 0, nSize);
	}
	return ptr;
}


//===================================================

int	CUtilityFun::readAllLineFromFile(const std::string& strFileName, VrtLinesConT& vrtAllLines)
{
	int nFunRes = 0;

	std::ifstream IfstreamSrcFile;
	char*  pOneLine = NULL;
	int nFilelines = 0;
	char* chPtr = NULL;


#ifdef  SYS_USE_BOOST
	boost::filesystem::path filepath;
	filepath = boost::filesystem::system_complete(boost::filesystem::path( strFileName, boost::filesystem::native));
	//boost::filesystem::ifstream IfstreamSrcFile(filepath );
#endif

	if (strFileName.empty())
	{
		nFunRes = -1;
		return nFunRes;
	}


	IfstreamSrcFile.open(strFileName.c_str());
	if (IfstreamSrcFile.fail())
	{
		nFunRes = -1;
		return nFunRes;
	}


	pOneLine = (char *)mysys_malloc(DEF_INT_SingleLineMaxSize);

	// Read in all lines and add them to a vector
	while ( IfstreamSrcFile.getline ( pOneLine, DEF_INT_SingleLineMaxSize ) )
	{

		nFilelines++;
		chPtr = NULL;
		chPtr = strchr( pOneLine, def_char_cfguse_lineEnd );
		if( NULL != chPtr )
		{
			*chPtr = '\0';
		}
		mysys_getVisableStr(pOneLine);
		std::string getOneLine = pOneLine;

		CUtilityFun::getInstance().mysys_trim(getOneLine);
#ifdef SYS_USE_BOOST
		boost::algorithm::trim(getOneLine);
#endif

		if (def_char_cfguse_lineCommont != pOneLine[0])
		{
			if (strlen(pOneLine) > 0)
			{
				vrtAllLines.push_back(getOneLine );
			}	
		}

		memset(pOneLine, 0, DEF_INT_SingleLineMaxSize);


	}//while

	// close file
	if (IfstreamSrcFile.is_open())
	{
		IfstreamSrcFile.close();
	}

	if (NULL != pOneLine)
	{
		mysys_free(pOneLine);
		pOneLine = NULL;
	}
	chPtr = NULL;

	nFunRes = 0;

	return nFunRes;
}
int	CUtilityFun::removeCommentInAllLine(VrtLinesConT& vrtAllLines)
{
	int nFunRes = 0;

	std::string::size_type nFindTmp = std::string::npos;
	VrtLinesConIterT iterAllLine;
	std::string      strOneLine;
	std::string::size_type  nFind = std::string::npos;

	iterAllLine = vrtAllLines.begin();

	while ( iterAllLine != vrtAllLines.end() )
	{	
		strOneLine = (*iterAllLine);

		CUtilityFun::getInstance().mysys_trim(strOneLine);
#ifdef SYS_USE_BOOST
		boost::algorithm::trim(strOneLine);
#endif

		
		if (!strOneLine.empty())  
		{	
			nFind = strOneLine.find(def_char_cfguse_lineCommont);

			if (std::string::npos != nFind)
			{
				strOneLine = strOneLine.substr(0, nFind);
			}			
		}


		if (!strOneLine.empty())  
		{
			CUtilityFun::getInstance().mysys_trim(strOneLine);
#ifdef SYS_USE_BOOST
			boost::algorithm::trim(strOneLine);
#endif

			(*iterAllLine) = strOneLine;
		}

		iterAllLine++;
	}//while


	nFunRes = 0;

	return nFunRes;
}
int	CUtilityFun::removeNoDataLineInAllLine(VrtLinesConT& vrtAllLines)
{
	int nFunRes = 0;
	int nSizeCount = -1;

	VrtLinesConIterT vtrLineIter = vrtAllLines.begin();

	nSizeCount = vrtAllLines.size();
	vtrLineIter = vrtAllLines.begin();
	while (vtrLineIter != vrtAllLines.end())
	{
		if ( (*vtrLineIter).empty())
		{
			vrtAllLines.erase(vtrLineIter);
			nSizeCount = vrtAllLines.size();
			vtrLineIter = vrtAllLines.begin();
		}
		else
		{
			vtrLineIter++;	
		}  			
	}//while	  

	return nFunRes;
}


//////////////////////////////////////////////////////////////////////////
/////b/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef  SYS_USE_BOOST


std::string  CUtilityFun::mysys_boost_GetTimeStr(unsigned int uTimeVale)
{
	int nFunRes = 0;
	unsigned int nGetTime = 0;
	tm* ptmVale;
	time_t timetValue;
	boost::posix_time::ptime ptimeTimetValue;
	boost::posix_time::ptime ptimePtmValue;
	std::string  strGetTime;

	nGetTime =  uTimeVale;
	timetValue = (time_t)nGetTime;
	time ( &timetValue );
	//printf ( "The current local time is: %s", ctime (&rawtime) );
	strGetTime = ctime (&timetValue);

	timetValue = (time_t)nGetTime; 
	ptmVale = localtime( &timetValue );
	timetValue = mktime( ptmVale );

	ptimeTimetValue = boost::posix_time::from_time_t( timetValue );
	ptimePtmValue = boost::posix_time::ptime_from_tm( *ptmVale );

	strGetTime = boost::posix_time::to_simple_string(ptimeTimetValue);
	strGetTime = boost::posix_time::to_simple_string(ptimePtmValue);

	return strGetTime;
}


int  CUtilityFun::mysys_boost_ToLwr(std::string &strSrc)
{
	int nFunRes = 0;
	if (strSrc.empty())
	{
		return nFunRes;
	}

	boost::algorithm::to_lower(strSrc);
	//boost::algorithm::to_lower_copy(strSrc);
	return nFunRes;
}

int  CUtilityFun::mysys_boost_ToUpper(std::string &strSrc)
{
	int nFunRes = 0;
	if (strSrc.empty())
	{
		return nFunRes;
	}

	boost::algorithm::to_upper(strSrc);
	//boost::algorithm::to_lower_copy(strSrc);
	return nFunRes;

}


int  CUtilityFun::mysys_boost_trim(std::string& strSrc)
{
	int nFunRes = 0;

	boost::algorithm::trim(strSrc);

	return nFunRes;
}

int	CUtilityFun::mysys_boost_stricmp(std::string& strLeft, std::string& strRight)
{
	int nFunRes = 0;
	std::string strLeftCopy;
	std::string strRightCopy;

	strLeftCopy	= strLeft;
	strRightCopy = strRight;

	boost::algorithm::to_upper(strLeftCopy);
	boost::algorithm::to_upper(strRight);

	nFunRes = -1;
	if (strLeftCopy == strRightCopy)
	{
		nFunRes = 0;
	}

	return nFunRes;
}

std::string	CUtilityFun::mysys_boost_intTostr(int nVal)
{
	std::string strNumber;

	strNumber = boost::lexical_cast<std::string>(nVal);

	return strNumber;
}

std::string	CUtilityFun::mysys_boost_unsignedintTostr(unsigned int nVal)
{
	std::string strNumber;

	strNumber = boost::lexical_cast<std::string>(nVal);

	return strNumber;
}

int	CUtilityFun::mysys_boost_strToint(std::string& strNumber)
{
	int nNumber;
	if (strNumber.empty())
	{
		nNumber = 0;
		return nNumber;
	}
	nNumber = boost::lexical_cast<int>(strNumber);

	return nNumber;
}

double CUtilityFun::mysys_boost_strTodouble( std::string& strNumber )
{
	double nNumber;
	if (strNumber.empty())
	{
		nNumber = 0;
		return nNumber;
	}

	nNumber = boost::lexical_cast<double>(strNumber);

	return nNumber;

}

void CUtilityFun::mysys_boost_sleep(int sec, int nsec)
{
	boost::xtime timeTmp;
	//boost::xtime_get(&timeTmp, boost::TIME_UTC);
	boost::xtime_get(&timeTmp, 1);
	timeTmp.sec += sec;
	timeTmp.nsec += nsec;
	boost::this_thread::sleep(timeTmp);	
	return;
}
#endif//  SYS_USE_BOOST


//////////////////////////////////////////////////////////////////////////
////boost//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////









NS_END(TA_Base_App)
















