#ifndef __DEF__SYS_TYPES_H_INCLUDED_
#define __DEF__SYS_TYPES_H_INCLUDED_


// # include <stddef.h>
# include <limits.h>
//# include <sys/types.h>
//#include <float.h> 

namespace TA_Base_Test
{

/*
BigInt64
UBigInt64

BigInt32
UBigInt32
*/

	//
#if defined(_MSC_VER) || defined(__BORLANDC__)
	typedef unsigned __int64		BigInt64;//%I64d
	typedef signed __int64			UBigInt64;// %I64u ¡¢ %I64o ¡¢ %I64x
#else
	typedef unsigned long long		BigInt64;
	typedef signed long long		UBigInt64;
#endif



#ifndef __linux__
	typedef long long				BigInt32;//LLONG_MAX LLONG_MIN
	typedef unsigned long long		UBigInt32;//ULLONG_MAX 0 
#else 
	typedef long					BigInt32;//LONG_MIN LONG_MAX
	typedef unsigned long			UBigInt32;//0   ULONG_MAX
#endif


/*


char -128 ~ +127 (1 Byte)
short -32767 ~ + 32768 (2 Bytes)
unsigned short 0 ~ 65536 (2 Bytes)
int -2147483648 ~ +2147483647 (4 Bytes)
unsigned int 0 ~ 4294967295 (4 Bytes)
long == int
long long -9223372036854775808 ~ +9223372036854775807 (8 Bytes)
double 1.7 * 10^308 (8 Bytes)
unsigned int 0¡«4294967295 
long long£º9223372036854775807
long long£º-9223372036854775808
unsigned long long£º1844674407370955161
__int64£º9223372036854775807
__int64£º-9223372036854775808
unsigned __int64£º18446744073709551615


====================


--        --      char      8          -2^7 ~ 2^7-1            %c %c ¡¢ %d ¡¢ %u
signed -- char                    8 -2^7 ~ 2^7-1 %c %c ¡¢ %d ¡¢ %u
unsigned -- char                8 0 ~ 2^8-1 %c %c ¡¢ %d ¡¢ %u
[signed] short [int]            16 -2^15 ~ 2^15-1 %hd
unsigned short [int]           16 0 ~ 2^16-1 %hu ¡¢ %ho ¡¢ %hx
[signed] -- int                    32 -2^31 ~ 2^31-1 %d
unsigned -- [int]                 32 0 ~ 2^32-1 %u ¡¢ %o ¡¢ %x
[signed] long [int]              32 -2^31 ~ 2^31-1 %ld
unsigned long [int]             32 0 ~ 2^32-1 %lu ¡¢ %lo ¡¢ %lx
[signed] long long [int]       64 -2^63 ~ 2^63-1 %I64d
unsigned long long [int]      64 0 ~ 2^64-1 %I64u ¡¢ %I64o ¡¢ %I64x
-- -- float                            32 +/- 3.40282e+038 %f ¡¢ %e ¡¢ %g
-- -- double                        64 +/- 1.79769e+308 %lf ¡¢ %le ¡¢ %lg %f ¡¢ %e ¡¢ %g
-- long double                    96 +/- 1.79769e+308 %Lf ¡¢ %Le ¡¢ %Lg



*/


}//namespace TA_Base_Test



#endif /* __DEF__SYS_TYPES_H_INCLUDED_ */

