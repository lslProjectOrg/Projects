#ifndef __CLASS_COM_WORKER__HH__
#define __CLASS_COM_WORKER__HH__

#include "CommonData.h"
#include "core/sockets/src/TcpSocket.h"
#include "DataBuffer.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"

NS_BEGIN(TA_Base_App)

class CComWorker 
{
private:
	typedef		std::list<CDataBuffer*>							lstFrameBufferConT;
	typedef		std::list<CDataBuffer*>::iterator				lstFrameBufferConIterT;
	typedef		std::list<CDataBuffer*>::value_type				lstFrameBufferValueTypeT;

public:
	CComWorker(TA_Base_Core::TcpSocket* pSocket);
	virtual ~CComWorker();
public:
	int			readData();
	int         analysisAndProcessData();
	int			writeBuffer(CDataBuffer* pSendBuff);//TODO.

private:
	bool		_HaveFrameInBuffer(CDataBuffer* pRecvBufferTotal);	
	int			_GetFrameFromBuffer(CDataBuffer* pRecvBufferTotal, lstFrameBufferConT& lstRecvFrame);
	int			_ProcessRecvFrame();

private:
	TA_Base_Core::ReEntrantThreadLockable m_lockSocketRead;
	TA_Base_Core::ReEntrantThreadLockable m_lockSocketWrite;

	TA_Base_Core::TcpSocket*	m_pSocket;//m_pSocketRef

	CDataBuffer*	m_pRecvBufferOnce;
	CDataBuffer*	m_pRecvBufferTotal;

	lstFrameBufferConT   m_lstRecvFrame;


	TA_Base_Core::ReEntrantThreadLockable m_lockBufferDataTmp;
	CDataBuffer*	m_pBufferDataTmp;

};


NS_END(TA_Base_App)


#endif //__CLASS_COM_WORKER__HH__






