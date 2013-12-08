#include <boost/test/unit_test.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include "MessageBroker.h"
#include <iostream>
using namespace cms;

static size_t m_nRecvMsgCount = 0;
const static size_t m_nSendMsgCount = 1000000;
static SessionID m_nClientSessionID = INVALID_SESSION_ID; 

static boost::mutex m_mutexRecvMsg;
static boost::condition_variable m_conditionMsg;

static char m_nCharValue = 'A';
static unsigned char m_nUCharValue = 240;

static int m_nIntValue = -1111;
static unsigned m_nUIntValue = 1111;

#ifndef __linux__
static long long m_nBitIntValue = -222222222;
static unsigned long long m_nUBitIntValue = 222222222; 
#else 
static long m_nBitIntValue = -222222222;
static unsigned long m_nUBitIntValue = 222222222; 
#endif
static float m_fFloatValue = 3.1416;
static double m_fDoubleValue = 3.14159;

static string m_strStringValue = "Hello World!!!";
static bool m_bClientConnected = false;
static string m_strServerAddress = "tcp://127.0.0.1:5555";

static boost::chrono::system_clock::time_point m_nTimeStart;
static boost::chrono::system_clock::time_point m_nTimeEnd;

void handleConnected(const SessionInfo &stSessionInfo)
{
	std::cout<<"tcp connected from "<<stSessionInfo.peerAddress<<std::endl;
	static int connections = 0;
	connections++;
	if (stSessionInfo.peerAddress == m_strServerAddress)
	{
		m_nClientSessionID = stSessionInfo.sessionID;
	}
	
	if (connections == 2)
	{
		m_bClientConnected = true;
		m_conditionMsg.notify_one();
	}
}

void handleMessage(Message::Ptr message)
{
	char nGetCharValue;
	unsigned char nGetUCharValue;
	int nGetIntValue;
	unsigned int nGetUIntValue;
#ifndef __linux__
	long long nGetBitIntValue;
	unsigned long long nGetUBitIntValue;
#else
 	long nGetBitIntValue;
	unsigned long nGetUBitIntValue;
#endif
	float fGetFloatValue;
	double fGetDoubleValue;
	string strGetStringValue;

	int duration = 0;

	message->read(nGetCharValue);
	message->read(nGetUCharValue);
	message->read(nGetIntValue);
	message->read(nGetUIntValue);
	message->read(nGetBitIntValue);
	message->read(nGetUBitIntValue);
	message->read(fGetFloatValue);
	message->read(fGetDoubleValue);
	message->read(strGetStringValue);

	BOOST_CHECK(nGetCharValue == m_nCharValue);
	BOOST_CHECK(nGetUCharValue == m_nUCharValue);
	BOOST_CHECK(nGetIntValue == m_nIntValue);
	BOOST_CHECK(nGetUIntValue == m_nUIntValue);
	BOOST_CHECK(nGetBitIntValue == m_nBitIntValue);
	BOOST_CHECK(nGetUBitIntValue == m_nUBitIntValue);
	BOOST_CHECK(fGetFloatValue == m_fFloatValue);
	BOOST_CHECK(fGetDoubleValue == m_fDoubleValue);		
	BOOST_CHECK(strGetStringValue == m_strStringValue);

	++m_nRecvMsgCount;
	if (m_nRecvMsgCount == m_nSendMsgCount)
	{
		m_nTimeEnd = boost::chrono::system_clock::now();
		duration = boost::chrono::duration_cast<boost::chrono::seconds>(m_nTimeEnd - m_nTimeStart).count();

		std::cout<<m_nSendMsgCount<<" messages received and verified successfully in " <<duration <<" seconds."<<std::endl;

		m_conditionMsg.notify_one();
	}	
}

BOOST_AUTO_TEST_CASE( IntegrityTest )
{
	MessageBroker broker;
	broker.onReceived = handleMessage;
	broker.onConnected = handleConnected;
	
	//broker.start();
	broker.listen(m_strServerAddress);
	broker.connect(m_strServerAddress);
	{	
		boost::mutex::scoped_lock lock(m_mutexRecvMsg);
		m_conditionMsg.wait(lock);
	}
	
	BOOST_CHECK(m_bClientConnected == true);

	m_nTimeStart = boost::chrono::system_clock::now();
	for (int i = 0; i < m_nSendMsgCount; ++i)
	{
		Message::Ptr message(new Message(10));
		message->write(m_nCharValue);
		message->write(m_nUCharValue);
		message->write(m_nIntValue);
		message->write(m_nUIntValue);
		message->write(m_nBitIntValue);
		message->write(m_nUBitIntValue);
		message->write(m_fFloatValue);
		message->write(m_fDoubleValue);
		message->write(m_strStringValue);
		message->setSessionID(m_nClientSessionID);	
		broker.sendToSession(m_nClientSessionID, message);
	}

	
	{	
		boost::mutex::scoped_lock lock(m_mutexRecvMsg);
		m_conditionMsg.wait(lock);
	}
}


