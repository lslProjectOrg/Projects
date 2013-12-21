#ifndef __CLASS_DATA_ACCESS_TEST_MAMAGER__HH__
#define __CLASS_DATA_ACCESS_TEST_MAMAGER__HH__

#include "core/utilities/src/UtilitiesCommonData.h"

NS_BEGIN(TA_Base_Test) 

class CDataAccesssTestWorker;

class CDataAccessTestManager 
{
public:
	CDataAccessTestManager(void);
	~CDataAccessTestManager(void);

public:
	CDataAccesssTestWorker* m_pWorker;
};

NS_END(TA_Base_Test) 


#endif //__CLASS_DATA_ACCESS_TEST_MAMAGER__HH__


