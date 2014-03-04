#ifndef __CLASS_INSTRUMENT_BAR_INFO_REQUEST_H__
#define __CLASS_INSTRUMENT_BAR_INFO_REQUEST_H__ 


#include "CFDServerCommonData.h"
#include "BarCalculator.h"

NS_BEGIN(TA_Base_App)


class CInstrumentBarInfoRequest
{
public:
	CInstrumentBarInfoRequest(void);	
	~CInstrumentBarInfoRequest(void);

public:
	CInstrumentBarInfoRequest& operator=(const CInstrumentBarInfoRequest& cfdRequest);

public:
	void logInfo();
	void getLstBarTime(std::list<int>& lstBarTime);
	void initData();
private:
	void _AddBarTimeNormal();

public:
	//c://Hisdata   20131220
	std::string m_strHistoryDataDirectory;

	//C://SaveData//SaveDataBar   3320.BAR
	std::string m_strSaveDataDirectoryBAR;

	//C://SaveData//SaveDataTIK   3320.TIK
	std::string m_strSaveDataDirectoryTIK;

	std::string m_strDbType;
	std::string m_strStartTime;

	time_t m_nStartTime;
	TA_Base_App::DbServerType m_nDBType;
public:
	//c://Hisdata//20131220
	std::string m_strCurrentAnalierFileName;	
	
private:
	std::list<int>   m_lstBarTime;
};


NS_END(TA_Base_App)


#endif // __CLASS_INSTRUMENT_BAR_INFO_REQUEST_H__









