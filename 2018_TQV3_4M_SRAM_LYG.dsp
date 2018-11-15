# Microsoft Developer Studio Project File - Name="2018_TQV3_4M_SRAM_LYG" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=2018_TQV3_4M_SRAM_LYG - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "2018_TQV3_4M_SRAM_LYG.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "2018_TQV3_4M_SRAM_LYG.mak" CFG="2018_TQV3_4M_SRAM_LYG - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "2018_TQV3_4M_SRAM_LYG - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "2018_TQV3_4M_SRAM_LYG - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "2018_TQV3_4M_SRAM_LYG - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "2018_TQV3_4M_SRAM_LYG - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "2018_TQV3_4M_SRAM_LYG - Win32 Release"
# Name "2018_TQV3_4M_SRAM_LYG - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\address_topo.cpp
# End Source File
# Begin Source File

SOURCE=.\data_topo.cpp
# End Source File
# Begin Source File

SOURCE=.\dialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\host_begin.cpp
# End Source File
# Begin Source File

SOURCE=.\Margin.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiDialog.rc
# End Source File
# Begin Source File

SOURCE=.\pattern1.cpp
# End Source File
# Begin Source File

SOURCE=.\pattern1_MAG2.cpp
# End Source File
# Begin Source File

SOURCE=.\pin_assignments.cpp
# End Source File
# Begin Source File

SOURCE=.\pin_lists.cpp
# End Source File
# Begin Source File

SOURCE=.\pin_scramble.cpp
# End Source File
# Begin Source File

SOURCE=.\seq_and_bin.cpp
# End Source File
# Begin Source File

SOURCE=.\site_begin.cpp
# End Source File
# Begin Source File

SOURCE=.\system_initial.cpp
# End Source File
# Begin Source File

SOURCE=.\test_blocks.cpp
# End Source File
# Begin Source File

SOURCE=.\tester.cpp
# End Source File
# Begin Source File

SOURCE=.\vihh_maps.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\file.h
# End Source File
# Begin Source File

SOURCE=.\pattern1.h
# End Source File
# Begin Source File

SOURCE=.\patterns.h
# End Source File
# Begin Source File

SOURCE=.\pin_lists.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\tester.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\apfp.ini
# End Source File
# Begin Source File

SOURCE=.\pattern1.pat

!IF  "$(CFG)" == "2018_TQV3_4M_SRAM_LYG - Win32 Release"

!ELSEIF  "$(CFG)" == "2018_TQV3_4M_SRAM_LYG - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\pattern1.pat

"pattern1_MAG2.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo Compiling pattern1.pat for Magnum2 & patcom   -P"Debug\2018_TQV3_4M_SRAM_LYG.exe" -magnum2 pattern1.pat

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
