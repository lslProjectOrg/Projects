# Microsoft Developer Studio Project File - Name="core.exceptions.TA_Exceptions" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=core.exceptions.TA_Exceptions - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "core.exceptions.TA_Exceptions.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "core.exceptions.TA_Exceptions.mak" CFG="core.exceptions.TA_Exceptions - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "core.exceptions.TA_Exceptions - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "core.exceptions.TA_Exceptions - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "core.exceptions.TA_Exceptions"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "core.exceptions.TA_Exceptions - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\build\win32_nr"
# PROP Intermediate_Dir "..\..\..\..\build\win32_nr\transactive\core\exceptions"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -link
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../../../build/win32_n/transactive/" /I "../../" /I "..\..\..\cots\omniORB\omniORB_4.0.5\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "__WIN32__" /D "__x86__" /D "__NT__" /D __OSVERSION__=4 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\build\win32_nr\TA_Exceptions.lib" /ignore:4006

!ELSEIF  "$(CFG)" == "core.exceptions.TA_Exceptions - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\build\win32_nd"
# PROP Intermediate_Dir "..\..\..\..\build\win32_nd\transactive\core\exceptions"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -link
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "../../../../build/win32_n/transactive/" /I "../../" /I "..\..\..\cots\omniORB\omniORB_4.0.5\include" /D "__WIN32__" /D "__x86__" /D "__NT__" /D __OSVERSION__=4 /D "WIN32" /D "_MBCS" /D "_LIB" /D "_DEBUG" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\build\win32_nd\TA_Exceptions_d.lib" /ignore:4006

!ENDIF 

# Begin Target

# Name "core.exceptions.TA_Exceptions - Win32 Release"
# Name "core.exceptions.TA_Exceptions - Win32 Debug"
# Begin Group "core"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\AssertException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AssertException.h
# End Source File
# Begin Source File

SOURCE=.\src\CorbaException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CorbaException.h
# End Source File
# Begin Source File

SOURCE=.\src\DebugMsgConstructionException.h
# End Source File
# Begin Source File

SOURCE=.\src\ObjectResolutionException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ObjectResolutionException.h
# End Source File
# Begin Source File

SOURCE=.\src\TransactiveException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TransactiveException.h
# End Source File
# Begin Source File

SOURCE=.\src\UtilitiesException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\UtilitiesException.h
# End Source File
# End Group
# Begin Group "database"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\DatabaseException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DatabaseException.h
# End Source File
# Begin Source File

SOURCE=.\src\DbConnectionFailed.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DbConnectionFailed.h
# End Source File
# Begin Source File

SOURCE=.\src\DbConnectionNoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DbConnectionNoFile.h
# End Source File
# Begin Source File

SOURCE=.\src\DbUniqueConstraintViolated.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DbUniqueConstraintViolated.h
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ArchiveException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ArchiveException.h
# End Source File
# Begin Source File

SOURCE=.\src\BannerFrameworkException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\BannerFrameworkException.h
# End Source File
# Begin Source File

SOURCE=.\src\CtaRuntimeEntityParameterNotFoundException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CtaRuntimeEntityParameterNotFoundException.h
# End Source File
# Begin Source File

SOURCE=.\src\CtaScadaExceptions.h
# End Source File
# Begin Source File

SOURCE=.\src\DataConfigurationException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DataConfigurationException.h
# End Source File
# Begin Source File

SOURCE=.\src\DataException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DataException.h
# End Source File
# Begin Source File

SOURCE=.\src\DataUserSettingException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DataUserSettingException.h
# End Source File
# Begin Source File

SOURCE=.\src\EntityTypeException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\EntityTypeException.h
# End Source File
# Begin Source File

SOURCE=.\src\GraphWorxException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GraphWorxException.h
# End Source File
# Begin Source File

SOURCE=.\src\HistoryInitialisationException.h
# End Source File
# Begin Source File

SOURCE=.\src\InterlockingException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\InterlockingException.h
# End Source File
# Begin Source File

SOURCE=.\src\MathematicalEvaluationException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MathematicalEvaluationException.h
# End Source File
# Begin Source File

SOURCE=.\src\MessageResourceException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MessageResourceException.h
# End Source File
# Begin Source File

SOURCE=.\src\NoAlarmAgentException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NoAlarmAgentException.h
# End Source File
# Begin Source File

SOURCE=.\src\PlanAgentException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PlanAgentException.h
# End Source File
# Begin Source File

SOURCE=.\src\PlanStepDetailException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PlanStepDetailException.h
# End Source File
# Begin Source File

SOURCE=.\src\PlanStepException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PlanStepException.h
# End Source File
# Begin Source File

SOURCE=.\src\ReportManagerException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ReportManagerException.h
# End Source File
# Begin Source File

SOURCE=.\src\ScadaCommonException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ScadaCommonException.h
# End Source File
# Begin Source File

SOURCE=.\src\ScadaProxyException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ScadaProxyException.h
# End Source File
# Begin Source File

