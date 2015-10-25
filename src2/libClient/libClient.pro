TEMPLATE = lib

include( ../config.pro )
INCLUDEPATH += ..
LIBS += -L..
unix:LIBS += -lAttalCommon
win32:LIBS += -lAttalCommon10

contains( DEFINES, WITH_SOUND ) {
	win32 {
		contains( CONFIG, crosslinwin ) {
			INCLUDEPATH += "$$quote($${SDL_DIR})/include"
				LIBS += -L"$$quote($${SDL_DIR})/lib"
		} else {
			INCLUDEPATH += "$$quote($${SDL_DIR})\include"
				LIBS += -L"$$quote($${SDL_DIR})\lib"
		}
	}
		
	LIBS += -lSDL
	LIBS += -lSDL_mixer
}

VERSION = 10.0.0


SOURCES += aboutDialog.cpp
SOURCES += askChest.cpp
SOURCES += askCost.cpp
SOURCES += askDialog.cpp
SOURCES += askWidget.cpp
SOURCES += attalButton.cpp
SOURCES += attalSound.cpp
SOURCES += attalSprite.cpp
SOURCES += attalStyle.cpp
SOURCES += bonus.cpp
SOURCES += building.cpp
SOURCES += cell.cpp
SOURCES += chatWidget.cpp
SOURCES += chest.cpp
SOURCES += displayArtefacts.cpp
SOURCES += displayBase.cpp
SOURCES += displayCreature.cpp
SOURCES += displayLord.cpp
SOURCES += displayTechnics.cpp
SOURCES += event.cpp
SOURCES += flag.cpp
SOURCES += gainLevel.cpp
SOURCES += game.cpp
SOURCES += gameControl.cpp
SOURCES += gameInfo.cpp
SOURCES += graphicalArtefact.cpp
SOURCES += graphicalBuilding.cpp
SOURCES += graphicalCell.cpp
SOURCES += graphicalGameData.cpp
SOURCES += graphicalLord.cpp
SOURCES += graphicalMap.cpp
SOURCES += graphicalPath.cpp
SOURCES += gui.cpp
SOURCES += imageTheme.cpp
SOURCES += insideAction.cpp
SOURCES += insideBase.cpp
SOURCES += insideBuilding.cpp
SOURCES += lord.cpp
SOURCES += lordExchange.cpp
SOURCES += map.cpp
SOURCES += mapCreature.cpp
SOURCES += mapView.cpp
SOURCES += market.cpp
SOURCES += miniMap.cpp
SOURCES += optionsDialog.cpp
SOURCES += pixmapSpinBox.cpp
SOURCES += player.cpp
SOURCES += ressourceBar.cpp
SOURCES += ressourceWin.cpp
SOURCES += unitExchange.cpp
SOURCES += tavern.cpp
SOURCES += widget.cpp

HEADERS += aboutDialog.h
HEADERS += askChest.h
HEADERS += askCost.h
HEADERS += askDialog.h
HEADERS += askWidget.h
HEADERS += attalButton.h
HEADERS += attalSound.h
HEADERS += attalSprite.h
HEADERS += attalStyle.h
HEADERS += bonus.h
HEADERS += building.h
HEADERS += cell.h
HEADERS += chatWidget.h
HEADERS += chest.h
HEADERS += displayArtefacts.h
HEADERS += displayBase.h
HEADERS += displayCreature.h
HEADERS += displayLord.h
HEADERS += displayTechnics.h
HEADERS += event.h
HEADERS += flag.h
HEADERS += gainLevel.h
HEADERS += game.h
HEADERS += gameControl.h
HEADERS += gameInfo.h
HEADERS += graphicalArtefact.h
HEADERS += graphicalBuilding.h
HEADERS += graphicalCell.h
HEADERS += graphicalGameData.h
HEADERS += graphicalLord.h
HEADERS += graphicalMap.h
HEADERS += graphicalPath.h
HEADERS += gui.h
HEADERS += imageTheme.h
HEADERS += insideAction.h
HEADERS += insideBase.h
HEADERS += insideBuilding.h
HEADERS += lord.h
HEADERS += lordExchange.h
HEADERS += map.h
HEADERS += mapCreature.h
HEADERS += mapView.h
HEADERS += market.h
HEADERS += miniMap.h
HEADERS += optionsDialog.h
HEADERS += pixmapSpinBox.h
HEADERS += player.h
HEADERS += ressourceBar.h
HEADERS += ressourceWin.h
HEADERS += unitExchange.h
HEADERS += tavern.h
HEADERS += widget.h

TRANSLATIONS += ../i18n/de/attal_libclient_de.ts
TRANSLATIONS += ../i18n/fr/attal_libclient_fr.ts
TRANSLATIONS += ../i18n/ru/attal_libclient_ru.ts
TRANSLATIONS += ../i18n/it/attal_libclient_it.ts

DESTDIR = ..

OBJECTS_DIR=./obj
MOC_DIR=./moc

TARGET = AttalClient

unix {
	target.path = $${ATT_LIB_PREFIX}
	INSTALLS += target
}

#The following line was inserted by qt3to4
QT +=  xml network
