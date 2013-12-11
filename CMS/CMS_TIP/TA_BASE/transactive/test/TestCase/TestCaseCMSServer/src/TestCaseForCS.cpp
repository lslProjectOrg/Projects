#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>

#include "TestCaseForCS.h"
#include "CommonData.h"
#include "SysTypes.h"
#include "ServerManage.h"
#include "ClientManager.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

namespace boost 
{
	class thread;
	class thread_group;
}


// extern boost::condition_variable g_conditionMainRun;

NS_BEGIN(TA_Base_Test) 
std::string g_string_strServerAddress = "tcp://127.0.0.1:5555";
int g_n_EachClientSendFrameNUM = 10000*10;
int g_n_ClientNUM = 1;
int g_n_TotalClientSendFrameNUM = g_n_ClientNUM * g_n_EachClientSendFrameNUM;








//////global Data init////////////////////////////////////////////////////////////////////

char g_TestData_nomal_Frame;// = ComFrameType_TestNomalData;
char g_TestData_min_Frame;// = ComFrameType_TestMinData;
char g_TestData_max_Frame;// = ComFrameType_TestMaxData;

char g_TestData_min_nCharValue;// = SCHAR_MIN;//SCHAR_MAX 
char g_TestData_nomal_nCharValue;// = 'A';
char g_TestData_max_nCharValue;// = SCHAR_MAX;//SCHAR_MAX

unsigned char g_TestData_min_nUCharValue;// = 0;//UCHAR_MAX
unsigned char g_TestData_nomal_nUCharValue;// = 240;
unsigned char g_TestData_max_nUCharValue;// = UCHAR_MAX;//UCHAR_MAX

short g_TestData_min_nShortIntValue;// = SHRT_MIN;//SHRT_MAX
short g_TestData_nomal_nShortIntValue;// = 1;//SHRT_MAX
short g_TestData_max_nShortIntValue;// = SHRT_MAX;//SHRT_MAX

unsigned short g_TestData_min_nUShortIntValue;// = 0;//USHRT_MAX
unsigned short g_TestData_nomal_nUShortIntValue;// = 1;//INT_MAX
unsigned short g_TestData_max_nUShortIntValue;// = USHRT_MAX;//USHRT_MAX

int g_TestData_min_nIntValue;// = INT_MIN;//INT_MAX
int g_TestData_nomal_nIntValue;// = -1111;
int g_TestData_max_nIntValue;// = INT_MAX;//INT_MAX

unsigned int g_TestData_min_nUIntValue;// = 0;//UINT_MAX
unsigned int g_TestData_nomal_nUIntValue;// = 1111;
unsigned int g_TestData_max_nUIntValue;// = UINT_MAX;//UINT_MAX



#ifndef __linux__
BigInt32 g_TestData_min_nBitIntValue;// = LLONG_MIN;//LLONG_MAX
UBigInt32 g_TestData_min_nUBitIntValue;// = 0;//ULONG_MAX
#else 
BigInt32 g_TestData_min_nBitIntValue;// = LONG_MIN;//LONG_MAX
UBigInt32 g_TestData_min_nUBitIntValue;// = 0;//ULONG_MAX
#endif

BigInt32 g_TestData_nomal_nBitIntValue;// = -222222222;
UBigInt32 g_TestData_nomal_nUBitIntValue;// = 222222222; 

#ifndef __linux__
BigInt32 g_TestData_max_nBitIntValue;// = LLONG_MAX;//LLONG_MIN
UBigInt32 g_TestData_max_nUBitIntValue;// = ULLONG_MAX;//0
#else 
BigInt32 g_TestData_max_nBitIntValue;// = LONG_MAX;
UBigInt32 g_TestData_max_nUBitIntValue;// = ULONG_MAX;
#endif

float g_TestData_min_fFloatValue;// = FLT_MIN;//FLT_MAX ;
float g_TestData_nomal_fFloatValue;// = 3.1416f;
float g_TestData_max_fFloatValue;// = FLT_MAX;//FLT_MAX ;

double g_TestData_min_fDoubleValue;// = DBL_MIN;//DBL_MAX
double g_TestData_nomal_fDoubleValue;// = 3.14159;
double g_TestData_max_fDoubleValue;// = DBL_MAX;//DBL_MAX

std::string g_TestData_min_strStringValue;// = "T";
std::string g_TestData_nomal_strStringValue;// = "Hello World!!!";
std::string g_TestData_max_strStringValue;// = "255";//sms support string max len 256
int DEF_CMS_MAX_STESTRING_LEN;//       255

//////global Data init end////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

