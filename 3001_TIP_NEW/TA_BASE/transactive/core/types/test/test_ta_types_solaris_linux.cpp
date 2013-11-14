/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/types/test/test_ta_types_solaris_linux.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * <description>
  *
  */

#include <string>
#include <iostream>
#include <stdio.h>
#define LINUX
//#define __WORDSIZE 64 


#include "ta_types.h"


int main()
{
#if defined( LINUX ) || defined( __linux__ )
	printf("LINUX\n\n");
#elif defined( WIN32 ) || defined( __WIN32__ )
	printf("WINDOWS\n\n");
#elif defined( SOLARIS ) || defined( __solaris__ )
	printf("SOLARIS\n\n");
#endif	
	printf( "size of 8 bits long signed integer: %d ... %s\n", sizeof(ta_int8), ((sizeof(ta_int8)==1)?"OK":"ERROR!"));
	printf( "size of 16 bits long signed integer: %d ... %s\n", sizeof(ta_int16), ((sizeof(ta_int16)==2)?"OK":"ERROR!"));
	printf( "size of 32 bits long signed integer: %d ... %s\n", sizeof(ta_int32), ((sizeof(ta_int32)==4)?"OK":"ERROR!"));

	printf( "size of 8 bits long unsigned integer: %d ... %s\n", sizeof(ta_uint8), ((sizeof(ta_uint8)==1)?"OK":"ERROR!"));
	printf( "size of 16 bits long unsigned integer: %d ... %s\n", sizeof(ta_uint16), ((sizeof(ta_uint16)==2)?"OK":"ERROR!"));
	printf( "size of 32 bits long unsigned integer: %d ... %s\n", sizeof(ta_uint32), ((sizeof(ta_uint32)==4)?"OK":"ERROR!"));
	

	printf( "single, 4 bytes long float: %d ... %s\n", sizeof(ta_float32), ((sizeof(ta_float32)==4)?"OK":"ERROR!"));
	//printf( "double, 8 bytes long float:  %d ... %s\n", sizeof(ta_float64), ((sizeof(ta_float64)==8)?"OK":"ERROR!"));

	// as << doesn't support 32 unsigned integer and 64 bits integers, just look 
	// at these values in the debugger and make sure they're correct.
	ta_uint32 umax_32 = TA_MAX_UINT32;  // 4294967295
	ta_uint32 umin_32 = TA_MIN_UINT32;  // 0

#if __WORDSIZE==64
	//printf("size of 64 bits long signed integer: %d ... %s\n", sizeof(ta_int64), ((sizeof(ta_int64)==8)?"OK":"ERROR!"));
	//printf( "size of 64 bits long unsigned integer: %d ... %s\n", sizeof(ta_uint64), ((sizeof(ta_uint64)==8)?"OK":"ERROR!"));
	ta_int64 nmax_64 = TA_MAX_INT64;  // 9223372036854775807
	ta_int64 nmin_64 = TA_MIN_INT64;  // -9223372036854775808
	ta_uint64 umax_64 = TA_MAX_UINT64;  // 18446744073709551615
	ta_uint64 umin_64 = TA_MIN_UINT64;  // 0
#endif

	return 0;
}

