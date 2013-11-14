
#The source code in this file is the property of
#Ripple Systems and is not for redistribution
#in any form.
#
#Source:   $File: //depot/3001_TIP_NEW/TA_BASE/tools/build/build.sh $ 
#Author:  Grace Koh
#Version: $Revision: #2 $
#
#Last modification: $DateTime: 2012/02/06 16:15:14 $
#Last modified by:  $Author: haijun.li $

#!/bin/bash

# Modify these as required.
export ROOT_BASE="/disk2/tabuild1/Nightly_Build/DDBase_Build/Base"

# Prepare the environment.
source /disk2/tabuild1/.profile

# Execute the base build.
cd $ROOT_BASE
ant clean_all p4checkoutCode version build stage
