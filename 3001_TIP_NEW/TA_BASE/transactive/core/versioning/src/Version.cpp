/**
* The source code in this file is the property of
* MI Consulting Group (Australia) and is not for
* redistribution in any form.
*
# Source:  $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/versioning/src/Version.cpp $ 
# Author:  Ripple
# Version: $Revision: #2 $
#
# Last modification: $DateTime: 2012/02/06 16:15:14 $
# Last modified by:  $Author: haijun.li $
*
* Main source file for Version information
*
* NOTE: The variables declared externall in Version.cpp
* are defined in the file VersionGen.cpp which is located in
* all component src directories. The variables are resolved
* at link time.
*/

#include <iostream>

#include "Version.h"

namespace TA_Base_Core
{
    // These variables are all declared external so they
    // will be picked up on link time with individual applications
	extern char componentName[];
	extern char componentVersion[];

    extern char buildVersion[];
	extern char buildDate[];
	extern int headerArraySize;
	extern char* headerArray[];
	extern int libraryArraySize;
	extern char* libraryArray[];

    extern int otherArraySize;
    extern char* otherArray[];

    /**
      * displayVersionInfo()
      *
      * This is a static method that will print out versionInformation to stdout.
      */
    void Version::displayVersionInfo()
    {
        // Print out the header information (copyright information and such)
        for( int j = 0; j < headerArraySize; j++)
        {
            std::cout << headerArray[j] << std::endl;
        }
        std::cout << std::endl;

        // Print out all the details relevant to this particular application
        std::cout << "Release Number:\t\t" << buildVersion << std::endl;
        std::cout << "Build Time:\t\t" << buildDate << std::endl;
        std::cout << "Component:\t\t" << componentName << std::endl;
        std::cout << "Component Version:\t" << componentVersion << std::endl << std::endl;

        // Print out all the relevant library information
        std::cout << "Component Library Information:" << std::endl;
        for (int i = 0; i < libraryArraySize; i++)
        {
            std::cout << "\t" << libraryArray[i] << std::endl;
        }
        std::cout << std::endl;

        // Print out any other information for this application
        std::cout << "Other Component Information:" << std::endl;
        for(  int k = 0; k < otherArraySize; k++)
        {
            std::cout << "\t" << otherArray[k] << std::endl;
        }
    }
}
