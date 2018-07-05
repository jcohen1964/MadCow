# Microsoft Developer Studio Project File - Name="MadCow" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MadCow - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MadCow.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MadCow.mak" CFG="MadCow - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MadCow - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MadCow - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MadCow - Win32 Release"

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
# ADD CPP /nologo /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "D:\Archive\Vault\Code\C++\Projects\MadCow" /d "NDEBUG" /d "CLASS_LIBRARY D:\Archive\Vault\Code\C++\Projects\MadCow\Classes"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "MadCow - Win32 Debug"

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
# ADD CPP /nologo /w /W0 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /x /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "MadCow - Win32 Release"
# Name "MadCow - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\globals.cpp
# End Source File
# Begin Source File

SOURCE=.\madcow.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\classes.h
# End Source File
# Begin Source File

SOURCE=.\globals.h
# End Source File
# Begin Source File

SOURCE=.\utilities.h
# End Source File
# End Group
# Begin Group "Classes"

# PROP Default_Filter ""
# Begin Group "Animals"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\animals.h
# End Source File
# Begin Source File

SOURCE=.\bovine.cpp
# End Source File
# Begin Source File

SOURCE=.\bovine.h
# End Source File
# Begin Source File

SOURCE=.\bovineGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\bovineGroup.h
# End Source File
# Begin Source File

SOURCE=.\bovineHerd.cpp
# End Source File
# Begin Source File

SOURCE=.\bovineHerd.h
# End Source File
# Begin Source File

SOURCE=.\sickBovine.cpp
# End Source File
# Begin Source File

SOURCE=.\sickBovine.h
# End Source File
# End Group
# Begin Group "Random"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mersenne.cpp
# End Source File
# Begin Source File

SOURCE=.\random.cpp
# End Source File
# Begin Source File

SOURCE=.\random.h
# End Source File
# Begin Source File

SOURCE=.\randomc.h
# End Source File
# Begin Source File

SOURCE=.\stoc1.cpp
# End Source File
# Begin Source File

SOURCE=.\stocc.h
# End Source File
# Begin Source File

SOURCE=.\userintf.cpp
# End Source File
# End Group
# Begin Group "Functor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\functDistrDisc.h
# End Source File
# Begin Source File

SOURCE=.\functDistrGaus.h
# End Source File
# Begin Source File

SOURCE=.\functDistrUnif.h
# End Source File
# Begin Source File

SOURCE=.\functInterLine.h
# End Source File
# Begin Source File

SOURCE=.\functInterStep.h
# End Source File
# Begin Source File

SOURCE=.\functTable.cpp
# End Source File
# Begin Source File

SOURCE=.\functTable.h
# End Source File
# End Group
# Begin Group "Iterator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bovineHerdIterator.cpp
# End Source File
# Begin Source File

SOURCE=.\bovineHerdIterator.h
# End Source File
# End Group
# Begin Group "Process"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AMInspector.cpp
# End Source File
# Begin Source File

SOURCE=.\AMInspector.h
# End Source File
# Begin Source File

SOURCE=.\feeder.cpp
# End Source File
# Begin Source File

SOURCE=.\feeder.h
# End Source File
# Begin Source File

SOURCE=.\feedProducer.cpp
# End Source File
# Begin Source File

SOURCE=.\feedProducer.h
# End Source File
# Begin Source File

SOURCE=.\feedTransporter.cpp
# End Source File
# Begin Source File

SOURCE=.\feedTransporter.h
# End Source File
# Begin Source File

SOURCE=.\foodInspector.cpp
# End Source File
# Begin Source File

SOURCE=.\foodInspector.h
# End Source File
# Begin Source File

SOURCE=.\materialize.cpp
# End Source File
# Begin Source File

SOURCE=.\materialize.h
# End Source File
# Begin Source File

SOURCE=.\MBMTransporter.cpp
# End Source File
# Begin Source File

SOURCE=.\MBMTransporter.h
# End Source File
# Begin Source File

