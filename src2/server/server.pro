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

win32:DEFINE += WIN32

win32 {
	contains( CONFIG, debug ) {
		CONFIG += console
	}
}

SOURCES += displayScenarii.cpp
SOURCES += serverWidgets.cpp
SOURCES += serverInterface.cpp
SOURCES += main.cpp

HEADERS += displayScenarii.h
HEADERS += serverWidgets.h
HEADERS += serverInterface.h

TRANSLATIONS += ../i18n/de/attal_server_de.ts
TRANSLATIONS += ../i18n/fr/attal_server_fr.ts
TRANSLATIONS += ../i18n/ru/attal_server_ru.ts
TRANSLATIONS += ../i18n/it/attal_server_it.ts

DESTDIR = ..

OBJECTS_DIR=./obj
MOC_DIR=./moc

TARGET = attal-server

unix:!macx {
	QMAKE_LFLAGS += -Wl,-rpath,.
}

unix {
	target.path = $${ATT_PREFIX}/bin/
	INSTALLS += target
}

#The following line was inserted by qt3to4
QT += network xml

