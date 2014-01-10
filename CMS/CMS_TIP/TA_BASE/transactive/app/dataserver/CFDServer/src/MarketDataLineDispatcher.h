#ifndef __CLASS_MARKET_DATA_LINE_DISPATCHER__HH__
#define __CLASS_MARKET_DATA_LINE_DISPATCHER__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <sstream>

#include "InstrumentMarketDataRequest.h"


NS_BEGIN(TA_Base_App) 

class CCFDServerUtilityFun;

class CMarketDataLineDispatcher 
{
public:
	typedef std::map<std::string, std::ofstream*>				MapInstrumentIDFileNameT;
	typedef std::map<std::string, std::ofstream*>::iterator		MapInstrumentIDFileNameIteratorT;
	typedef std::map<std::string, std::ofstream*>::value_type	MapInstrumentIDFileNameValueTypeT;
public:
	CMarketDataLineDispatcher(void);
	~CMarketDataLineDispatcher(void);

public:
	void setInstrumentMarketDataRequest(const CInstrumentMarketDataRequest& instrumentMarketDataRequest);
	int analierFile();
	int _AnalyseOneLine(const std::string& strOneLine);
	std::string _GetFileName(const std::string& strInstrumentID);
	std::string _GetInstrumentIDFormLine(const std::string& strGetLine);
	void _ClearData(MapInstrumentIDFileNameT& mapInstrumentIDFileName);
private:
	CInstrumentMarketDataRequest		m_InstrumentMarketDataRequest;
	CCFDServerUtilityFun*				m_pUtilityFun;
	MapInstrumentIDFileNameT            m_mapInstrumentIDAndFileName;
};

NS_END(TA_Base_App) 


#endif //__CLASS_MARKET_DATA_FILE_ANALIER__HH__


