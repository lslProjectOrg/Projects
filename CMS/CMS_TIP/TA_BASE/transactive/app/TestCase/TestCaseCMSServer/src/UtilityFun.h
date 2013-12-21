#ifndef __DEF__UTILITY_FUN_H__
#define __DEF__UTILITY_FUN_H__ 



#include "CommonDef.h"
#include "CommonData.h"



//////////////////////////////////////////////////////////////////////////



// #ifdef WIN32
// #include <objbase.h>
// #endif //WIN32

//
#include "boost/thread.hpp"
#include "boost/thread/xtime.hpp"
#include "boost/thread/tss.hpp"
//
#include "boost/chrono.hpp"
//
#include "boost/date_time/posix_time/posix_time.hpp" 
#include "boost/date_time/c_time.hpp" 
#include "boost/date_time/date.hpp" 
#include "boost/date_time/time.hpp" 
//
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/filesystem/exception.hpp"
#include "boost/filesystem.hpp"
//
#include "boost/lexical_cast.hpp" 
#include "boost/tokenizer.hpp"
//
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
//
#include "boost/algorithm/string.hpp"
//

//


//#include <boost/test/unit_test.hpp>


using namespace boost::posix_time; 
using namespace boost::gregorian; 


//////////////////////////////////////////////////////////////////////////


NS_BEGIN(TA_Base_Test)


class CUtilityFun
{
public:
	CUtilityFun(void);	
	~CUtilityFun(void);

public:
	std::string		mysys_boost_GetTimeStr(unsigned int uTimeVale);
	double			mysys_boost_strTodouble(std::string& strNumber);
	int				mysys_boost_strToint(std::string& strNumber);
	std::string		mysys_boost_intTostr(int nVal);
	std::string     mysys_boost_unsignedintTostr(unsigned int nVal);
	int				mysys_boost_stricmp(std::string& strLeft, std::string& strRight);
	int				mysys_boost_trim(std::string& strSrc);
	int				mysys_boost_ToUpper(std::string &strSrc);
	int				mysys_boost_ToLwr(std::string &strSrc);
	void            mysys_boost_sleep(int ms);
	std::string     mysys_boost_generateUUID();

};


NS_END(TA_Base_Test)


#endif // __DEF__UTILITY_FUN_H__









