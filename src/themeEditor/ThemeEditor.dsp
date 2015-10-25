# Microsoft Developer Studio Project File - Name="ThemeEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ThemeEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ThemeEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ThemeEditor.mak" CFG="ThemeEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ThemeEditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ThemeEditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ThemeEditor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib imm32.lib uuid.lib ..\common.lib ..\libattal.lib qt-mt311.lib qtmain.lib /nologo /subsystem:windows /incremental:yes /machine:I386 /out:"../ThemeEditor.exe"

!ELSEIF  "$(CFG)" == "ThemeEditor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib imm32.lib uuid.lib ..\common_d.lib ..\libattal_d.lib qt232.lib qtmain.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"c" /nodefaultlib:"libc" /out:"..\themeEditor_d.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ThemeEditor - Win32 Release"
# Name "ThemeEditor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\sectionArtefact.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionBase.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionBuilding.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionDecoration.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionGeneral.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionGround.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionLord.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionRessource.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionTeam.cpp
# End Source File
# Begin Source File

SOURCE=.\sectionWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\themeEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\themeInterface.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\sectionArtefact.h

!IF  "$(CFG)" == "ThemeEditor - Win32 Release"

# Begin Custom Build - moc sectionArtefact.h
InputPath=.\sectionArtefact.h

"moc_sectionArtefcat.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionArtefact.h -o moc_sectionArtefcat.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ThemeEditor - Win32 Debug"

# Begin Custom Build - moc sectionArtefact.h
InputPath=.\sectionArtefact.h

"moc_sectionArtefcat.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionArtefact.h -o moc_sectionArtefcat.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sectionBase.h
# End Source File
# Begin Source File

SOURCE=.\sectionBuilding.h
# End Source File
# Begin Source File

SOURCE=.\sectionCreature.h

!IF  "$(CFG)" == "ThemeEditor - Win32 Release"

# Begin Custom Build - moc sectionCreature.h
InputPath=.\sectionCreature.h

"moc_sectionCreature.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionCreature.h -o moc_sectionCreature.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ThemeEditor - Win32 Debug"

# Begin Custom Build - moc sectionCreature.h
InputPath=.\sectionCreature.h

"moc_sectionCreature.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionCreature.h -o moc_sectionCreature.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sectionDecoration.h

!IF  "$(CFG)" == "ThemeEditor - Win32 Release"

# Begin Custom Build - moc sectionDecoration.h
InputPath=.\sectionDecoration.h

"moc_sectionDecoration.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionDecoration.h -o moc_sectionDecoration.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ThemeEditor - Win32 Debug"

# Begin Custom Build - moc sectionDecoration.h
InputPath=.\sectionDecoration.h

"moc_sectionDecoration.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionDecoration.h -o moc_sectionDecoration.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sectionGeneral.h
# End Source File
# Begin Source File

SOURCE=.\sectionGround.h
# End Source File
# Begin Source File

SOURCE=.\sectionLord.h

!IF  "$(CFG)" == "ThemeEditor - Win32 Release"

# Begin Custom Build - moc sectionLord.h
InputPath=.\sectionLord.h

"moc_sectionLord.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionLord.h -o moc_sectionLord.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ThemeEditor - Win32 Debug"

# Begin Custom Build - moc sectionLord.h
InputPath=.\sectionLord.h

"moc_sectionLord.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionLord.h -o moc_sectionLord.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sectionMachine.h
# End Source File
# Begin Source File

SOURCE=.\sectionRessource.h
# End Source File
# Begin Source File

SOURCE=.\sectionSelector.h

!IF  "$(CFG)" == "ThemeEditor - Win32 Release"

# Begin Custom Build - moc sectionSelector.h
InputPath=.\sectionSelector.h

"moc_sectionSelector.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionSelector.h -o moc_sectionSelector.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ThemeEditor - Win32 Debug"

# Begin Custom Build - moc sectionSelector.h
InputPath=.\sectionSelector.h

"moc_sectionSelector.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionSelector.h -o moc_sectionSelector.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sectionTeam.h

!IF  "$(CFG)" == "ThemeEditor - Win32 Release"

# Begin Custom Build - moc sectionTeam.h
InputPath=.\sectionTeam.h

"moc_sectionTeam.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionTeam.h -o moc_sectionTeam.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ThemeEditor - Win32 Debug"

# Begin Custom Build - moc sectionTeam.h
InputPath=.\sectionTeam.h

"moc_sectionTeam.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionTeam.h -o moc_sectionTeam.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sectionWidget.h

!IF  "$(CFG)" == "ThemeEditor - Win32 Release"

# Begin Custom Build - moc sectionWidget.h
InputPath=.\sectionWidget.h

"moc_sectionWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionWidget.h -o moc_sectionWidget.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ThemeEditor - Win32 Debug"

# Begin Custom Build - moc sectionWidget.h
InputPath=.\sectionWidget.h

"moc_sectionWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc sectionWidget.h -o moc_sectionWidget.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\themeInterface.h

!IF  "$(CFG)" == "ThemeEditor - Win32 Release"

# Begin Custom Build - moc themeInterface.h
InputPath=.\themeInterface.h

"moc_themeInterface.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc themeInterface.h -o moc_themeInterface.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ThemeEditor - Win32 Debug"

# Begin Custom Build - moc themeInterface.h
InputPath=.\themeInterface.h

"moc_themeInterface.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc themeInterface.h -o moc_themeInterface.cpp

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\moc_sectionArtefcat.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_sectionCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_sectionDecoration.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_sectionLord.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_sectionSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_sectionTeam.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_sectionWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_themeInterface.cpp
# End Source File
# End Group
# End Target
# End Project
