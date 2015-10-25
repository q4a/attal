TEMPLATE = lib

include( ../config.pro )
INCLUDEPATH += ..
LIBS += -L..
unix:LIBS += -lAttalCommon
win32:LIBS += -lAttalCommon10

VERSION = 10.0.0

SOURCES += attalServer.cpp
SOURCES += playingTroops.cpp
SOURCES += fightEngine.cpp
SOURCES += loadGame.cpp
SOURCES += engine.cpp
SOURCES += questionManager.cpp
SOURCES += tavernManager.cpp
SOURCES += scenarioDescription.cpp
SOURCES += fightAnalyst.cpp

HEADERS += attalServer.h
HEADERS += playingTroops.h
HEADERS += fightEngine.h
HEADERS += loadGame.h
HEADERS += engine.h
HEADERS += questionManager.h
HEADERS += tavernManager.h
HEADERS += scenarioDescription.h
HEADERS += fightAnalyst.h

TRANSLATIONS += ../i18n/de/attal_libserver_de.ts
TRANSLATIONS += ../i18n/fr/attal_libserver_fr.ts
TRANSLATIONS += ../i18n/ru/attal_libserver_ru.ts
TRANSLATIONS += ../i18n/it/attal_libserver_it.ts

DESTDIR = ..

OBJECTS_DIR=./obj
MOC_DIR=./moc

TARGET = AttalServer

unix {
	target.path = $${ATT_LIB_PREFIX}
	INSTALLS += target
}

#The following line was inserted by qt3to4
QT +=  xml network 

