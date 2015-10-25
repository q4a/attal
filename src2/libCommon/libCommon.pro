
TEMPLATE = lib

include( ../config.pro )

INCLUDEPATH += ..

VERSION = 10.0.0

SOURCES += action.cpp
SOURCES += artefact.cpp
SOURCES += artefactManager.cpp
SOURCES += attalCommon.cpp
SOURCES += attalSettings.cpp
SOURCES += attalSocket.cpp
SOURCES += attalSocketData.cpp
SOURCES += calendar.cpp
SOURCES += campaign.cpp
SOURCES += categoryManager.cpp
SOURCES += condition.cpp
SOURCES += creature.cpp
SOURCES += dataTheme.cpp
SOURCES += displayHelp.cpp
SOURCES += fightResultStatus.cpp
SOURCES += gameData.cpp
SOURCES += generalOptions.cpp
SOURCES += genericBuilding.cpp
SOURCES += genericBase.cpp
SOURCES += genericBonus.cpp
SOURCES += genericCell.cpp
SOURCES += genericChest.cpp
SOURCES += genericDecoration.cpp
SOURCES += genericEvent.cpp
SOURCES += genericFightCell.cpp
SOURCES += genericFightMap.cpp
SOURCES += genericInsideBuilding.cpp
SOURCES += genericLord.cpp
SOURCES += genericMapCreature.cpp
SOURCES += genericMapDisposition.cpp
SOURCES += genericPlayer.cpp
SOURCES += genericRessources.cpp
SOURCES += genericMap.cpp
SOURCES += genericTeam.cpp
SOURCES += groupName.cpp
SOURCES += log.cpp
SOURCES += lordArtefactsConfiguration.cpp
SOURCES += lordCategoryModel.cpp
SOURCES += lordExperience.cpp
SOURCES += parser.cpp
SOURCES += pathFinder.cpp
SOURCES += priceMarket.cpp
SOURCES += quest.cpp
SOURCES += questManager.cpp
SOURCES += skill.cpp
SOURCES += specialty.cpp
SOURCES += technic.cpp
SOURCES += unit.cpp
SOURCES += warMachine.cpp

HEADERS += ../conf.h
HEADERS += action.h
HEADERS += artefact.h
HEADERS += artefactManager.h
HEADERS += attalCommon.h
HEADERS += attalSettings.h
HEADERS += attalSocket.h
HEADERS += attalSocketData.h
HEADERS += attalTemplates.h
HEADERS += calendar.h
HEADERS += campaign.h
HEADERS += categoryManager.h
HEADERS += condition.h
HEADERS += creature.h
HEADERS += dataTheme.h
HEADERS += displayHelp.h
HEADERS += fightResultStatus.h
HEADERS += gameData.h
HEADERS += generalOptions.h
HEADERS += genericBuilding.h
HEADERS += genericBase.h
HEADERS += genericBonus.h
HEADERS += genericCell.h
HEADERS += genericChest.h
HEADERS += genericDecoration.h
HEADERS += genericEvent.h
HEADERS += genericFightCell.h
HEADERS += genericFightMap.h
HEADERS += genericInsideBuilding.h
HEADERS += genericLord.h
HEADERS += genericMap.h
HEADERS += genericMapCreature.h
HEADERS += genericMapDisposition.h
HEADERS += genericPlayer.h
HEADERS += genericRessources.h
HEADERS += genericTeam.h
HEADERS += groupName.h
HEADERS += log.h
HEADERS += lordArtefactsConfiguration.h
HEADERS += lordCategoryModel.h
HEADERS += lordExperience.h
HEADERS += parser.h
HEADERS += pathFinder.h
HEADERS += priceMarket.h
HEADERS += quest.h
HEADERS += questManager.h
HEADERS += skill.h
HEADERS += specialty.h
HEADERS += technic.h
HEADERS += unit.h
HEADERS += warMachine.h

TRANSLATIONS += ../i18n/de/attal_libcommon_de.ts
TRANSLATIONS += ../i18n/fr/attal_libcommon_fr.ts
TRANSLATIONS += ../i18n/ru/attal_libcommon_ru.ts
TRANSLATIONS += ../i18n/it/attal_libcommon_it.ts

DESTDIR = ..

OBJECTS_DIR=./obj
MOC_DIR=./moc

TARGET = AttalCommon

unix {
	target.path = $${ATT_LIB_PREFIX}
	INSTALLS += target
}

#The following line was inserted by qt3to4
QT +=  xml network
