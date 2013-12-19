#ifndef  __DEF__UTILITIES_COMMON_DEF_H_
#define  __DEF__UTILITIES_COMMON_DEF_H_

#include "core/utilities/src/UtilitiesSysTypes.h"

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

#define  DEF_INT_BUFFSIZE        512
#define  DEF_INT_MAXLINELEN      1024*1024


//---------------------------------------------------------------



NS_END(TA_Base_Core)

#endif  // __DEF__UTILITIES_COMMON_DEF_H_