SOURCE=.\src\SchedulingManagerException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SchedulingManagerException.h
# End Source File
# Begin Source File

SOURCE=.\src\SocketException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SocketException.h
# End Source File
# Begin Source File

SOURCE=.\src\SoundPlayerException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SoundPlayerException.h
# End Source File
# Begin Source File

SOURCE=.\src\TcpSocketException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TcpSocketException.h
# End Source File
# Begin Source File

SOURCE=.\src\TrendViewerException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TrendViewerException.h
# End Source File
# Begin Source File

SOURCE=.\src\UserCancelException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\UserCancelException.h
# End Source File
# Begin Source File

SOURCE=.\src\UserSettingsException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\UserSettingsException.h
# End Source File
# Begin Source File

SOURCE=.\src\ValueNotSetException.h
# End Source File
# End Group
# Begin Group "obsolete"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\CCTVException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CCTVException.h
# End Source File
# Begin Source File

SOURCE=.\src\DutyManagerException.h
# End Source File
# Begin Source File

SOURCE=.\src\EventViewerException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\EventViewerException.h
# End Source File
# Begin Source File

SOURCE=.\src\FailoverException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FailoverException.h
# End Source File
# Begin Source File

SOURCE=.\src\GenericApplicationException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GenericApplicationException.h
# End Source File
# Begin Source File

SOURCE=.\src\NameNotFoundException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NameNotFoundException.h
# End Source File
# Begin Source File

SOURCE=.\src\PlanControllerException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PlanControllerException.h
# End Source File
# Begin Source File

SOURCE=.\src\PlanEditorException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PlanEditorException.h
# End Source File
# Begin Source File

SOURCE=.\src\SoundInitException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SoundInitException.h
# End Source File
# Begin Source File

SOURCE=.\src\ThreadException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ThreadException.h
# End Source File
# End Group
# Begin Group "alarm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\AlarmActionException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AlarmActionException.h
# End Source File
# Begin Source File

SOURCE=.\src\AlarmAgentException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AlarmAgentException.h
# End Source File
# Begin Source File

SOURCE=.\src\AlarmCacheCapacityExceededException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AlarmCacheCapacityExceededException.h
# End Source File
# Begin Source File

SOURCE=.\src\AlarmListCtrlException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AlarmListCtrlException.h
# End Source File
# Begin Source File

SOURCE=.\src\AlarmStoreException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AlarmStoreException.h
# End Source File
# Begin Source File

SOURCE=.\src\AlarmViewerException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AlarmViewerException.h
# End Source File
# End Group
# Begin Group "4669"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\AtsAgentException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AtsAgentException.h
# End Source File
# Begin Source File

SOURCE=.\src\DataRecordingAgentException.h
# End Source File
# Begin Source File

SOURCE=.\src\ECSAgentExceptions.h
# End Source File
# Begin Source File

SOURCE=.\src\IncidentException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\IncidentException.h
# End Source File
# Begin Source File

SOURCE=.\src\PasExceptions.h
# End Source File
# Begin Source File

SOURCE=.\src\VideoSwitchAgentInternalException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VideoSwitchAgentInternalException.h
# End Source File
# Begin Source File

SOURCE=.\src\WILDAgentExceptions.h
# End Source File
# End Group
# Begin Group "system control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ApplicationException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ApplicationException.h
# End Source File
# Begin Source File

SOURCE=.\src\CannotUseSysControllerException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\CannotUseSysControllerException.h
# End Source File
# Begin Source File

SOURCE=.\src\FatalConfigurationException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FatalConfigurationException.h
# End Source File
# Begin Source File

SOURCE=.\src\GenericAgentException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GenericAgentException.h
# End Source File
# Begin Source File

SOURCE=.\src\GenericGUIException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GenericGUIException.h
# End Source File
# Begin Source File

SOURCE=.\src\InvalidCommandLineException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\InvalidCommandLineException.h
# End Source File
# Begin Source File

SOURCE=.\src\ManagedProcessException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ManagedProcessException.h
# End Source File
# Begin Source File

SOURCE=.\src\ProcessAlreadyRunningException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ProcessAlreadyRunningException.h
# End Source File
# Begin Source File

SOURCE=.\src\SessionException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SessionException.h
# End Source File
# Begin Source File

SOURCE=.\src\SystemControllerException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SystemControllerException.h
# End Source File
# End Group
# Begin Group "security"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\AuthenticationAgentException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AuthenticationAgentException.h
# End Source File
# Begin Source File

SOURCE=.\src\AuthenticationSecurityException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AuthenticationSecurityException.h
# End Source File
# Begin Source File

SOURCE=.\src\DutyAgentException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\DutyAgentException.h
# End Source File
# Begin Source File

SOURCE=.\src\PolicyDecisionPointException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PolicyDecisionPointException.h
# End Source File
# Begin Source File

SOURCE=.\src\RightsException.cpp
# End Source File
# Begin Source File

SOURCE=.\src\RightsException.h
# End Source File
# End Group
# End Target
# End Project
