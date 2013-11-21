#ifndef __CLASS_SERVER_CONNECTION_LISTENER__HH__
#define __CLASS_SERVER_CONNECTION_LISTENER__HH__

#include "CommonData.h"
#include "core/threads/src/Thread.h"
#include "core/sockets/src/TcpSocket.h"
#include "core/sockets/src/TcpServerSocket.h"
#include "core/sockets/src/SocketSet.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "core/synchronisation/src/TASemaphore.h"

NS_BEGIN(TA_Base_App)


class CServerConnectionListener : public TA_Base_Core::Thread
{

private:
	enum EThreadJobState
	{
		Job_State_Begin,

		Job_State_WaitListenPort,
		Job_State_GetListenPort,
		Job_State_CreateListenSocket,
		Job_State_WaitForNewClient,

		Job_State_End,
	};

public:
	static CServerConnectionListener& getInstance();
	static void removeInstance();

private:
	static CServerConnectionListener*                      m_pInstance;
	static TA_Base_Core::NonReEntrantThreadLockable	     m_instanceLock;
private:
	CServerConnectionListener(void);	
	~CServerConnectionListener(void);

	DEF_DISABLE_COPY_AND_ASSIGNMENT(CServerConnectionListener);



public:
	virtual void run() ;	
	virtual void terminate() ;

		

private:
	int    _ThreadJob();
	
//////////////////////////////////////////////////////////////////////////
public:
	bool  isFinishWork();
	int   setListenPort(const std::string& strListenPort);

private:
	int	  _ProcessUserTerminate();  


public:
	void  acceptNewClient();
	
private:	
	int _Process_WaitListenPort();
	int _Process_GetListenPort();
	int _Process_CreateListenSocket();
	int _Process_WaitForNewClient();

private:
	int _AcceptClient(TA_Base_Core::TcpServerSocket<TA_Base_Core::TcpSocket>* pTCPServerSocket);


private:
	bool	m_toTerminate;
	EThreadJobState m_nThreadJobState;	
	TA_Base_Core::CTASemaphore m_semaphore;

private:
	TA_Base_Core::NonReEntrantThreadLockable	     m_LockSocketPort;
	std::string   m_strListenSocketPort;	
	int			  m_nListenSocketPort;	
	static const std::string STRDEFAULTSOCKETPORT;

	TA_Base_Core::TcpServerSocket<TA_Base_Core::TcpSocket>*					m_pTCPServerSocket;//server listen socket
	TA_Base_Core::SocketSet< TA_Base_Core::TcpServerSocket< TA_Base_Core::TcpSocket > >	m_SetTCPServerSocket;

	bool 	      m_bTCPRead;
	bool		  m_bTCPWrite; 
	int			  m_nTCPSelectRes; 
	unsigned int  m_timeTCPWait;//MilliSeconds


};


NS_END(TA_Base_App)

#endif  // __CLASS_SERVER_CONNECTION_LISTENER__HH__
