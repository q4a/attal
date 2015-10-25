TEMPLATE = lib

include( ../config.pro )
INCLUDEPATH += ..
LIBS += -L..

unix:LIBS += -lAttalCommon
win32:LIBS += -lAttalCommon10

unix:LIBS += -lAttalServer
win32:LIBS += -lAttalServer10

VERSION = 3.0.0

SOURCES += aiLord.cpp
SOURCES += analyst.cpp

HEADERS += aiLord.h
HEADERS += analyst.h

TRANSLATIONS += ../i18n/de/attal_libai_de.ts
TRANSLATIONS += ../i18n/fr/attal_libai_fr.ts
TRANSLATIONS += ../i18n/ru/attal_libai_ru.ts
TRANSLATIONS += ../i18n/it/attal_libai_it.ts

DESTDIR = ..

OBJECTS_DIR=./obj
MOC_DIR=./moc

TARGET = AttalAi

unix {
	target.path = $${ATT_LIB_PREFIX}
	INSTALLS += target
}

#The following line was inserted by qt3to4
QT +=  xml network 

