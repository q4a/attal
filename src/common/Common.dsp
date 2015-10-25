# Microsoft Developer Studio Project File - Name="Common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Common - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak" CFG="Common - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Common - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Common - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Common - Win32 Release"

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
# ADD LIB32 /nologo /out:"..\common.lib"

!ELSEIF  "$(CFG)" == "Common - Win32 Debug"

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
# ADD LIB32 /nologo /out:"..\common_d.lib"

!ENDIF 

# Begin Target

# Name "Common - Win32 Release"
# Name "Common - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\action.cpp
# End Source File
# Begin Source File

SOURCE=.\artefact.cpp
# End Source File
# Begin Source File

SOURCE=.\attalSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\calendar.cpp
# End Source File
# Begin Source File

SOURCE=.\categoryManager.cpp
# End Source File
# Begin Source File

SOURCE=.\condition.cpp
# End Source File
# Begin Source File

SOURCE=.\creature.cpp
# End Source File
# Begin Source File

SOURCE=.\dataTheme.cpp
# End Source File
# Begin Source File

SOURCE=.\genericBase.cpp
# End Source File
# Begin Source File

SOURCE=.\genericBuilding.cpp
# End Source File
# Begin Source File

SOURCE=.\genericCell.cpp
# End Source File
# Begin Source File

SOURCE=.\genericDecoration.cpp
# End Source File
# Begin Source File

SOURCE=.\genericEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\genericFightCell.cpp
# End Source File
# Begin Source File

SOURCE=.\genericFightMap.cpp
# End Source File
# Begin Source File

SOURCE=.\genericInsideBuilding.cpp
# End Source File
# Begin Source File

SOURCE=.\genericLord.cpp
# End Source File
# Begin Source File

SOURCE=.\genericMap.cpp
# End Source File
# Begin Source File

SOURCE=.\genericMapCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\genericPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\genericRessources.cpp
# End Source File
# Begin Source File

SOURCE=.\genericTeam.cpp
# End Source File
# Begin Source File

SOURCE=.\groupName.cpp
# End Source File
# Begin Source File

SOURCE=.\log.cpp
# End Source File
# Begin Source File

SOURCE=.\pathFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\priceMarket.cpp
# End Source File
# Begin Source File

SOURCE=.\quest.cpp
# End Source File
# Begin Source File

SOURCE=.\skill.cpp
# End Source File
# Begin Source File

SOURCE=.\specialty.cpp
# End Source File
# Begin Source File

SOURCE=.\specifications.cpp
# End Source File
# Begin Source File

SOURCE=.\technic.cpp
# End Source File
# Begin Source File

SOURCE=.\unit.cpp
# End Source File
# Begin Source File

SOURCE=.\warMachine.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\action.h
# End Source File
# Begin Source File

SOURCE=.\artefact.h
# End Source File
# Begin Source File

SOURCE=.\attalSocket.h
# End Source File
# Begin Source File

SOURCE=.\calendar.h

!IF  "$(CFG)" == "Common - Win32 Release"

# Begin Custom Build - moc calendar.h
InputPath=.\calendar.h

"moc_calendar.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc calendar.h -o moc_calendar.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Common - Win32 Debug"

# Begin Custom Build - moc calendar.h
InputPath=.\calendar.h

"moc_calendar.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc calendar.h -o moc_calendar.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\categoryManager.h
# End Source File
# Begin Source File

SOURCE=.\condition.h
# End Source File
# Begin Source File

SOURCE=.\creature.h
# End Source File
# Begin Source File

SOURCE=.\dataTheme.h
# End Source File
# Begin Source File

SOURCE=.\genericBase.h
# End Source File
# Begin Source File

SOURCE=.\genericBuilding.h
# End Source File
# Begin Source File

SOURCE=.\genericCell.h
# End Source File
# Begin Source File

SOURCE=.\genericDecoration.h
# End Source File
# Begin Source File

SOURCE=.\genericEvent.h
# End Source File
# Begin Source File

SOURCE=.\genericFightCell.h
# End Source File
# Begin Source File

SOURCE=.\genericFightMap.h
# End Source File
# Begin Source File

SOURCE=.\genericInsideBuilding.h
# End Source File
# Begin Source File

SOURCE=.\genericLord.h
# End Source File
# Begin Source File

SOURCE=.\genericMap.h
# End Source File
# Begin Source File

SOURCE=.\genericMapCreature.h
# End Source File
# Begin Source File

SOURCE=.\genericPlayer.h
# End Source File
# Begin Source File

SOURCE=.\genericRessources.h
# End Source File
# Begin Source File

SOURCE=.\genericTeam.h
# End Source File
# Begin Source File

SOURCE=.\groupName.h
# End Source File
# Begin Source File

SOURCE=.\log.h

!IF  "$(CFG)" == "Common - Win32 Release"

# Begin Custom Build - moc log.h
InputPath=.\log.h

"moc_log.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc log.h -o moc_log.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "Common - Win32 Debug"

# Begin Custom Build - moc log.h
InputPath=.\log.h

"moc_log.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc log.h -o moc_log.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pathFinder.h
# End Source File
# Begin Source File

SOURCE=.\priceMarket.h
# End Source File
# Begin Source File

SOURCE=.\quest.h
# End Source File
# Begin Source File

SOURCE=.\skill.h
# End Source File
# Begin Source File

SOURCE=.\specialty.h
# End Source File
# Begin Source File

SOURCE=.\specifications.h
# End Source File
# Begin Source File

SOURCE=.\technic.h
# End Source File
# Begin Source File

SOURCE=.\unit.h
# End Source File
# Begin Source File

SOURCE=.\warMachine.h
# End Source File
# End Group
# Begin Group "Moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\moc_calendar.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_log.cpp
# End Source File
# End Group
# End Target
# End Project
