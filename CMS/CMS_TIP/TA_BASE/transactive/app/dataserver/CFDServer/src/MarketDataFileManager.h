/*
manager MarketData history Data in one File
*/
#ifndef __CLASS_MARKET_DATA_FILE_MANAGER__HH__
#define __CLASS_MARKET_DATA_FILE_MANAGER__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "InstrumentBarInfoRequest.h"
#include "InstrumentMarketDataRequest.h"

NS_BEGIN(TA_Base_App) 

class CMarketDataFileAnalier;
class CMarketDataLineDispatcher;

class CMarketDataFileManager 
{
public:
	enum enAnalierType
	{
		AnalierType_Begin,

		AnalierType_Dispatch_MarkketData,

		AnalierType_Dispatch_MarkketDataLine,

		AnalierType_End,
	};
public:
	CMarketDataFileManager(enAnalierType nAnalierType);
	~CMarketDataFileManager(void);
public:
	void setInstrumentBarInfoRequest(const CInstrumentBarInfoRequest& instrumentBarInfoRequest);
	int analierFile();
	int setAnalieType(enAnalierType nAnalierType);
	void setInstrumentMarketDataRequest( const CInstrumentMarketDataRequest& instrumentMarketDataRequest );
private:
	CInstrumentBarInfoRequest		m_InstrumentBarInfoRequest;
	CInstrumentMarketDataRequest    m_InstrumentMarketDataRequest;
	CMarketDataFileAnalier*			m_pMarketDataFileAnalier;
	CMarketDataLineDispatcher*      m_pMarketDataLineDispatcher;
	enAnalierType m_nAnalierType;
};

NS_END(TA_Base_App) 


#endif //__CLASS_MARKET_DATA_FILE_MANAGER__HH__


