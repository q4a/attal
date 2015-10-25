# Microsoft Developer Studio Project File - Name="libattal" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libattal - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libattal.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libattal.mak" CFG="libattal - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libattal - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libattal - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libattal - Win32 Release"

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
# ADD LIB32 /nologo /out:"..\libattal.lib"

!ELSEIF  "$(CFG)" == "libattal - Win32 Debug"

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
# ADD LIB32 /nologo /out:"..\libattal_d.lib"

!ENDIF 

# Begin Target

# Name "libattal - Win32 Release"
# Name "libattal - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\askWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\attalStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\building.cpp
# End Source File
# Begin Source File

SOURCE=.\cell.cpp
# End Source File
# Begin Source File

SOURCE=.\chatWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\event.cpp
# End Source File
# Begin Source File

SOURCE=.\game.cpp
# End Source File
# Begin Source File

SOURCE=.\gameControl.cpp
# End Source File
# Begin Source File

SOURCE=.\gameDescription.cpp
# End Source File
# Begin Source File

SOURCE=.\gameInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\graphicalArtefact.cpp
# End Source File
# Begin Source File

SOURCE=.\graphicalBuilding.cpp
# End Source File
# Begin Source File

SOURCE=.\graphicalCell.cpp
# End Source File
# Begin Source File

SOURCE=.\graphicalLord.cpp
# End Source File
# Begin Source File

SOURCE=.\graphicalMap.cpp
# End Source File
# Begin Source File

SOURCE=.\graphicalPath.cpp
# End Source File
# Begin Source File

SOURCE=.\gui.cpp
# End Source File
# Begin Source File

SOURCE=.\imageTheme.cpp
# End Source File
# Begin Source File

SOURCE=.\insideAction.cpp
# End Source File
# Begin Source File

SOURCE=.\insideBase.cpp
# End Source File
# Begin Source File

SOURCE=.\insideBuilding.cpp
# End Source File
# Begin Source File

SOURCE=.\lord.cpp
# End Source File
# Begin Source File

SOURCE=.\map.cpp
# End Source File
# Begin Source File

SOURCE=.\mapCreature.cpp
# End Source File
# Begin Source File

SOURCE=.\mapView.cpp
# End Source File
# Begin Source File

SOURCE=.\miniMap.cpp
# End Source File
# Begin Source File

SOURCE=.\player.cpp
# End Source File
# Begin Source File

SOURCE=.\ressourceBar.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\askWidget.h

!IF  "$(CFG)" == "libattal - Win32 Release"

# Begin Custom Build - moc askWidget.h
InputPath=.\askWidget.h

"moc_askWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc askWidget.h -o moc_askWidget.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "libattal - Win32 Debug"

# Begin Custom Build - moc askWidget.h
InputPath=.\askWidget.h

"moc_askWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc askWidget.h -o moc_askWidget.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\attalStyle.h
# End Source File
# Begin Source File

SOURCE=.\building.h
# End Source File
# Begin Source File

SOURCE=.\cell.h
# End Source File
# Begin Source File

SOURCE=.\chatWidget.h

!IF  "$(CFG)" == "libattal - Win32 Release"

# Begin Custom Build - moc chatWidget.h
InputPath=.\chatWidget.h

"moc_chatWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc chatWidget.h -o moc_chatWidget.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "libattal - Win32 Debug"

# Begin Custom Build - moc chatWidget.h
InputPath=.\chatWidget.h

"moc_chatWidget.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc chatWidget.h -o moc_chatWidget.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\event.h
# End Source File
# Begin Source File

SOURCE=.\game.h

!IF  "$(CFG)" == "libattal - Win32 Release"

# Begin Custom Build - moc game.h
InputPath=.\game.h

"moc_game.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc game.h -o moc_game.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "libattal - Win32 Debug"

# Begin Custom Build - moc game.h
InputPath=.\game.h

"moc_game.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc game.h -o moc_game.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\gameControl.h

!IF  "$(CFG)" == "libattal - Win32 Release"

# Begin Custom Build - moc gameControl.h
InputPath=.\gameControl.h

"moc_gameControl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc gameControl.h -o moc_gameControl.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "libattal - Win32 Debug"

# Begin Custom Build - moc gameControl.h
InputPath=.\gameControl.h

"moc_gameControl.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc gameControl.h -o moc_gameControl.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\gameDescription.h
# End Source File
# Begin Source File

SOURCE=.\gameInfo.h

!IF  "$(CFG)" == "libattal - Win32 Release"

# Begin Custom Build - moc gameInfo.h
InputPath=.\gameInfo.h

"moc_gameInfo.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc gameInfo.h -o moc_gameInfo.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "libattal - Win32 Debug"

# Begin Custom Build - moc gameInfo.h
InputPath=.\gameInfo.h

"moc_gameInfo.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc gameInfo.h -o moc_gameInfo.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\graphicalArtefact.h
# End Source File
# Begin Source File

SOURCE=.\graphicalBuilding.h
# End Source File
# Begin Source File

SOURCE=.\graphicalCell.h
# End Source File
# Begin Source File

SOURCE=.\graphicalLord.h
# End Source File
# Begin Source File

SOURCE=.\graphicalMap.h
# End Source File
# Begin Source File

SOURCE=.\graphicalPath.h
# End Source File
# Begin Source File

SOURCE=.\gui.h
# End Source File
# Begin Source File

SOURCE=.\imageTheme.h
# End Source File
# Begin Source File

SOURCE=.\insideAction.h

!IF  "$(CFG)" == "libattal - Win32 Release"

# Begin Custom Build - moc insideAction.h
InputPath=.\insideAction.h

"moc_insideAction.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc insideAction.h -o moc_insideAction.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "libattal - Win32 Debug"

# Begin Custom Build - moc insideAction.h
InputPath=.\insideAction.h

"moc_insideAction.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc insideAction.h -o moc_insideAction.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\insideBase.h
# End Source File
# Begin Source File

SOURCE=.\insideBuilding.h
# End Source File
# Begin Source File

SOURCE=.\lord.h
# End Source File
# Begin Source File

SOURCE=.\map.h
# End Source File
# Begin Source File

SOURCE=.\mapCreature.h
# End Source File
# Begin Source File

SOURCE=.\mapView.h

!IF  "$(CFG)" == "libattal - Win32 Release"

# Begin Custom Build - moc mapView.h
InputPath=.\mapView.h

"moc_mapView.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc mapView.h -o moc_mapView.cpp

# End Custom Build

!ELSEIF  "$(CFG)" == "libattal - Win32 Debug"

# Begin Custom Build - moc mapView.h
InputPath=.\mapView.h

"moc_mapView.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	moc mapView.h -o moc_mapView.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\miniMap.h
# End Source File
# Begin Source File

SOURCE=.\player.h
# End Source File
# Begin Source File

SOURCE=.\ressourceBar.h
# End Source File
# End Group
# Begin Group "moc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\moc_askWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_chatWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_game.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_gameControl.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_gameInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_insideAction.cpp
# End Source File
# Begin Source File

SOURCE=.\moc_mapView.cpp
# End Source File
# End Group
# End Target
# End Project
