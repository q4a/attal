# Microsoft Developer Studio Project File - Name="ScenarioEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ScenarioEditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ScenarioEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ScenarioEditor.mak" CFG="ScenarioEditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ScenarioEditor - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ScenarioEditor - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ScenarioEditor - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib ..\common.lib ..\libattal.lib ..\libserver.lib qt-mt311.lib /nologo /subsystem:console /incremental:yes /machine:I386 /out:"../ScenarioEditor.exe"

!ELSEIF  "$(CFG)" == "ScenarioEditor - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib imm32.lib ..\common_d.lib ..\libattal_d.lib ..\libserver_d.lib qt232.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\scenarioEditor_d.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ScenarioEditor - Win32 Release"
# Name "ScenarioEditor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\editorHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\graphicalGameData.cpp
# End Source File
# Begin Source File

SOURCE=.\itemSelection.cpp
# End Source File
# Begin Source File

SOURCE=.\itemSelector.cpp
# End Source File
# Begin Source File

SOURCE=.\propertiesItem.cpp
# End Source File
# Begin Source File

SOURCE=.\propertiesLord.cpp
# End Source File
# Begin Source File

SOURCE=.\scenarioEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\scenarioInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\scenarioInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\screen.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\editorHandler.h
# End Source File
# Begin Source File

SOURCE=.\graphicalGameData.h
# End Source File
# Begin Source File

SOURCE=.\itemSelection.h

!IF  "$(CFG)" == "ScenarioEditor - Win32 Release"

# Begin Custom Build - moc itemSelection.h
InputPath=.\itemSelection.h

"moc_itemSelection.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc itemSelection.h -o moc_itemSelection.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ScenarioEditor - Win32 Debug"

# Begin Custom Build - moc itemSelection.h
InputPath=.\itemSelection.h

"moc_itemSelection.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc itemSelection.h -o moc_itemSelection.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\itemSelector.h
# End Source File
# Begin Source File

SOURCE=.\propertiesItem.h

!IF  "$(CFG)" == "ScenarioEditor - Win32 Release"

# Begin Custom Build - moc propertiesItem.h
InputPath=.\propertiesItem.h

"moc_propertiesItem.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc propertiesItem.h -o moc_propertiesItem.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ScenarioEditor - Win32 Debug"

# Begin Custom Build - moc propertiesItem.h
InputPath=.\propertiesItem.h

"moc_propertiesItem.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc propertiesItem.h -o moc_propertiesItem.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\propertiesLord.h

!IF  "$(CFG)" == "ScenarioEditor - Win32 Release"

# Begin Custom Build - moc propertiesLord.h
InputPath=.\propertiesLord.h

"moc_propertiesLord.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc propertiesLord.h -o moc_propertiesLord.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ScenarioEditor - Win32 Debug"

# Begin Custom Build - moc propertiesLord.h
InputPath=.\propertiesLord.h

"moc_propertiesLord.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc propertiesLord.h -o moc_propertiesLord.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\scenarioInfo.h

!IF  "$(CFG)" == "ScenarioEditor - Win32 Release"

# Begin Custom Build - moc scenarioInfo.h
InputPath=.\scenarioInfo.h

"moc_scenarioInfo.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc scenarioInfo.h -o moc_scenarioInfo.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ScenarioEditor - Win32 Debug"

# Begin Custom Build - moc scenarioInfo.h
InputPath=.\scenarioInfo.h

"moc_scenarioInfo.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc scenarioInfo.h -o moc_scenarioInfo.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\scenarioInterface.h

!IF  "$(CFG)" == "ScenarioEditor - Win32 Release"

# Begin Custom Build - moc scenarioInterface.h
InputPath=.\scenarioInterface.h

"moc_scenarioInterface.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc scenarioInterface.h -o moc_scenarioInterface.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ScenarioEditor - Win32 Debug"

# Begin Custom Build - moc scenarioInterface.h
InputPath=.\scenarioInterface.h

"moc_scenarioInterface.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc scenarioInterface.h -o moc_scenarioInterface.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\screen.h

!IF  "$(CFG)" == "ScenarioEditor - Win32 Release"

# Begin Custom Build - moc screen.h
InputPath=.\screen.h

"moc_screen.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc screen.h -o moc_screen.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "ScenarioEditor - Win32 Debug"

# Begin Custom Build - moc screen.h
InputPath=.\screen.h

"moc_screen.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc screen.h -o moc_screen.cpp

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

SOURCE=.\moc_itemSelection.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_propertiesItem.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_propertiesLord.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_scenarioInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_scenarioInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_screen.cpp
# End Source File
# End Group
# End Target
# End Project
