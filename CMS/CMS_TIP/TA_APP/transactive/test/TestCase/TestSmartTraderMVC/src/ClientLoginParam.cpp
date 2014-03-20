#include "ClientLoginParam.h"

#include "BoostLogger.h"
USING_BOOST_LOG;

NS_BEGIN(TA_App_App)


//////////////////////////////////////////////////////////////////////////
static const std::string DEFVALUE_String_UserName = "CTS007";
static const std::string DEFVALUE_String_PassWord = "Password1";
static const bool DEFVALUE_Bool_EnableDebug = false;

static const std::string DEFVALUE_String_ServerIP = "211.144.192.149";
static const int DEFVALUE_Int_ServerPort = 9170;
static const bool DEFVALUE_Bool_Synchronous = true;
//////////////////////////////////////////////////////////////////////////

CClientLoginParam::CClientLoginParam()
{
	m_strUserName = DEFVALUE_String_UserName;
	m_strPassWord = DEFVALUE_String_PassWord;
	m_bEnableDebug = DEFVALUE_Bool_EnableDebug;
	m_strServerIP = DEFVALUE_String_ServerIP;
	m_nServerPort = DEFVALUE_Int_ServerPort;
	m_bSynchronous = DEFVALUE_Bool_Synchronous;
}

CClientLoginParam::~CClientLoginParam()
{

}

CClientLoginParam& CClientLoginParam::operator=( const CClientLoginParam& clinetLoginParam )
{
	m_strUserName = clinetLoginParam.m_strUserName;
	m_strPassWord = clinetLoginParam.m_strPassWord;
	m_bEnableDebug = clinetLoginParam.m_bEnableDebug;
	m_strServerIP = clinetLoginParam.m_strServerIP;
	m_nServerPort = clinetLoginParam.m_nServerPort;
	m_bSynchronous = clinetLoginParam.m_bSynchronous;
	return *this;
}

void CClientLoginParam::setDefaultValue()
{
	m_strUserName = DEFVALUE_String_UserName;
	m_strPassWord = DEFVALUE_String_PassWord;
	m_bEnableDebug = DEFVALUE_Bool_EnableDebug;
	m_strServerIP = DEFVALUE_String_ServerIP;
	m_nServerPort = DEFVALUE_Int_ServerPort;
	m_bSynchronous = DEFVALUE_Bool_Synchronous;
}

NS_END(TA_App_App)












