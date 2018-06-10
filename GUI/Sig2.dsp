# Microsoft Developer Studio Project File - Name="Sig2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Sig2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Sig2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Sig2.mak" CFG="Sig2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Sig2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Sig2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Sig2 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../Release/Sig.exe"

!ELSEIF  "$(CFG)" == "Sig2 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../Release/Sig.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Sig2 - Win32 Release"
# Name "Sig2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AccountDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoFuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\External\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\External\BrowseCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\External\CmdLine.cpp
# End Source File
# Begin Source File

SOURCE=.\D2PlayerStats.cpp
# End Source File
# Begin Source File

SOURCE=.\D2Spy.cpp
# End Source File
# Begin Source File

SOURCE=.\DicDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\External\EditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\External\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\External\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\External\KCSideBannerWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\External\Logger.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\External\MsgSender.cpp
# End Source File
# Begin Source File

SOURCE=.\External\MsgWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Mule.cpp
# End Source File
# Begin Source File

SOURCE=.\NameDictionary.cpp
# End Source File
# Begin Source File

SOURCE=.\PageD2Exe.cpp
# End Source File
# Begin Source File

SOURCE=.\PageD2H.cpp
# End Source File
# Begin Source File

SOURCE=.\PageFinish.cpp
# End Source File
# Begin Source File

SOURCE=.\PageGame.cpp
# End Source File
# Begin Source File

SOURCE=.\PageHack.cpp
# End Source File
# Begin Source File

SOURCE=.\PageJSP.cpp
# End Source File
# Begin Source File

SOURCE=.\PageLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\PageMule.cpp
# End Source File
# Begin Source File

SOURCE=.\PageName.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSchData.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSchName.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSchRepeat.cpp
# End Source File
# Begin Source File

SOURCE=.\PageThreadType.cpp
# End Source File
# Begin Source File

SOURCE=.\External\PopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\RealmDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RealmEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\External\ReportCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SChDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SchSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\Sig2.cpp
# End Source File
# Begin Source File

SOURCE=.\Sig2.rc
# End Source File
# Begin Source File

SOURCE=.\SIGConfigSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\SIGData.cpp
# End Source File
# Begin Source File

SOURCE=.\SIGDataDef.cpp
# End Source File
# Begin Source File

SOURCE=.\SIGInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\SIGSchedule.cpp
# End Source File
# Begin Source File

SOURCE=.\SigView.cpp
# End Source File
# Begin Source File

SOURCE=.\External\Singleton.cpp
# End Source File
# Begin Source File

SOURCE=.\External\StateTicker.cpp
# End Source File
# Begin Source File

SOURCE=.\StatsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\External\StreamBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\External\StringEx.cpp
# End Source File
# Begin Source File

SOURCE=.\External\SystemTray.cpp
# End Source File
# Begin Source File

SOURCE=.\External\TickThread.cpp
# End Source File
# Begin Source File

SOURCE=.\External\WinAppEx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AccountDlg.h
# End Source File
# Begin Source File

SOURCE=.\AutoFuncs.h
# End Source File
# Begin Source File

SOURCE=.\External\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\External\BrowseCtrl.h
# End Source File
# Begin Source File

SOURCE=.\External\CmdLine.h
# End Source File
# Begin Source File

SOURCE=.\D2PlayerStats.h
# End Source File
# Begin Source File

SOURCE=.\D2Spy.h
# End Source File
# Begin Source File

SOURCE=.\D2WndPosition.h
# End Source File
# Begin Source File

SOURCE=.\DicDlg.h
# End Source File
# Begin Source File

SOURCE=.\External\EditEx.h
# End Source File
# Begin Source File

SOURCE=.\GlobalDlg.h
# End Source File
# Begin Source File

SOURCE=.\External\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\External\Ini.h
# End Source File
# Begin Source File

SOURCE=.\External\KCSideBannerWnd.h
# End Source File
# Begin Source File

SOURCE=.\External\Logger.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\External\MsgSender.h
# End Source File
# Begin Source File

SOURCE=.\External\MsgWnd.h
# End Source File
# Begin Source File

SOURCE=.\Mule.h
# End Source File
# Begin Source File

SOURCE=.\NameDictionary.h
# End Source File
# Begin Source File

SOURCE=.\PageD2Exe.h
# End Source File
# Begin Source File

SOURCE=.\PageD2H.h
# End Source File
# Begin Source File

SOURCE=.\PageFinish.h
# End Source File
# Begin Source File

SOURCE=.\PageGame.h
# End Source File
# Begin Source File

SOURCE=.\PageHack.h
# End Source File
# Begin Source File

SOURCE=.\PageJSP.h
# End Source File
# Begin Source File

SOURCE=.\PageLogin.h
# End Source File
# Begin Source File

SOURCE=.\PageMule.h
# End Source File
# Begin Source File

SOURCE=.\PageName.h
# End Source File
# Begin Source File

SOURCE=.\PageSchData.h
# End Source File
# Begin Source File

SOURCE=.\PageSchName.h
# End Source File
# Begin Source File

SOURCE=.\PageSchRepeat.h
# End Source File
# Begin Source File

SOURCE=.\PageThreadType.h
# End Source File
# Begin Source File

SOURCE=.\External\PopupMenu.h
# End Source File
# Begin Source File

SOURCE=.\RealmDlg.h
# End Source File
# Begin Source File

SOURCE=.\RealmEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\External\RealmInfo.h
# End Source File
# Begin Source File

SOURCE=.\External\ReportCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SChDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\SchSheet.h
# End Source File
# Begin Source File

SOURCE=.\Sig2.h
# End Source File
# Begin Source File

SOURCE=.\SIGConfigSheet.h
# End Source File
# Begin Source File

SOURCE=.\SIGData.h
# End Source File
# Begin Source File

SOURCE=.\SIGDataDef.h
# End Source File
# Begin Source File

SOURCE=.\SIGInstance.h
# End Source File
# Begin Source File

SOURCE=.\SIGSchedule.h
# End Source File
# Begin Source File

SOURCE=.\SigView.h
# End Source File
# Begin Source File

SOURCE=.\External\StateTicker.h
# End Source File
# Begin Source File

SOURCE=.\StatsDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\External\StreamBuffer.h
# End Source File
# Begin Source File

SOURCE=.\External\StringEx.h
# End Source File
# Begin Source File

SOURCE=.\External\SystemTray.h
# End Source File
# Begin Source File

SOURCE=.\External\TickThread.h
# End Source File
# Begin Source File

SOURCE=.\External\WinAppEx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\item.bmp
# End Source File
# Begin Source File

SOURCE=.\res\schbal_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\schbal_e.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Sig2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Sig2.rc2
# End Source File
# Begin Source File

SOURCE=.\res\toolbal_chs.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\External\RemoteLib.lib
# End Source File
# Begin Source File

SOURCE=.\External\RealmInfo.lib
# End Source File
# Begin Source File

SOURCE=.\External\Encrypt.lib
# End Source File
# End Target
# End Project
