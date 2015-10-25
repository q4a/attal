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

SOURCES += campaignEditor.cpp
SOURCES += campaignInterface.cpp
SOURCES += mainCampaignScreen.cpp

HEADERS += campaignInterface.h
HEADERS += mainCampaignScreen.h


DESTDIR = ..

OBJECTS_DIR=./obj
MOC_DIR=./moc

TARGET = attal-campaign-editor

TRANSLATIONS += ../i18n/de/campaign_editor_de.ts
TRANSLATIONS += ../i18n/fr/campaign_editor_fr.ts
TRANSLATIONS += ../i18n/ru/campaign_editor_ru.ts
TRANSLATIONS += ../i18n/it/campaign_editor_it.ts

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

