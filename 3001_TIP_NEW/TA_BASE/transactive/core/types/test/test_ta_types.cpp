/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/types/test/test_ta_types.cpp $
  * @author:  San Teo
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * Contains definitions of platform independent datatypes
  * that should be used in all TransActive softwares to 
  * ensure code could be used cross platform.
  * 
  */

#include <string>
#include <sstream>
#include "ta_types.h"

int main()
{
	std::ostringstream out;
#if defined( LINUX ) || defined( __linux__ )
	out << LINUX\n\n";
#elif defined( WIN32 ) || defined( __WIN32__ )
	out << "WINDOWS\n\n";
#elif defined( SOLARIS ) || defined( __solaris__ )
	out << "SOLARIS\n\n";
#endif	

	out << "size of 8 bits long signed integer: " << sizeof(ta_int8) << " ... " << ((sizeof(ta_int8)==1)?"OK":"ERROR!") << "\n";
	out << "size of 16 bits long signed integer: " << sizeof(ta_int16) << " ... " << ((sizeof(ta_int16)==2)?"OK":"ERROR!") << "\n";
	out << "size of 32 bits long signed integer: " << sizeof(ta_int32) << " ... " << ((sizeof(ta_int32)==4)?"OK":"ERROR!") << "\n";
	out << "size of 64 bits long signed integer: " << sizeof(ta_int64) << " ... " << ((sizeof(ta_int64)==8)?"OK":"ERROR!") << "\n";

	out << "size of 8 bits long unsigned integer: " << sizeof(ta_uint8) << " ... " << ((sizeof(ta_uint8)==1)?"OK":"ERROR!") << "\n";
	out << "size of 16 bits long unsigned integer: " << sizeof(ta_uint16) << " ... " << ((sizeof(ta_uint16)==2)?"OK":"ERROR!") << "\n";
	out << "size of 32 bits long unsigned integer: " << sizeof(ta_uint32) << " ... " << ((sizeof(ta_uint32)==4)?"OK":"ERROR!") << "\n";
//	out << "size of 64 bits long unsigned integer: " << sizeof(ta_uint64) << " ... " << ((sizeof(ta_uint64)==8)?"OK":"ERROR!") << "\n";

	out << "single, 32 bytes long float: " << sizeof(ta_float32) << " ... " << ((sizeof(ta_float32)==4)?"OK":"ERROR!") << "\n";
	out << "double, 64 bytes long float: " << sizeof(ta_float64) << " ... " << ((sizeof(ta_float64)==8)?"OK":"ERROR!") << "\n";
//	out << "extended, 80 bytes long float: " << sizeof(ta_float80)<< " ... " << ((sizeof(ta_float80)==80)?"OK":"ERROR!") << "\n";

	
	out << "int8: max size: " << TA_MAX_INT8 << " min size: " << TA_MIN_INT8 << "\n";
	out << "int16: max size: " << TA_MAX_INT16 << " min size: " << TA_MIN_INT16 << "\n";
	out << "int32: max size: " << TA_MAX_INT32 << " min size: " << TA_MIN_INT32 << "\n";
//	out << "int64: max size: " << TA_MAX_INT64 << " min size: " << TA_MIN_INT64 << "\n";

	out << "uint8: max size: " << TA_MAX_UINT8 << " min size: " << TA_MIN_UINT8 << "\n";
	out << "uint16: max size: " << TA_MAX_UINT16 << " min size: " << TA_MIN_UINT16 << "\n";
//	out << "uint32: max size: " << TA_MAX_UINT32 << " min size: " << TA_MIN_UINT32 << "\n";
//	out << "uint64: max size: " << TA_MAX_UINT64 << " min size: " << TA_MIN_UINT64 << "\n";

	// as << doesn't support 32 unsigned integer and 64 bits integers, just look 
	// at these values in the debugger and make sure they're correct.
	ta_uint32 umax_32 = TA_MAX_UINT32;  // 4294967295
	ta_uint32 umin_32 = TA_MIN_UINT32;  // 0

#if __WORDSIZE==64
	ta_int64 nmax_64 = TA_MAX_INT64;  // 9223372036854775807
	ta_int64 nmin_64 = TA_MIN_INT64;  // -9223372036854775808
	ta_uint64 umax_64 = TA_MAX_UINT64;  // 18446744073709551615
	ta_uint64 umin_64 = TA_MIN_UINT64;  // 0
#endif

	std::string str = out.str();
	printf(str.c_str());
	return 0;
}

