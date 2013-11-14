# Microsoft Developer Studio Project File - Name="core.sockets.TA_Sockets" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=core.sockets.TA_Sockets - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "core.sockets.TA_Sockets.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "core.sockets.TA_Sockets.mak" CFG="core.sockets.TA_Sockets - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "core.sockets.TA_Sockets - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "core.sockets.TA_Sockets - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "core.sockets.TA_Sockets - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../build/win32_nr"
# PROP Intermediate_Dir "../../../../build/win32_nr/transactive/core/sockets"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -link
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\.." /I "..\..\..\cots\ACE\5_3\ACE_wrappers" /I "..\..\..\cots\omniORB\omniORB_4.0.5\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "__WIN32__" /D "__x86__" /D "__NT__" /D __OSVERSION__=4 /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../build/win32_nr/TA_Sockets.lib" /ignore:4006

!ELSEIF  "$(CFG)" == "core.sockets.TA_Sockets - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../build/win32_nd"
# PROP Intermediate_Dir "../../../../build/win32_nd/transactive/core/sockets"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe -link
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /I "..\.." /I "..\..\..\cots\ACE\5_3\ACE_wrappers" /I "..\..\..\cots\omniORB\omniORB_4.0.5\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "__WIN32__" /D "__x86__" /D "__NT__" /D __OSVERSION__=4 /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../build/win32_nd/TA_Sockets_d.lib" /ignore:4006

!ENDIF 

# Begin Target

# Name "core.sockets.TA_Sockets - Win32 Release"
# Name "core.sockets.TA_Sockets - Win32 Debug"
# Begin Group "src"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\Datagram.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Datagram.h
# End Source File
# Begin Source File

SOURCE=.\src\ICMPMessageCodec.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ICMPMessageCodec.h
# End Source File
# Begin Source File

SOURCE=.\src\IDatagramSocket.h
# End Source File
# Begin Source File

SOURCE=.\src\InetAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\src\InetAddress.h
# End Source File
# Begin Source File

SOURCE=.\src\ISocket.h
# End Source File
# Begin Source File

SOURCE=.\src\ITcpSocketObserver.h
# End Source File
# Begin Source File

SOURCE=.\src\RawDatagramSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\src\RawDatagramSocket.h
# End Source File
# Begin Source File

SOURCE=.\src\SocketSet.h
# End Source File
# Begin Source File

SOURCE=.\src\SocketSetP.h
# End Source File
# Begin Source File

SOURCE=.\src\TcpNonblockingSocket.h
# End Source File
# Begin Source File

SOURCE=.\src\TcpObservedSocket.h
# End Source File
# Begin Source File

SOURCE=.\src\TcpServerSocket.h
# End Source File
# Begin Source File

SOURCE=.\src\TcpSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TcpSocket.h
# End Source File
# End Group
# Begin Group "Socket Options"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\AbstractSocketOption.cpp
# End Source File
# Begin Source File

SOURCE=.\src\AbstractSocketOption.h
# End Source File
# Begin Source File

SOURCE=.\src\SocketOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SocketOptions.h
# End Source File
# Begin Source File

SOURCE=.\src\TcpNoDelay.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TcpNoDelay.h
# End Source File
# End Group
# Begin Group "ACE Sockets"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ISocketObserver.h
# End Source File
# Begin Source File

SOURCE=.\src\ObservedSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ObservedSocket.h
# End Source File
# End Group
# End Target
# End Project
