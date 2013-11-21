#ifndef __CLASS_COM_WORKER__HH__
#define __CLASS_COM_WORKER__HH__

#include "CommonData.h"
#include "core/sockets/src/TcpSocket.h"
#include "DataBuffer.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"

NS_BEGIN(TA_Base_App)

class CComWorker 
{
public:
	typedef		std::list<CDataBuffer*>							lstFrameBufferConT;
	typedef		std::list<CDataBuffer*>::iterator				lstFrameBufferConIterT;

public:
	CComWorker(TA_Base_Core::TcpSocket* pSocketRef);
	virtual ~CComWorker();
public:
	int			readData();
	int         analysisData();
	int			writeBuffer(CDataBuffer* pSendBuff);//TODO.
	bool		isStillConnected();
	int			getRecvFrameCount();
	int getListRecvFrame(lstFrameBufferConT& LstFrameBuffer);

private:
	bool		_HaveFrameInBuffer(CDataBuffer* pRecvBufferTotal);	
	int			_GetFrameFromBuffer(CDataBuffer* pRecvBufferTotal, lstFrameBufferConT& lstRecvFrame);

private:
	TA_Base_Core::ReEntrantThreadLockable m_lockSocketRead;
	TA_Base_Core::ReEntrantThreadLockable m_lockSocketWrite;

	TA_Base_Core::TcpSocket*	m_pSocketRef;//m_pSocketRef

	CDataBuffer*	m_pRecvBufferOnce;
	CDataBuffer*	m_pRecvBufferTotal;

	lstFrameBufferConT   m_lstRecvFrame;


	TA_Base_Core::ReEntrantThreadLockable m_lockBufferDataTmp;
	CDataBuffer*	m_pBufferDataTmp;
	bool            m_bIsStillConnected;

};


NS_END(TA_Base_App)


#endif //__CLASS_COM_WORKER__HH__






