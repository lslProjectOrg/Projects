#include "MarketDataLineDispatcher.h"

#include "CFDServerUtilityFun.h"

#include "core/utilities/src/BoostLogger.h"
USING_BOOST_LOG;



NS_BEGIN(TA_Base_App) 

//////////////////////////////////////////////////////////////////////////

CMarketDataLineDispatcher::CMarketDataLineDispatcher(void)
{
	BOOST_LOG_FUNCTION();
	m_pUtilityFun = new CCFDServerUtilityFun();
	m_mapInstrumentIDAndFileName.clear();
}

CMarketDataLineDispatcher::~CMarketDataLineDispatcher(void)
{
	BOOST_LOG_FUNCTION();

	if (NULL != m_pUtilityFun)
	{
		delete m_pUtilityFun;
		m_pUtilityFun = NULL;
	}
	_ClearData(m_mapInstrumentIDAndFileName);
	m_mapInstrumentIDAndFileName.clear();

}

int CMarketDataLineDispatcher::analierFile()
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	
	std::string filename = m_InstrumentMarketDataRequest.m_strCurrentAnalierFileName;
	std::ifstream priceFile(filename.c_str());
	std::string strLogInfo = "get data from file";
	std::string strOneLine;

	LOG_INFO<<"AnalierFile: "<<m_InstrumentMarketDataRequest.m_strCurrentAnalierFileName;

	while (!priceFile.eof())
	{
		priceFile>>strOneLine;
		_AnalyseOneLine(strOneLine);
		strOneLine.clear();
	}

	priceFile.close();

	return nFunRes;
}

void CMarketDataLineDispatcher::setInstrumentMarketDataRequest( const CInstrumentMarketDataRequest& instrumentMarketDataRequest )
{
	BOOST_LOG_FUNCTION();

	m_InstrumentMarketDataRequest = instrumentMarketDataRequest;
}
int CMarketDataLineDispatcher::_AnalyseOneLine(const std::string& strOneLine)
{
	BOOST_LOG_FUNCTION();
	int nFunRes = 0;
	std::string strInstrumentID;
	MapInstrumentIDFileNameIteratorT  iterFind;
	std::ofstream*  pFileHandle = NULL;
	std::string strFileName;

	if (strOneLine.empty())
	{
		nFunRes = 0;
		return nFunRes;
	}

	strInstrumentID = _GetInstrumentIDFormLine(strOneLine); 
	iterFind = m_mapInstrumentIDAndFileName.find(strInstrumentID);
	if (iterFind != m_mapInstrumentIDAndFileName.end())
	{
		pFileHandle = iterFind->second;
	}
	else
	{
		strFileName = _GetFileName(strInstrumentID);
		pFileHandle = new std::ofstream(strFileName.c_str());
		m_mapInstrumentIDAndFileName.insert(MapInstrumentIDFileNameValueTypeT(strInstrumentID, pFileHandle));
	}

	if (NULL != pFileHandle)
	{
		(*pFileHandle)<<strOneLine;
	}
	
	return nFunRes;
}

std::string CMarketDataLineDispatcher::_GetInstrumentIDFormLine(const std::string& strGetLine)
{
	BOOST_LOG_FUNCTION();
	int nFind = -1;
	std::string strInstrumentID;

	nFind = strGetLine.find_first_of(",");
	strInstrumentID = strGetLine.substr(0, nFind);

	return strInstrumentID;
}

std::string CMarketDataLineDispatcher::_GetFileName(const std::string& strInstrumentID)
{
	BOOST_LOG_FUNCTION();
	int nFind = -1;
	std::string strFileName;

	//C:\TestData\HistoryMarketDataInstrument\3320.csv
	strFileName = m_InstrumentMarketDataRequest.m_strHistoryMarketDataInstrument + "//"+ strInstrumentID +".csv";

	return strFileName;
}

void CMarketDataLineDispatcher::_ClearData(MapInstrumentIDFileNameT& mapInstrumentIDFileName)
{
	BOOST_LOG_FUNCTION();
	MapInstrumentIDFileNameIteratorT iterMap;
	std::ofstream*  pFileHandle = NULL;

	iterMap = mapInstrumentIDFileName.begin();
	while (iterMap != mapInstrumentIDFileName.end())
	{
		pFileHandle = iterMap->second;
		pFileHandle->flush();
		pFileHandle->close();

		delete pFileHandle;
		pFileHandle = NULL;

		iterMap++;
	}//while
	
	mapInstrumentIDFileName.clear();
}




NS_END(TA_Base_App) 



