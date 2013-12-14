#include "UtilityFun.h"


NS_BEGIN(TA_Base_Test)


CUtilityFun::CUtilityFun( void )
{

}

CUtilityFun::~CUtilityFun( void )
{

}




//////////////////////////////////////////////////////////////////////////
/////b/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



//0C632E95-26C6-A803-C191-FDCA3A738D0D
std::string CUtilityFun::mysys_boost_generateUUID()
{
	std::string strUUID36;

	boost::uuids::uuid nUUID = boost::uuids::random_generator()();
	//std::cout << uuid << std::endl;
	//7feb24af-fc38-44de-bc38-04defc3804de
	strUUID36 = boost::lexical_cast<std::string>(nUUID);

	// Return the UUID as a std::string
	return strUUID36;
}






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



void CUtilityFun::mysys_boost_sleep(int ms)
{
	boost::xtime timeTmp;
	boost::xtime_get(&timeTmp, boost::TIME_UTC_);
	timeTmp.nsec += ms%1000*1000*1000;
	timeTmp.sec += ms/1000;
	boost::thread::sleep(timeTmp);
}


//////////////////////////////////////////////////////////////////////////
////boost//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////









NS_END(TA_Base_Test)
















