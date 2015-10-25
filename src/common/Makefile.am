# include files
INCLUDES= $(all_includes)

# compilation
CXXFLAGS = -pipe -Wall -g # -O2

LDFLAGS =  #-s 

noinst_LIBRARIES = libattal.a

noinst_HEADERS = 		\
	action.h		\
	attalSocket.h		\
	artefact.h		\
	calendar.h		\
	categoryManager.h	\
	condition.h		\
	creature.h		\
	define.h		\
	genericBuilding.h	\
	genericBase.h		\
	genericCell.h		\
	genericDecoration.h	\
	genericEvent.h		\
	genericFightCell.h	\
	genericFightMap.h	\
	genericInsideBuilding.h	\
	genericLord.h		\
	priceMarket.h		\
	genericRessources.h	\
	genericTeam.h		\
	genericMap.h		\
	groupName.h		\
	log.h			\
	pathFinder.h		\
	quest.h			\
	genericPlayer.h		\
	skill.h			\
	specialty.h		\
	specifications.h	\
	technic.h		\
	unit.h			\
	warMachine.h


libattal_a_SOURCES = 		\
	action.cpp		\
	attalSocket.cpp		\
	artefact.cpp 		\
	calendar.cpp		\
	categoryManager.cpp	\
	condition.cpp		\
	creature.cpp		\
	dataTheme.cpp		\
	genericBuilding.cpp 	\
	genericBase.cpp		\
	genericCell.cpp 	\
	genericDecoration.cpp	\
	genericEvent.cpp	\
	genericFightCell.cpp 	\
	genericFightMap.cpp 	\
	genericInsideBuilding.cpp	\
	genericLord.cpp		\
	genericMapCreature.cpp	\
	groupName.cpp		\
	priceMarket.cpp		\
	genericRessources.cpp	\
	genericMap.cpp		\
	genericTeam.cpp		\
	log.cpp			\
	pathFinder.cpp		\
	genericPlayer.cpp	\
	quest.cpp		\
	skill.cpp		\
	specialty.cpp		\
	specifications.cpp	\
	technic.cpp		\
	unit.cpp		\
	warMachine.cpp

libattal_a_METASOURCES = USE_AUTOMOC

CLEANFILES = $(libattal_a_METASOURCES)

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -x ../../../kdelibs/po/kde.pot $(libattal_a_SOURCES) && mv messages.po ../po/attalcommon.pot


