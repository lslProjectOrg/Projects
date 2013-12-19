/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //COTS/sqlgen/1.1.0/src/UtilityFun.cpp $
 * @author:  Shenglong.lin
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/06/21 12:00:00 $
 * Last modified by:  Shenglong.Lin $
 * 
 */
 
#include "UtilityFun.h"
 
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype> //#include <ctype.h>
#include <functional>

#include "CommonDef.h"
#include "CommonData.h"


CUtilityFun* CUtilityFun::m_pInstance = 0;

CUtilityFun& CUtilityFun::getInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CUtilityFun();
	} 
	return (*m_pInstance);
}

void CUtilityFun::removeInstance()
{
	DEF_DELETE(m_pInstance);
}


CUtilityFun::CUtilityFun()
{

}

CUtilityFun::~CUtilityFun()
{

}

char* CUtilityFun::sysStrLwr( char* pszSrc )
{
	char *pszOri = pszSrc;
	
	for (; *pszSrc != '\0'; pszSrc++)
	{	
		*pszSrc = tolower(*pszSrc);
	}
	
	return pszOri;
}

char* CUtilityFun::sysStrUpper( char* pszSrc )
{
	char *pszOri = pszSrc;
	
	for (; *pszSrc != '\0'; pszSrc++)
	{	
		*pszSrc = toupper(*pszSrc);	
	}
	
	return pszOri;
}

std::string CUtilityFun::int2str( int nVal )
{
	std::string strValue; 				
	std::ostringstream str;
	
	str << nVal;	
	strValue = str.str();
	
	return strValue;
}

int CUtilityFun::sysStricmp( const char* pzLeft, const char* pzRight )
{
	const char* pszLeftPos = pzLeft;
	const char* pszRightPos = pzRight;
	int nRes = 0;

	if (strlen(pszLeftPos) != strlen(pszRightPos))
	{
		nRes = -1;
		return nRes;
	}

	while (true) 
	{
		nRes = toupper(*pszLeftPos) - toupper(*pszRightPos);
		if (nRes)
		{
			//diff then break
			break;
		}	
		else if (!*pszLeftPos)
		{
			//the end pos of left string
			break;
		}

		pszLeftPos++;             
		pszRightPos++;
	}
	
	return nRes;
}
 

// trim from start(left)
std::string& CUtilityFun::ltrim(std::string& s)
{
	s.erase(s.begin(),std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end(right)
std::string& CUtilityFun::rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both end
std::string& CUtilityFun::trim(std::string& s)
{
	return ltrim(rtrim(s));
}







int CUtilityFun::removeUnVisableStr(std::string& strSrc)
{
	int nFunRes = 0;
	int nstrSrcSize = -1;
	std::string strTmp;

	if (strSrc.empty())
	{
		return nFunRes;
	}

	nstrSrcSize = strSrc.size();

	for (int nIndex = 0; nIndex < nstrSrcSize; nIndex++)
	{
		if (strSrc[nIndex] < 0x20 || strSrc[nIndex] > 0x7e ) //32  126 strSrc[nIndex] < 0x21 || strSrc[nIndex] > 0x7e
		{
			//UnVisable char
			//strTmp += strSrc[nIndex];
		}
		else
		{
			strTmp += strSrc[nIndex];
		}


		if (0x09 == strSrc[nIndex])//Black char ' '
		{
			strTmp += strSrc[nIndex];
		}
	}//for

	strSrc = strTmp;  
	nstrSrcSize = strSrc.size(); 
	return nFunRes;
}








