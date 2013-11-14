/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution in any form.
  * 
  * Source:    $File: //depot/CODE_4677_T00060500/transactive/app/maintenance_management/mms_agent/src/mq/PubFunction.cpp $
  * @author:   xinsong xia
  * @version:  $Revision: #1 $
  * @see:      9999-Q01-02-0016-GenericAgent_Usage_Guidelines.doc
  *
  * Last modification: $DateTime: 2006/12/08 11:20:56 $
  * Last mofified by:  $Author: buildteam $
  *
  * 
  * This class privide some static public function for MMS Agent
  */

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/filesystem/operations.hpp>

#include "StdPubUtility.h"

#pragma warning(disable:4786)

namespace fs = boost::filesystem;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace TA_Base_Core
{
	
	char * CStdPubUtility::ltrim( char * szSourceText )
	{
		char * szToken = szSourceText; 	
		while(*szToken != '\0' &&(CStdPubUtility::isSpace(*szToken)))
			szToken ++;
		return szToken;
	}

	char * CStdPubUtility::trim( char * szSourceText )
	{
		if (NULL == szSourceText)
			return NULL;

		if (strlen(szSourceText) == 0)
			return szSourceText;

		char * szStart = CStdPubUtility::ltrim(szSourceText);
		char * szReturn = szStart;
		assert(strlen(szStart) >= 0);

		char * szLast = NULL;
		while (*szStart != '\0')
		{
			if (CStdPubUtility::isSpace(*szStart))
			{
				if (NULL == szLast)
					szLast = szStart;
			}
			else
				szLast = NULL;
			szStart ++;
		}

		if (szLast != NULL)
			// truncate at trailing space start
			*szLast = '\0';
		return szReturn;		
	}

    char * CStdPubUtility::seperateText(char ** szText, char ch)
    {
        assert(*szText != NULL);

        if (**szText == '\0')
            return NULL;

        char * szToken = NULL;
        //skip all balnks	
        while(**szText != '\0' && ( (**szText) == ch ) )
            (*szText) ++;

        //forward
        szToken = *szText;
        while( **szText != '\0' && ( **szText != ch ) )
            (*szText)++;

        //seperate the wszText
        if (**szText != '\0')
        {
            **szText = '\0';
            (*szText)++;
        }

        return CStdPubUtility::trim( szToken ); 

    }

	char * CStdPubUtility::cutToToken( char ** szText)
	{
		if (**szText == L'\0')
			return NULL;

		char * szToken = NULL;
		//skip all balnks	
		while(**szText != 0x0 &&(isSpace(**szText)))
			(*szText) ++;

		//forward
		szToken = *szText;
		while(**szText != 0x0 && ( !isSpace(**szText) ) )
			(*szText)++;

		//seperate the wszText
		if (**szText != 0x0 )
		{
			**szText = 0x0 ;
			(*szText)++;
		}

		return szToken;
	}


}