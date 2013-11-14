/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/core/utilities/src/DebugUtilInit.cpp $
  * @author:  Lachlan Partington
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  * 
  */

#include "core/utilities/src/DebugUtilInit.h"

namespace TA_Base_Core
{
    void gSetDebugUtilFromRunParams()
    {
        // Set the debug file name
        std::string param = RunParams::getInstance().get(RPARAM_DEBUGFILE);
        if (!param.empty())
        {
        	LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "setting DebugFile=%s", param.c_str());
            DebugUtil::getInstance().setFile(param.c_str());
        }

        // Set the debug level
        param = RunParams::getInstance().get(RPARAM_DEBUGLEVEL);
        if (!param.empty())
        {
        	LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "setting DebugLevel=%s", param.c_str());
            DebugUtil::getInstance().setLevel(DebugUtil::getDebugLevelFromString(param.c_str()));
        }

        // Set the maximum size of a debug file in bytes
        param = RunParams::getInstance().get(RPARAM_DEBUGFILEMAXSIZE);
        if (!param.empty())
        {
        	LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "setting DebugFileMaxSize=%s", param.c_str());
            DebugUtil::getInstance().setMaxSize(atoi(param.c_str()));
        }

        // Set the maximum number of log files
        param = RunParams::getInstance().get(RPARAM_DEBUGMAXFILES);
        if (!param.empty())
        {
        	LOG1(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "setting DebugMaxFiles=%s", param.c_str());
            DebugUtil::getInstance().setMaxFiles(atoi(param.c_str()));
        }

		// Use Pid encoding in filenames
        param = TA_Base_Core::RunParams::getInstance().get(RPARAM_DEBUGPIDFILENAMES);
        if (!param.empty())
        {
            TA_Base_Core::DebugUtil::getInstance().encryptPidInFilenames(param);
        }

		// Use debug level specific log files
        param = TA_Base_Core::RunParams::getInstance().get(RPARAM_DEBUGLEVELFILE);
        if (!param.empty())
        {
            TA_Base_Core::DebugUtil::getInstance().decodeLevelFilenames(param);
        }

		// Turn off logging for debug levels
        param = TA_Base_Core::RunParams::getInstance().get(RPARAM_DEBUGLEVELOFF);
        if (!param.empty())
        {
            TA_Base_Core::DebugUtil::getInstance().decodeDisabledLevels(param);
        }
    }
}



