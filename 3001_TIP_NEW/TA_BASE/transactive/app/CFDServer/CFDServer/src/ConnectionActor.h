#ifndef __CLASS_CONNECTION_ACTOR__HH__
#define __CLASS_CONNECTION_ACTOR__HH__



#include "CommonData.h"
#include "core/threads/src/IThreadPoolBase.h"
#include "core/sockets/src/TcpSocket.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"

NS_BEGIN(TA_Base_App)


class CComWorker;


class CConnectionActor :public TA_Base_Core::IThreadPoolExecuteItem
{
private:
	enum EConnectionActorState
	{
		Actor_State_Begin,

		Actor_State_WaitTCPHandle,
		Actor_State_GetTCPHandle,

		Actor_State_WaitData,
		Actor_State_RecvingData,
		Actor_State_ProcessDataToRequest,

		Actor_State_NewRequest,
		Actor_State_ProcessingRequest,
		Actor_State_FinishProcessRequest,

		Actor_State_End,
	};
	
public:
	CConnectionActor();
	~CConnectionActor();
public:
	virtual void executeWorkItem();

public:
	void setSocketHandle(TA_Base_Core::TcpSocket* pSocket);
	TA_Base_Core::TcpSocket*  getSocketHandle();
	int recvData(bool bCanRead);
	int getRequestCount();
	bool isHealth();
	bool isFinishProcessedRequest();

private:
	int setRequestStatusNew();
	int setRequestStatusProcessing();
	int setRequestStatusProcessed();
private:
	TA_Base_Core::ReEntrantThreadLockable  m_lockActorState;
	EConnectionActorState m_nActorState;
	TA_Base_Core::TcpSocket* m_pSocket;
	CComWorker* m_pComWorker;
	bool m_bIsHealth;
};


NS_END(TA_Base_App)


#endif //__CLASS_CONNECTION_ACTOR__HH__






