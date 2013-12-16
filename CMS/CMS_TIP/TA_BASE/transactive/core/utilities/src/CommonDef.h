#ifndef  __DEF__COMMON_DEF_HH__
#define  __DEF__COMMON_DEF_HH__

#include "SysTypes.h"

//////////////////////////////////////////////////////////////////////////
///#define
//////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------
// easy usage of the namespace identifier
#define  NS_BEGIN(name)			namespace name {
#define  NS_END(name)			};
//---------------------------------------------------------------




NS_BEGIN(TA_Base_Core) 

//-function
//---------------------------------------------------------------

#define DEF_DELETE_ARRAY(pArray)		if (pArray) { delete [](pArray); (pArray) = NULL; }
#define DEF_DELETE(p)					if (p) { delete (p); (p) = NULL; }

//---------------------------------------------------------------



//---------------------------------------------------------------



NS_END(TA_Base_Core)

#endif  // __DEF__COMMON_DEF_HH__











