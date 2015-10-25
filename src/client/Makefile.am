# include files
INCLUDES= -I.. $(all_includes)

# compilation
CXXFLAGS = -pipe -Wall -g # -O2

LDFLAGS =  #-s 

noinst_LIBRARIES = libattal_client.a libattal_fight.a

noinst_HEADERS = 		\
	attalStyle.h		\
	building.h		\
	cell.h			\
	event.h			\
	game.h			\
	gameControl.h		\
	gameDescription.h	\
	gameInfo.h		\
	graphicalBuilding.h	\
	graphicalCell.h		\
	graphicalLord.h		\
	graphicalMap.h		\
	graphicalPath.h		\
	gui.h			\
	lord.h			\
	map.h			\
	mapView.h		\
	miniMap.h		\
	pixmapSpinBox.h		\
	player.h		\
	ressourceBar.h


libattal_client_a_SOURCES = 	\
	askWidget.cpp		\
	attalStyle.cpp		\
	building.cpp		\
	cell.cpp		\
	chatWidget.cpp		\
	event.cpp		\
	game.cpp		\
	gameControl.cpp		\
	gameDescription.cpp	\
	gameInfo.cpp		\
	graphicalArtefact.cpp	\
	graphicalBuilding.cpp	\
	graphicalCell.cpp	\
	graphicalLord.cpp	\
	graphicalMap.cpp	\
	graphicalPath.cpp	\
	gui.cpp			\
	imageTheme.cpp		\
	lord.cpp		\
	map.cpp			\
	mapCreature.cpp		\
	mapView.cpp		\
	miniMap.cpp		\
	pixmapSpinBox.cpp	\
	player.cpp		\
	ressourceBar.cpp	\
	askWidget.moc.cpp	\
	chatWidget.moc.cpp	\
	gameControl.moc.cpp 	\
	game.moc.cpp 		\
	gameInfo.moc.cpp	\
	mapView.moc.cpp 	\
	pixmapSpinBox.moc.cpp

libattal_fight_a_SOURCES = 	\
	fight.cpp		\
	fightCell.cpp		\
	fightMap.cpp		\
	fightResult.cpp		\
	fightMapView.cpp	\
	fightUnit.cpp		\
	graphicalFightCell.cpp	\
	graphicalFightMap.cpp	\
	graphicalFightUnit.cpp

libattal_fight_a_METASOURCES = USE_AUTOMOC


bin_PROGRAMS = attal-client duel-client

#attal_client
attal_client_METASOURCES = USE_AUTOMOC
attal_client_LDFLAGS = $(all_libraries)
attal_client_SOURCES = 		\
	displayBase.cpp		\
	displayCreature.cpp	\
	displayLord.cpp		\
	displayTechnics.cpp	\
	gainLevel.cpp		\
	clientInterface.cpp	\
	insideAction.cpp	\
	insideBase.cpp		\
	insideBuilding.cpp	\
	lordExchange.cpp	\
	market.cpp		\
	unitExchange.cpp	\
	widget.cpp		\
	main.cpp		

attal_client_LDADD   =  ../client/libattal_fight.a ../client/libattal_client.a ../common/libattal.a -lqt -lXext -lX11 #$(LIBSOCKET)

duel_client_METASOURCES = USE_AUTOMOC
duel_client_LDFLAGS = $(all_libraries)
duel_client_SOURCES = 		\
	interfaceDuel.cpp	\
	widget.cpp		\
	duel.cpp
duel_client_LDADD    = $(attal_client_LDADD)


CLEANFILES = $(libattal_fight_a_METASOURCES) $(libattal_client_a_METASOURCES) $(duel_client_METASOURCES) $(attal_client_METASOURCES) attal-*.log

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -x ../../../kdelibs/po/kde.pot $(libattal_client_a_SOURCES) $(attal_client_SOURCES) && mv messages.po ../po/attalclient.pot


attal_appsdir = $(kde_appsdir)/Games/Attal

#(un)install stuff
install-data-local:
	$(mkinstalldirs) $(attal_appsdir)/
	$(INSTALL_DATA) attal-client.kdelnk $(attal_appsdir)/attal-client.kdelnk
	$(mkinstalldirs) $(kde_icondir)/
	$(INSTALL_DATA) attal-editor.xpm $(kde_icondir)/attal-editor.xpm
	$(mkinstalldirs) $(kde_minidir)/
	$(INSTALL_DATA) mini-attal-client.xpm $(kde_minidir)/mini-attal-client.xpm

uninstall-local:
	-rm -f $(attal_appsdir)/attal-client.kdelnk
	-rm -f $(kde_icondir)/attal-client.xpm
	-rm -f $(kde_minidir)/mini-attal-client.xpm


