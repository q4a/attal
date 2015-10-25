TEMPLATE = app
include( ../config.pro )

INCLUDEPATH += ..

LIBS += -L..

unix:LIBS += -lAttalClient
unix:LIBS += -lAttalCommon

win32:LIBS += -lAttalClient10
win32:LIBS += -lAttalCommon10

contains( DEFINES, WITH_SOUND ) {
	win32:INCLUDEPATH += "$$quote($${SDL_DIR})\include"
			
	contains( CONFIG, staticlib ) {
		LIBS += -lSDL
		LIBS += -lSDL_mixer
	}
}

SOURCES += displayQuests.cpp
#SOURCES += editorHandler.cpp
SOURCES += itemSelection.cpp
SOURCES += itemSelector.cpp
SOURCES += propertiesBase.cpp
SOURCES += propertiesBuilding.cpp
SOURCES += propertiesCreature.cpp
SOURCES += propertiesEvent.cpp
SOURCES += propertiesLord.cpp
SOURCES += propertiesPlayers.cpp
SOURCES += questConditionEditor.cpp
SOURCES += questConditionProperties.cpp
SOURCES += questEditor.cpp
SOURCES += scenarioEditor.cpp
SOURCES += scenarioInfo.cpp
SOURCES += scenarioInterface.cpp
SOURCES += screen.cpp
SOURCES += tilePresentation.cpp
SOURCES += tileSelection.cpp

HEADERS += displayQuests.h
#HEADERS += editorHandler.h
HEADERS += itemSelection.h
HEADERS += itemSelector.h
HEADERS += propertiesBase.h
HEADERS += propertiesBuilding.h
HEADERS += propertiesCreature.h
HEADERS += propertiesEvent.h
HEADERS += propertiesLord.h
HEADERS += propertiesPlayers.h
HEADERS += questConditionEditor.h
HEADERS += questConditionProperties.h
HEADERS += questEditor.h
HEADERS += scenarioInfo.h
HEADERS += scenarioInterface.h
HEADERS += screen.h
HEADERS += tilePresentation.h
HEADERS += tileSelection.h


DESTDIR = ..

OBJECTS_DIR=./obj
MOC_DIR=./moc

TARGET = attal-scenario-editor

TRANSLATIONS += ../i18n/de/scenario_editor_de.ts
TRANSLATIONS += ../i18n/fr/scenario_editor_fr.ts
TRANSLATIONS += ../i18n/ru/scenario_editor_ru.ts
TRANSLATIONS += ../i18n/it/scenario_editor_it.ts

unix:!macx {
	QMAKE_LFLAGS += -Wl,-rpath,.
}

unix {
	target.path = $${ATT_PREFIX}/bin/
	INSTALLS += target
}

QT +=  xml 
win32 {
	contains( CONFIG, debug ) {
		CONFIG += console
	}
}