TestCaseForCS::TestCaseForCS(void)
{
	_InitGlobalData();
	_InitMaxLenString();
}

TestCaseForCS::~TestCaseForCS(void)
{

}




void TestCaseForCS::_InitGlobalData()
{
	//////global Data init////////////////////////////////////////////////////////////////////
	g_TestData_nomal_Frame = ComFrameType_TestNomalData;
	g_TestData_min_Frame = ComFrameType_TestMinData;
	g_TestData_max_Frame = ComFrameType_TestMaxData;


	g_TestData_min_nCharValue = SCHAR_MIN;//SCHAR_MAX 
	g_TestData_nomal_nCharValue = 'A';
	g_TestData_max_nCharValue = SCHAR_MAX;//SCHAR_MAX

	g_TestData_min_nUCharValue = 0;//UCHAR_MAX
	g_TestData_nomal_nUCharValue = 240;
	g_TestData_max_nUCharValue = UCHAR_MAX;//UCHAR_MAX

	g_TestData_min_nShortIntValue = SHRT_MIN;//SHRT_MAX
	g_TestData_nomal_nShortIntValue = 1;//SHRT_MAX
	g_TestData_max_nShortIntValue = SHRT_MAX;//SHRT_MAX

	g_TestData_min_nUShortIntValue = 0;//USHRT_MAX
	g_TestData_nomal_nUShortIntValue = 1;//INT_MAX
	g_TestData_max_nUShortIntValue = USHRT_MAX;//USHRT_MAX

	g_TestData_min_nIntValue = INT_MIN;//INT_MAX
	g_TestData_nomal_nIntValue = -1111;
	g_TestData_max_nIntValue = INT_MAX;//INT_MAX

	g_TestData_min_nUIntValue = 0;//UINT_MAX
	g_TestData_nomal_nUIntValue = 1111;
	g_TestData_max_nUIntValue = UINT_MAX;//UINT_MAX



#ifndef __linux__
	g_TestData_min_nBitIntValue = LLONG_MIN;//LLONG_MAX
	g_TestData_min_nUBitIntValue = 0;//ULONG_MAX
#else 
	g_TestData_min_nBitIntValue = LONG_MIN;//LONG_MAX
	g_TestData_min_nUBitIntValue = 0;//ULONG_MAX
#endif

	g_TestData_nomal_nBitIntValue = -222222222;
	g_TestData_nomal_nUBitIntValue = 222222222; 

#ifndef __linux__
	g_TestData_max_nBitIntValue = LLONG_MAX;//LLONG_MIN
	g_TestData_max_nUBitIntValue = ULLONG_MAX;//0
#else 
	g_TestData_max_nBitIntValue = LONG_MAX;
	g_TestData_max_nUBitIntValue = ULONG_MAX;
#endif

	g_TestData_min_fFloatValue = FLT_MIN;//FLT_MAX ;
	g_TestData_nomal_fFloatValue = 3.1416f;
	g_TestData_max_fFloatValue = FLT_MAX;//FLT_MAX ;

	g_TestData_min_fDoubleValue = DBL_MIN;//DBL_MAX
	g_TestData_nomal_fDoubleValue = 3.14159;
	g_TestData_max_fDoubleValue = DBL_MAX;//DBL_MAX

	g_TestData_min_strStringValue = "T";
	g_TestData_nomal_strStringValue = "Hello World!!!";
	g_TestData_max_strStringValue = "255";//sms support string max len 256
	DEF_CMS_MAX_STESTRING_LEN   =    255;

}


void TestCaseForCS::_InitMaxLenString()
{
	BOOST_LOG_FUNCTION();

	//1024*64
	std::string strTemp1024;

	strTemp1024.clear();
	//128<std::string<256
	for (int nIndex = 0; nIndex < DEF_CMS_MAX_STESTRING_LEN; nIndex++)
	{
		strTemp1024 += "T";
	}

	g_TestData_max_strStringValue.clear();
	g_TestData_max_strStringValue += strTemp1024;

	LOG_DEBUG<<"g_TestData_max_strStringValue="<<g_TestData_max_strStringValue;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void TestCaseForCS::runTestCase_ForClient()
{
	BOOST_LOG_FUNCTION();

	TA_Base_Test::CClientManager* pTEST_TestCaseCMSClient = new TA_Base_Test::CClientManager();
	pTEST_TestCaseCMSClient->start();



}


void TestCaseForCS::runTestCase_ForServer()
{
	BOOST_LOG_FUNCTION();
	TA_Base_Test::CServerManager* pTEST_ServerManager = new TA_Base_Test::CServerManager();
	pTEST_ServerManager->runTestCase();

}


NS_END(TA_Base_Test) 



