#ifndef __CLASS_CLIENT_LOGIN_PARAM_HH__
#define __CLASS_CLIENT_LOGIN_PARAM_HH__

#include "ProjectCommonData.h"

#include <QtCore/QtGlobal>
#include <QtCore/QString>


NS_BEGIN(TA_App_App)

class CClientLoginParam 
{ 
public: 
	CClientLoginParam(); 
	virtual ~CClientLoginParam(); 
public:
	CClientLoginParam& operator=(const CClientLoginParam& clinetLoginParam);
public:
	void setDefaultValue();
public:
	std::string m_strUserName;
	std::string m_strPassWord;
	bool m_bEnableDebug;
public:
	std::string m_strServerIP;
	int m_nServerPort;
	bool m_bSynchronous;
}; 


NS_END(TA_App_App)




#endif//__CLASS_NODE_HH__



