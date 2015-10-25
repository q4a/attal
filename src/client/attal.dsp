# Microsoft Developer Studio Project File - Name="attal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=attal - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "attal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "attal.mak" CFG="attal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "attal - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "attal - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "attal - Win32 Release"

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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib imm32.lib ..\common.lib ..\libattal.lib ..\libfight.lib qt-mt311.lib /nologo /subsystem:console /incremental:yes /machine:I386 /out:"../attal-client.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "attal - Win32 Debug"

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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib imm32.lib ..\libattal_d.lib ..\libfight_d.lib ..\common_d.lib qt232.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\attal-client_d.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "attal - Win32 Release"
# Name "attal - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\clientInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\displayBase.cpp
# End Source File
# Begin Source File

SOURCE=.\displayCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\displayLord.cpp
# End Source File
# Begin Source File

SOURCE=.\gainLevel.cpp
# End Source File
# Begin Source File

SOURCE=.\insideBase.cpp
# End Source File
# Begin Source File

SOURCE=.\insideBuilding.cpp
# End Source File
# Begin Source File

SOURCE=.\lordExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\market.cpp
# End Source File
# Begin Source File

SOURCE=.\unitExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\widget.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\clientInterface.h

!IF  "$(CFG)" == "attal - Win32 Release"

# Begin Custom Build - moc clientInterface.h
InputPath=.\clientInterface.h

"moc_clientInterface.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc clientInterface.h -o moc_clientInterface.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "attal - Win32 Debug"

# Begin Custom Build - moc clientInterface.h
InputPath=.\clientInterface.h

"moc_clientInterface.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc clientInterface.h -o moc_clientInterface.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\displayBase.h

!IF  "$(CFG)" == "attal - Win32 Release"

# Begin Custom Build - moc displayBase.h
InputPath=.\displayBase.h

"moc_displayBase.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc displayBase.h -o moc_displayBase.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "attal - Win32 Debug"

# Begin Custom Build - moc displayBase.h
InputPath=.\displayBase.h

"moc_displayBase.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc displayBase.h -o moc_displayBase.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\displayCreature.h

!IF  "$(CFG)" == "attal - Win32 Release"

# Begin Custom Build - moc displayCreature.h
InputPath=.\displayCreature.h

"moc_displayCreature.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc displayCreature.h -o moc_displayCreature.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "attal - Win32 Debug"

# Begin Custom Build - moc displayCreature.h
InputPath=.\displayCreature.h

"moc_displayCreature.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc displayCreature.h -o moc_displayCreature.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\displayLord.h

!IF  "$(CFG)" == "attal - Win32 Release"

# Begin Custom Build - moc displayLord.h
InputPath=.\displayLord.h

"moc_displayLord.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc displayLord.h -o moc_displayLord.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "attal - Win32 Debug"

# Begin Custom Build - moc displayLord.h
InputPath=.\displayLord.h

"moc_displayLord.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc displayLord.h -o moc_displayLord.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\gainLevel.h

!IF  "$(CFG)" == "attal - Win32 Release"

# Begin Custom Build - moc gainLevel.h
InputPath=.\gainLevel.h

"moc_gainLevel.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc gainLevel.h -o moc_gainLevel.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "attal - Win32 Debug"

# Begin Custom Build - moc gainLevel.h
InputPath=.\gainLevel.h

"moc_gainLevel.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc gainLevel.h -o moc_gainLevel.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\insideBase.h

!IF  "$(CFG)" == "attal - Win32 Release"

# Begin Custom Build - moc insideBase.h
InputPath=.\insideBase.h

"moc_insideBase.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc insideBase.h -o moc_insideBase.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "attal - Win32 Debug"

# Begin Custom Build - moc insideBase.h
InputPath=.\insideBase.h

"moc_insideBase.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc insideBase.h -o moc_insideBase.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\insideBuilding.h
# End Source File
# Begin Source File

SOURCE=.\lordExchange.h

!IF  "$(CFG)" == "attal - Win32 Release"

# Begin Custom Build - moc lordExchange.h
InputPath=.\lordExchange.h

"moc_lordExchange.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc lordExchange.h -o moc_lordExchange.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "attal - Win32 Debug"

# Begin Custom Build - moc lordExchange.h
InputPath=.\lordExchange.h

"moc_lordExchange.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc lordExchange.h -o moc_lordExchange.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\market.h

!IF  "$(CFG)" == "attal - Win32 Release"

# Begin Custom Build - moc market.h
InputPath=.\market.h

"moc_market.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc market.h -o moc_market.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "attal - Win32 Debug"

# Begin Custom Build - moc market.h
InputPath=.\market.h

"moc_market.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc market.h -o moc_market.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\unitExchange.h

!IF  "$(CFG)" == "attal - Win32 Release"

# Begin Custom Build - moc unitExchange.h
InputPath=.\unitExchange.h

"moc_unitExchange.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc unitExchange.h -o moc_unitExchange.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "attal - Win32 Debug"

# Begin Custom Build - moc unitExchange.h
InputPath=.\unitExchange.h

"moc_unitExchange.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc unitExchange.h -o moc_unitExchange.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\widget.h

!IF  "$(CFG)" == "attal - Win32 Release"

# Begin Custom Build - moc widget.h -o moc_widget.cpp
InputPath=.\widget.h

"moc_widget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc widget.h -o moc_widget.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "attal - Win32 Debug"

# Begin Custom Build - moc widget.h -o moc_widget.cpp
InputPath=.\widget.h

"moc_widget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc widget.h -o moc_widget.cpp

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

SOURCE=.\moc_clientInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_displayBase.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_displayCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_displayLord.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_gainLevel.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_insideBase.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_lordExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_market.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_unitExchange.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_widget.cpp
# End Source File
# End Group
# End Target
# End Project
