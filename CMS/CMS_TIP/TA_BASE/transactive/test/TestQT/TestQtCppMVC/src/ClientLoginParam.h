#ifndef __CLASS_CLIENT_LOGIN_PARAM_HH__
#define __CLASS_CLIENT_LOGIN_PARAM_HH__

#include "ProjectCommonData.h"

#include <QtCore/QtGlobal>
#include <QtCore/QString>


NS_BEGIN(TA_Base_App)

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
	QString m_strUserName;
	QString m_strPassWord;
	bool m_bEnableDebug;
public:
	QString m_strServerIP;
	qint32 m_nServerPort;
	bool m_bSynchronous;
}; 


NS_END(TA_Base_App)




#endif//__CLASS_NODE_HH__



