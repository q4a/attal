# Microsoft Developer Studio Project File - Name="libfight" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libfight - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libfight.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libfight.mak" CFG="libfight - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libfight - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libfight - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libfight - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\libfight.lib"

!ELSEIF  "$(CFG)" == "libfight - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\libfight_d.lib"

!ENDIF 

# Begin Target

# Name "libfight - Win32 Release"
# Name "libfight - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\fight.cpp
# End Source File
# Begin Source File

SOURCE=.\fightCell.cpp
# End Source File
# Begin Source File

SOURCE=.\fightMap.cpp
# End Source File
# Begin Source File

SOURCE=.\fightMapView.cpp
# End Source File
# Begin Source File

SOURCE=.\fightResult.cpp
# End Source File
# Begin Source File

SOURCE=.\fightUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\graphicalFightCell.cpp
# End Source File
# Begin Source File

SOURCE=.\graphicalFightMap.cpp
# End Source File
# Begin Source File

SOURCE=.\graphicalFightUnit.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\fight.h

!IF  "$(CFG)" == "libfight - Win32 Release"

# Begin Custom Build - moc fight.h
InputPath=.\fight.h

"moc_fight.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc fight.h -o moc_fight.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "libfight - Win32 Debug"

# Begin Custom Build - moc fight.h
InputPath=.\fight.h

"moc_fight.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc fight.h -o moc_fight.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fightCell.h
# End Source File
# Begin Source File

SOURCE=.\fightMap.h
# End Source File
# Begin Source File

SOURCE=.\fightMapView.h

!IF  "$(CFG)" == "libfight - Win32 Release"

# Begin Custom Build - moc fightMapView.h
InputPath=.\fightMapView.h

"moc_fightMapView.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc fightMapView.h -o moc_fightMapView.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "libfight - Win32 Debug"

# Begin Custom Build - moc fightMapView.h
InputPath=.\fightMapView.h

"moc_fightMapView.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc fightMapView.h -o moc_fightMapView.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fightResult.h
# End Source File
# Begin Source File

SOURCE=.\fightUnit.h
# End Source File
# Begin Source File

SOURCE=.\graphicalFightCell.h
# End Source File
# Begin Source File

SOURCE=.\graphicalFightMap.h
# End Source File
# Begin Source File

SOURCE=.\graphicalFightUnit.h
# End Source File
# End Group
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\moc_fight.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_fightMapView.cpp
# End Source File
# End Group
# End Target
# End Project
