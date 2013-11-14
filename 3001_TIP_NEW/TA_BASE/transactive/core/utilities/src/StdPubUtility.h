/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/CODE_4677_T00060500/transactive/app/maintenance_management/mms_agent/src/mq/PubFunction.h $
  * @author:  xinsong xia
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2006/12/08 11:20:56 $
  * Last modified by:  $Author: buildteam $
  * 
  * This class privide some static public function for MMS Agent, those function will be refined to form a public class library 
  */

#if !defined(AFX_STD_PUBUTILITY_H__1E5CCA2B_9CF1_4096_A15A_492220F783D6__INCLUDED_)
#define AFX_STD_PUBUTILITY_H__1E5CCA2B_9CF1_4096_A15A_492220F783D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <assert.h>
#include <wchar.h>
#include <time.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

namespace TA_Base_Core
{

	class CStdPubUtility  
	{
	public:
		
		static bool isSpace( char ch )
		{
			if ( '\r' == ch || '\t' == ch || '\n' == ch || ' ' == ch )
				return true;
			return false;

		}
		inline static bool isEmptyString( const char * szText )
		{
			return ( szText == 0x0 || *szText == 0x0 );
		}

		static char * cutToToken( char ** szText);

		static char * ltrim( char * szSourceText );

		static char * trim( char * szSourceText );

        static char * seperateText(char ** szText, char ch);

        static int stricmp(const char* string1, const char* string2) 
        {
            const char* ch1 = string1;
            const char* ch2 = string2;
            int res;

            while (true) 
            {
                res = toupper(*ch1) - toupper(*ch2);
                if (res)
                    return res;
                else
                    if (!*ch1)
                        return 0;

                ch1++;
                ch2++;
            }
        }
	};
	
};

#endif // !defined(AFX_PUBFUNCTION_H__1E5CCA2B_9CF1_4096_A15A_492220F783D6__INCLUDED_)
