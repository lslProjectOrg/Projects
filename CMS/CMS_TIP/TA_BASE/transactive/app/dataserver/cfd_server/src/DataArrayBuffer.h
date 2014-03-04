#ifndef __CLASS_DATA_ARRAY_BUFFER__HH__
#define __CLASS_DATA_ARRAY_BUFFER__HH__

#include "CFDServerCommonData.h"

#include "BarCalculator.h"
#include "IDbOper.h"


NS_BEGIN(TA_Base_App) 

class CInstrumentBarInfo;
class CInstrumentTickInfo;
class CDBOperParam;
class CInstrumentBarInfoRequest;
class CSqliteDbOper;


class CDataArrayBuffer 
{
public:
	typedef std::list<CInstrumentBarInfo*>							  LstInstrumentBarInfoT;
	typedef std::list<CInstrumentBarInfo*>::iterator				  LstInstrumentBarInfoIterT;

	typedef std::map<int, LstInstrumentBarInfoT*>				      MapIntervalBarLstT;
	typedef std::map<int, LstInstrumentBarInfoT*>::iterator           MapIntervalBarLstIterT;
	typedef std::map<int, LstInstrumentBarInfoT*>::value_type         MapIntervalBarLstValueTypeT;

	typedef std::list<CInstrumentTickInfo*>							  LstInstrumentTickInfoT;
	typedef std::list<CInstrumentTickInfo*>::iterator				  LstInstrumentTickInfoIterT;

public:
	CDataArrayBuffer(const CDBOperParam&  dbOperParam, const CInstrumentBarInfoRequest& barInfoRequest);
	~CDataArrayBuffer(void);

public:
	int storeBarInfo(int interval, Bar& barInfo);
	int storeTickInfo(Tick& tickInfo);
	int storeMemoryDataToDB();

private:
	int _StoreData();
	int _InitMapIntervalBarLst();
	void _UnInitMapIntervalBarLst();

	int _InitLstInstrumentTickInfo();
	void _UnInitLstInstrumentTickInfo();

	int _InitDBTables();
	int _StoreDataArrayBar(MapIntervalBarLstT* pMapIntervalBarLst);
	void _ClearDataArrayBar(LstInstrumentBarInfoT* pLstInstrumentBarInfo);
	int _StoreDataArrayTick(LstInstrumentTickInfoT* pLstInstrumentTickInfo);
	void _ClearDataArrayTick(LstInstrumentTickInfoT* pLstInstrumentTickInfo);
private:
	MapIntervalBarLstT*   m_pMapIntervalBarLst;
	LstInstrumentTickInfoT* m_pLstInstrumentTickInfo;
private:
	unsigned int       m_nInsertBatchSize;
	unsigned int       m_nInsertBatchIndex;
	CDBOperParam*      m_pDBOperParam;
	CInstrumentBarInfoRequest* m_pBarInfoRequest;
	IDbOper*     m_pDbOper;

};

NS_END(TA_Base_App) 


#endif //__CLASS_DATA_ARRAY_BUFFER__HH__


