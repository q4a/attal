TEMPLATE = lib
include( ../config.pro )

INCLUDEPATH += ..

LIBS += -L..

unix:LIBS += -lAttalCommon
unix:LIBS += -lAttalClient
win32:LIBS += -lAttalCommon10
win32:LIBS += -lAttalClient10

contains( DEFINES, WITH_SOUND ) {
	win32:INCLUDEPATH += "$$quote($${SDL_DIR})\include"
}


VERSION = 9.0.0

SOURCES += casualtiesReport.cpp
SOURCES += fight.cpp
SOURCES += fightCell.cpp
SOURCES += fightControl.cpp
SOURCES += fightMap.cpp
SOURCES += fightResult.cpp
SOURCES += fightMapView.cpp
SOURCES += fightSettingsDialog.cpp
SOURCES += fightUnit.cpp
SOURCES += graphicalFightCell.cpp
SOURCES += graphicalFightMap.cpp
SOURCES += lordReport.cpp

HEADERS += casualtiesReport.h
HEADERS += fight.h
HEADERS += fightCell.h
HEADERS += fightControl.h
HEADERS += fightMap.h
HEADERS += fightResult.h
HEADERS += fightMapView.h
HEADERS += fightSettingsDialog.h
HEADERS += fightUnit.h
HEADERS += graphicalFightCell.h
HEADERS += graphicalFightMap.h
HEADERS += lordReport.h

TRANSLATIONS += ../i18n/de/attal_libfight_de.ts
TRANSLATIONS += ../i18n/fr/attal_libfight_fr.ts
TRANSLATIONS += ../i18n/ru/attal_libfight_ru.ts
TRANSLATIONS += ../i18n/it/attal_libfight_it.ts

DESTDIR = ..
OBJECTS_DIR=./obj
MOC_DIR=./moc
TARGET = AttalFight

unix {
	target.path = $${ATT_LIB_PREFIX}
	INSTALLS += target
}

#The following line was inserted by qt3to4
QT +=  xml network 
