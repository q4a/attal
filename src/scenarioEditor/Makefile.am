# include files
#INCLUDES= -I../visual $(all_includes)
INCLUDES= -I.. $(all_includes)
attal_appsdir = $(kde_appsdir)/Games/Attal

# compilation
CXXFLAGS = -pipe -Wall -g # -O2
#	MY_HOST :=  $(shell hostname | tr '[a-z]' '[A-Z]')_$(shell domainname | tr '[a-z]' '[A-Z]')
#	CXXFLAGS += -D$(MY_HOST)
LDFLAGS =  #-s 


bin_PROGRAMS = scenario-editor
#SUBDIRS = docs 


#scenario_editor
scenario_editor_LDFLAGS = $(all_libraries) 
scenario_editor_SOURCES = 	\
	itemSelector.cpp	\
	itemSelection.cpp	\
	scenarioEditor.cpp	\
	editorHandler.cpp	\
	graphicalGameData.cpp	\
	propertiesItem.cpp	\
	propertiesLord.cpp	\
	scenarioInterface.cpp	\
	scenarioInfo.cpp	\
	screen.cpp		
scenario_editor_METASOURCES = USE_AUTOMOC

scenario_editor_LDADD   =  ../client/libattal_client.a ../server/libattal_server.a ../common/libattal.a -lqt -lXext -lX11 #$(LIBSOCKET)

#(un)install stuff
install-data-local:
	$(mkinstalldirs) $(attal_appsdir)/
	$(INSTALL_DATA) attal-editor.kdelnk $(attal_appsdir)/attal-editor.kdelnk
	$(mkinstalldirs) $(kde_icondir)/
	$(INSTALL_DATA) attal-editor.xpm $(kde_icondir)/attal-editor.xpm
	$(mkinstalldirs) $(kde_minidir)/
	$(INSTALL_DATA) mini-attal-editor.xpm $(kde_minidir)/mini-attal-editor.xpm

uninstall-local:
	-rm -f $(attal_appsdir)/attal-editor.kdelnk
	-rm -f $(kde_icondir)/attal-editor.xpm
	-rm -f $(kde_minidir)/mini-attal-editor.xpm


#garbage
CLEANFILES = $(scenario_editor_METASOURCES) attal-*.log

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -x ../../../kdelibs/po/kde.pot $(attal_editor_SOURCES) && mv messages.po ../po/boeditor.pot


