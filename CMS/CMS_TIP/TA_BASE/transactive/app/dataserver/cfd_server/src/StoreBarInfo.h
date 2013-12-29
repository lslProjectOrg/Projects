#ifndef __CLASS_STORE_BAR_INFO__HH__
#define __CLASS_STORE_BAR_INFO__HH__

#include "app/dataserver/cfd_server/src/CFDServerCommonData.h"

#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

using namespace cms;


NS_BEGIN(TA_Base_App) 


class CStoreBarInfo 
{
public:
	CStoreBarInfo(void);
	~CStoreBarInfo(void);
};

NS_END(TA_Base_App) 


#endif //__CLASS_STORE_BAR_INFO__HH__


