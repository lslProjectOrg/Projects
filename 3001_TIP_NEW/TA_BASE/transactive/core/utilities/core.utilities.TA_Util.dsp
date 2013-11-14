# Microsoft Developer Studio Project File - Name="core.utilities.TA_Util" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=core.utilities.TA_Util - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "core.utilities.TA_Util.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "core.utilities.TA_Util.mak" CFG="core.utilities.TA_Util - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "core.utilities.TA_Util - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "core.utilities.TA_Util - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/4 - Engineering/4.20 Common Code/core.utilities.TA_Util", YGFAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "core.utilities.TA_Util - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../build/win32_nr"
# PROP Intermediate_Dir "../../../../build/win32_nr/transactive/core/utilities"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -link
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\..\cots\omniORB\omniORB_3.0.3\include" /I "..\exceptions\src" /I "..\..\core\versioning\src" /I "..\..\core\synchronisation\src" /I "..\..\..\cots\omniORB\omniORB_4.0.5\include" /I "..\..\\" /I ".\src" /I "..\..\..\cots\boost\boost_1_31_0" /I "..\..\..\cots\ACE\5_3\ACE_wrappers" /I "..\..\..\\" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "__WIN32__" /D "__x86__" /D "__NT__" /D __OSVERSION__=4 /D "_AFXDLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../build/win32_nr/TA_Utilities.lib" /ignore:4006

!ELSEIF  "$(CFG)" == "core.utilities.TA_Util - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../build/win32_nd"
# PROP Intermediate_Dir "../../../../build/win32_nd/transactive/core/utilities"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -link
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\..\..\cots\omniORB\omniORB_4.0.5\include" /I "..\..\\" /I ".\src" /I "..\..\..\cots\boost\boost_1_31_0" /I "..\..\..\cots\ACE\5_3\ACE_wrappers" /I "..\..\..\\" /D "_AFXDLL" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "__WIN32__" /D "__x86__" /D "__NT__" /D __OSVERSION__=4 /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../build/win32_nd/TA_Utilities_d.lib" /ignore:4006

!ENDIF 

# Begin Target

# Name "core.utilities.TA_Util - Win32 Release"
# Name "core.utilities.TA_Util - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\DataConversion.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DataConversion.h
# End Source File
# Begin Source File

SOURCE=.\src\DateFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DateFormat.h
# End Source File
# Begin Source File

SOURCE=.\src\DllHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DllHandler.h
# End Source File
# Begin Source File

SOURCE=.\src\Hostname.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Hostname.h
# End Source File
# Begin Source File

SOURCE=.\src\IDllHandler.h
# End Source File
# Begin Source File

SOURCE=.\src\TA_String.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TA_String.h
# End Source File
# Begin Source File

SOURCE=.\src\TAAssert.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TAAssert.h
# End Source File
# Begin Source File

SOURCE=.\src\ThreasdafeScreenOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ThreasdafeScreenOutput.h
# End Source File
# End Group
# Begin Group "DebugUtil"

# PROP Default_Filter ""
# Begin Group "Log Helpers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\GenericLogType.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GenericLogType.h
# End Source File
# Begin Source File

SOURCE=.\src\ILogType.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ILogType.h
# End Source File
# Begin Source File

SOURCE=.\src\Logger.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Logger.h
# End Source File
# Begin Source File

SOURCE=.\src\PreformattedLogType.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PreformattedLogType.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\DebugSETranslator.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DebugSETranslator.h
# End Source File
# Begin Source File

SOURCE=.\src\DebugUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DebugUtil.h
# End Source File
# Begin Source File

SOURCE=.\src\DebugUtilInit.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DebugUtilInit.h
# End Source File
# Begin Source File

SOURCE=.\src\DebugUtilMacros.h
# End Source File
# Begin Source File

SOURCE=.\src\FunctionEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FunctionEntry.h
# End Source File
# End Group
# Begin Group "RunParams"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\RunParams.cpp
# End Source File
# Begin Source File

SOURCE=.\src\RunParams.h
# End Source File
# End Group
# Begin Group "Containers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\src\Matrix.inl
# End Source File
# Begin Source File

SOURCE=.\src\MatrixException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MatrixException.h
# End Source File
# End Group
# Begin Group "time"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\AlarmClock.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AlarmClock.h
# End Source File
# Begin Source File

SOURCE=.\src\IAlarmClockHelper.h
# End Source File
# Begin Source File

SOURCE=.\src\Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Timer.h
# End Source File
# End Group
# End Target
# End Project
