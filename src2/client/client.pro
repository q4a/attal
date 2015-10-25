TEMPLATE = app
include( ../config.pro )

INCLUDEPATH += ..

LIBS += -L..

unix:LIBS += -lAttalClient
unix:LIBS += -lAttalFight
unix:LIBS += -lAttalAi
unix:LIBS += -lAttalServer
unix:LIBS += -lAttalCommon

win32:LIBS += -lAttalClient10
win32:LIBS += -lAttalFight9
win32:LIBS += -lAttalAi3
win32:LIBS += -lAttalServer10
win32:LIBS += -lAttalCommon10

contains(DEFINES, WITH_SOUND ) {
	win32:INCLUDEPATH += "$$quote($${SDL_DIR})\include"

	contains( CONFIG, staticlib ) {
		win32:LIBS += -L"$$quote($${SDL_DIR})\lib"
		
		LIBS += -lSDL
		LIBS += -lSDL_mixer
	}
}

SOURCES += clientWidgets.cpp
SOURCES += clientInterface.cpp
SOURCES += main.cpp

HEADERS += clientWidgets.h
HEADERS += clientInterface.h

TRANSLATIONS += ../i18n/de/attal_client_de.ts
TRANSLATIONS += ../i18n/fr/attal_client_fr.ts
TRANSLATIONS += ../i18n/ru/attal_client_ru.ts
TRANSLATIONS += ../i18n/it/attal_client_it.ts

DESTDIR = ..

OBJECTS_DIR=./obj
MOC_DIR=./moc

TARGET = attal-client

unix:!macx {
	QMAKE_LFLAGS += -Wl,-rpath,.
}

unix {
	target.path = $${ATT_PREFIX}/bin/
	INSTALLS += target
}

QT += xml network

win32 {
	contains( CONFIG, debug ) {
		CONFIG += console
	}
}

