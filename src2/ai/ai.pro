TEMPLATE = app

include( ../config.pro )
INCLUDEPATH += ..

LIBS += -L..
unix:LIBS += -lAttalServer
unix:LIBS += -lAttalCommon
unix:LIBS += -lAttalAi

win32:LIBS += -lAttalServer10
win32:LIBS += -lAttalCommon10
win32:LIBS += -lAttalAi3

SOURCES += aiInterface.cpp
SOURCES += main.cpp

HEADERS += aiInterface.h

TRANSLATIONS += ../i18n/de/attal_ai_de.ts
TRANSLATIONS += ../i18n/fr/attal_ai_fr.ts
TRANSLATIONS += ../i18n/ru/attal_ai_ru.ts
TRANSLATIONS += ../i18n/it/attal_ai_it.ts

DESTDIR = ..
OBJECTS_DIR=./obj
MOC_DIR=./moc

TARGET = attal-ai 
win32 {
	contains( CONFIG, debug ) {
		CONFIG += console
	}
}
QT +=  xml network

unix:!macx {
	QMAKE_LFLAGS += -Wl,-rpath,.
}

unix {
	target.path = $${ATT_PREFIX}/bin/
	INSTALLS += target
}

