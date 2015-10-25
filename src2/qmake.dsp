# Microsoft Developer Studio Project File - Name="qmake" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=qmake - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "qmake.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "qmake.mak" CFG="qmake - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "qmake - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "qmake - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "qmake - Win32 Release"

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

!ELSEIF  "$(CFG)" == "qmake - Win32 Debug"

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

!ENDIF 

# Begin Target

# Name "qmake - Win32 Release"
# Name "qmake - Win32 Debug"
# Begin Group "libCommon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libCommon\Makefile.pro

!IF  "$(CFG)" == "qmake - Win32 Release"

!ELSEIF  "$(CFG)" == "qmake - Win32 Debug"

# Begin Custom Build - qmake 'libCommon'
InputPath=.\libCommon\Makefile.pro

"Common.dsp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd libCommon 
	qmake -t vclib Makefile.pro 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "libClient"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libClient\Makefile.pro

!IF  "$(CFG)" == "qmake - Win32 Release"

!ELSEIF  "$(CFG)" == "qmake - Win32 Debug"

# Begin Custom Build - qmake 'libClient'
InputPath=.\libClient\Makefile.pro

"Client.dsp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd libClient 
	qmake -t vclib Makefile.pro 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "libServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libServer\Makefile.pro

!IF  "$(CFG)" == "qmake - Win32 Release"

!ELSEIF  "$(CFG)" == "qmake - Win32 Debug"

# Begin Custom Build - qmake 'libServer'
InputPath=.\libServer\Makefile.pro

"Server.dsp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd libServer 
	qmake -t vclib Makefile.pro 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "libFight"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libFight\Makefile.pro

!IF  "$(CFG)" == "qmake - Win32 Release"

!ELSEIF  "$(CFG)" == "qmake - Win32 Debug"

# Begin Custom Build - qmake 'libFight'
InputPath=.\libFight\Makefile.pro

"Fight.dsp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd libFight 
	qmake -t vclib Makefile.pro 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "client"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\client\Makefile.pro

!IF  "$(CFG)" == "qmake - Win32 Release"

!ELSEIF  "$(CFG)" == "qmake - Win32 Debug"

# Begin Custom Build - qmake 'client'
InputPath=.\client\Makefile.pro

"attal_client.dsp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd client 
	qmake -t vcapp Makefile.pro 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "server"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\server\Makefile.pro

!IF  "$(CFG)" == "qmake - Win32 Release"

!ELSEIF  "$(CFG)" == "qmake - Win32 Debug"

# Begin Custom Build - qmake 'server'
InputPath=.\server\Makefile.pro

"attal_server.dsp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd server 
	qmake -t vcapp Makefile.pro 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "ai"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ai\Makefile.pro

!IF  "$(CFG)" == "qmake - Win32 Release"

!ELSEIF  "$(CFG)" == "qmake - Win32 Debug"

# Begin Custom Build - qmake 'ai'
InputPath=.\ai\Makefile.pro

"attal_ai.dsp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd ai 
	qmake -t vcapp Makefile.pro 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "scenarioEditor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\scenarioEditor\Makefile.pro

!IF  "$(CFG)" == "qmake - Win32 Release"

!ELSEIF  "$(CFG)" == "qmake - Win32 Debug"

# Begin Custom Build - qmake scenarioEditor
InputPath=.\scenarioEditor\Makefile.pro

"scenario_editor.dsp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd scenarioEditor 
	qmake -t vcapp Makefile.pro 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "themeEditor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\themeEditor\Makefile.pro

!IF  "$(CFG)" == "qmake - Win32 Release"

!ELSEIF  "$(CFG)" == "qmake - Win32 Debug"

# Begin Custom Build - qmake themeEditor
InputPath=.\themeEditor\Makefile.pro

"theme_editor.dsp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cd themeEditor 
	qmake -t vcapp Makefile.pro 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
