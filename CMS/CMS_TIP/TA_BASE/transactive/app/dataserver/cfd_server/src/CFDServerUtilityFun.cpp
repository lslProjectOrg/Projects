#include "CFDServerUtilityFun.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <list> 
#include <time.h>


#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_Base_App)


CCFDServerUtilityFun::CCFDServerUtilityFun( void )
{

}

CCFDServerUtilityFun::~CCFDServerUtilityFun( void )
{

}



std::string CCFDServerUtilityFun::getTimeString(unsigned int nTimeValue)
{
	std::string	strTimeString;
	char* pszCurTime = NULL;
	int nBufferSize = 256;
	struct tm* pTM = NULL;
	time_t time_Value = (time_t)nTimeValue;

	pszCurTime =new char[nBufferSize];
	memset(pszCurTime, 0, nBufferSize);

	pTM = localtime(&(time_Value));

	//format to string
	if ( NULL != pTM )
	{
		sprintf(pszCurTime, "[%04d-%02d-%02d %02d:%02d:%02d]",
			pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,
			pTM->tm_hour, pTM->tm_min, pTM->tm_sec); 

		strTimeString = pszCurTime;     
	}

	delete pszCurTime;
	pszCurTime = NULL;
	pTM = NULL;

	return strTimeString;
}

void CCFDServerUtilityFun::logBarInfo(const std::string& strInfo, const Bar &bar)
{
	std::string strBarTime;

	strBarTime = getTimeString(bar.Time);
	LOG_DEBUG<<strInfo<<" "<<"bar.Time="<<bar.Time;
	LOG_DEBUG<<strInfo<<" "<<"strBarTime="<<strBarTime;
	LOG_DEBUG<<strInfo<<" "<<"bar.Open="<<bar.Open;
	LOG_DEBUG<<strInfo<<" "<<"bar.Close="<<bar.Close;
	LOG_DEBUG<<strInfo<<" "<<"bar.High="<<bar.High;
	LOG_DEBUG<<strInfo<<" "<<"bar.Low="<<bar.Low;
	LOG_DEBUG<<strInfo<<" "<<"bar.Volume="<<bar.Volume;
}



NS_END(TA_Base_App)
















