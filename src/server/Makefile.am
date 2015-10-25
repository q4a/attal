# include files
INCLUDES= $(all_includes)
attal_appsdir = $(kde_appsdir)/Games/Attal

# compilation
CXXFLAGS = -pipe -Wall -g # -O2

# linkage
LDFLAGS = #-s

noinst_LIBRARIES = libattal_server.a

libattal_server_a_SOURCES =	\
	attalServer.cpp		\
	fightAnalyst.cpp	\
	fightEngine.cpp		\
	gameData.cpp		\
	parser.cpp		\
	questManager.cpp

libattal_server_a_METASOURCES = USE_AUTOMOC

bin_PROGRAMS = attal-server duel-server

#attal_server
attal_server_METASOURCES = USE_AUTOMOC
attal_server_LDFLAGS = $(all_libraries)
attal_server_SOURCES = 		\
	serverInterface.cpp	\
	engine.cpp		\
	main.cpp
attal_server_LDADD = ../server/libattal_server.a ../common/libattal.a -lqt -lXext -lX11 $(LIBSOCKET)

#duel_server
duel_server_METASOURCES = USE_AUTOMOC
duel_server_LDFLAGS = $(all_libraries)
duel_server_SOURCES =		\
	interfaceDuel.cpp	\
	duel.cpp
duel_server_LDADD = $(attal_server_LDADD) 

#(un)install
install-data-local:
	$(mkinstalldirs) $(attal_appsdir)/
	$(INSTALL_DATA) attal-server.kdelnk $(attal_appsdir)/attal-server.kdelnk
	$(mkinstalldirs) $(kde_icondir)/
	$(INSTALL_DATA) attal-server.xpm $(kde_icondir)/attal-server.xpm
	$(mkinstalldirs) $(kde_minidir)/
	$(INSTALL_DATA) mini-attal-server.xpm $(kde_minidir)/mini-attal-server.xpm

uninstall-local:
	-rm -f $(attal_appsdir)/attal-server.kdelnk
	-rm -f $(kde_icondir)/attal-server.xpm
	-rm -f $(kde_minidir)/mini-attal-server.xpm


# garbage
CLEANFILES = $(libattal_server_METASOURCES) $(attal_server_METASOURCES) $(duel_server_METASOURCES) attal-*.log

#EXTRA_DIST = main.cpp boserver.cpp boserver.h connect.h connect.cpp serverUnit.cpp serverUnit.h serverMap.cpp player.cpp serverCell.h serverCell.cpp game.h game.cpp

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -x ../../../kdelibs/po/kde.pot $(boserver_SOURCES) && mv messages.po ../po/attal-server.pot