SOURCE=.\PMInspector.cpp
# End Source File
# Begin Source File

SOURCE=.\PMInspector.h
# End Source File
# Begin Source File

SOURCE=.\renderer.cpp
# End Source File
# Begin Source File

SOURCE=.\renderer.h
# End Source File
# Begin Source File

SOURCE=.\splitter.cpp
# End Source File
# Begin Source File

SOURCE=.\splitter.h
# End Source File
# Begin Source File

SOURCE=.\SRMInspector.cpp
# End Source File
# Begin Source File

SOURCE=.\SRMInspector.h
# End Source File
# Begin Source File

SOURCE=.\stunner.cpp
# End Source File
# Begin Source File

SOURCE=.\stunner.h
# End Source File
# End Group
# Begin Group "Visitor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\birthVisitor.cpp
# End Source File
# Begin Source File

SOURCE=.\birthVisitor.h
# End Source File
# Begin Source File

SOURCE=.\bloodInfector.cpp
# End Source File
# Begin Source File

SOURCE=.\bloodInfector.h
# End Source File
# Begin Source File

SOURCE=.\deathVisitor.cpp
# End Source File
# Begin Source File

SOURCE=.\deathVisitor.h
# End Source File
# Begin Source File

SOURCE=.\genesisVisitor.cpp
# End Source File
# Begin Source File

SOURCE=.\genesisVisitor.h
# End Source File
# Begin Source File

SOURCE=.\proteinInfector.cpp
# End Source File
# Begin Source File

SOURCE=.\proteinInfector.h
# End Source File
# Begin Source File

SOURCE=.\randomInfector.cpp
# End Source File
# Begin Source File

SOURCE=.\randomInfector.h
# End Source File
# Begin Source File

SOURCE=.\spontInfector.cpp
# End Source File
# Begin Source File

SOURCE=.\spontInfector.h
# End Source File
# Begin Source File

SOURCE=.\totalClinicalVisitor.cpp
# End Source File
# Begin Source File

SOURCE=.\totalClinicalVisitor.h
# End Source File
# Begin Source File

SOURCE=.\totalInfectedVisitor.cpp
# End Source File
# Begin Source File

SOURCE=.\totalInfectedVisitor.h
# End Source File
# Begin Source File

SOURCE=.\totalNumVisitor.cpp
# End Source File
# Begin Source File

SOURCE=.\totalNumVisitor.h
# End Source File
# Begin Source File

SOURCE=.\visitor.h
# End Source File
# End Group
# Begin Group "Material"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\emboli.cpp
# End Source File
# Begin Source File

SOURCE=.\emboli.h
# End Source File
# Begin Source File

SOURCE=.\feed.cpp
# End Source File
# Begin Source File

SOURCE=.\feed.h
# End Source File
# Begin Source File

SOURCE=.\feedGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\feedGroup.h
# End Source File
# Begin Source File

SOURCE=.\meal.cpp
# End Source File
# Begin Source File

SOURCE=.\meal.h
# End Source File
# Begin Source File

SOURCE=.\mealGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\mealGroup.h
# End Source File
# Begin Source File

SOURCE=.\organ.cpp
# End Source File
# Begin Source File

SOURCE=.\organ.h
# End Source File
# End Group
# Begin Group "Reporter"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\reporter.cpp
# End Source File
# Begin Source File

SOURCE=.\reporter.h
# End Source File
# End Group
# Begin Group "Process_Deads"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MBMTransporter_Dead.cpp
# End Source File
# Begin Source File

SOURCE=.\MBMTransporter_Dead.h
# End Source File
# Begin Source File

SOURCE=.\Renderer_Dead.cpp
# End Source File
# Begin Source File

SOURCE=.\RENDERER_Dead.H
# End Source File
# Begin Source File

SOURCE=.\SRMInspector_Dead.cpp
# End Source File
# Begin Source File

SOURCE=.\SRMInspector_Dead.h
# End Source File
# End Group
# End Group
# End Target
# End Project
