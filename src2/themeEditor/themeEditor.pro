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

SOURCES += artefactAction.cpp
SOURCES += askDiversification.cpp
SOURCES += buildingAction.cpp
SOURCES += creatureAnimation.cpp
SOURCES += decorationEffect.cpp
SOURCES += editWidget.cpp
SOURCES += mapDispositionEditor.cpp
SOURCES += previewBaseDialog.cpp
SOURCES += sectionArtefact.cpp
SOURCES += sectionBase.cpp
SOURCES += sectionBuilding.cpp
SOURCES += sectionCreature.cpp
SOURCES += sectionDecoration.cpp
SOURCES += sectionExperience.cpp
SOURCES += sectionGeneral.cpp
SOURCES += sectionGround.cpp
SOURCES += sectionLord.cpp
SOURCES += sectionLordCategory.cpp
SOURCES += sectionMachine.cpp
SOURCES += sectionRessource.cpp
SOURCES += sectionSelector.cpp
SOURCES += sectionTeam.cpp
SOURCES += sectionWidget.cpp
SOURCES += themeEditor.cpp
SOURCES += themeInterface.cpp

HEADERS += artefactAction.h
HEADERS += askDiversification.h
HEADERS += buildingAction.h
HEADERS += creatureAnimation.h
HEADERS += decorationEffect.h
HEADERS += editWidget.h
HEADERS += mapDispositionEditor.h
HEADERS += previewBaseDialog.h
HEADERS += sectionArtefact.h
HEADERS += sectionBase.h
HEADERS += sectionBuilding.h
HEADERS += sectionCreature.h
HEADERS += sectionDecoration.h
HEADERS += sectionExperience.h
HEADERS += sectionGeneral.h
HEADERS += sectionGround.h
HEADERS += sectionLord.h
HEADERS += sectionLordCategory.h
HEADERS += sectionMachine.h
HEADERS += sectionRessource.h
HEADERS += sectionSelector.h
HEADERS += sectionTeam.h
HEADERS += sectionWidget.h
HEADERS += themeInterface.h

TRANSLATIONS += ../i18n/de/theme_editor_de.ts
TRANSLATIONS += ../i18n/fr/theme_editor_fr.ts
TRANSLATIONS += ../i18n/ru/theme_editor_ru.ts
TRANSLATIONS += ../i18n/it/theme_editor_it.ts

DESTDIR = ..

OBJECTS_DIR=./obj
MOC_DIR=./moc

TARGET = attal-theme-editor

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
