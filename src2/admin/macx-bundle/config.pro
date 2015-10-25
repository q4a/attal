#
# config.pro
# File included by any Makefile.pro
#
# if you are a developper (using cvs), don't change this file but
# create a devel.pro file with your options.
#
# Otherwise:
#
# Uncomment 'CONFIG += staticlib' if you want static libs and static link
# Uncomment 'CONFIG += debug' if you do want debugging symbols to be added (may be add next line "CONFIG += debug")
# export ATT_CROSS_COMPILE if you do a cross compilation from unix to win32)
# Comment 'DEFINES += WITH_SOUND' if you want to enable sound support (need SDL and SDL_mixer installed)

# If you are using a version of qt with windows-style as plugin, you may
# try to add these 2 lines 'DEFINES += QT_PLUGIN' and 'LIBS += -lqwindowsstyle' (maybe it is not enough, I do not know

exists( devel.pro ) {
	include( devel.pro )
} else {

	CONFIG += qt
	CONFIG += warn_on
	#CONFIG += debug
	#CONFIG += staticlib

	!isEmpty( ATT_CROSS_COMPILE ) {
		CONFIG += crosslinwin
	}
	
	isEmpty( ATTAL_VERSION ) {
	 ATTAL_VERSION=1.0-rc1
	}

	DEFINES += ATTAL_VERSION=\\\"$$ATTAL_VERSION\\\"
	#DEFINES += WITH_SOUND

unix:!macx {
	isEmpty( ATT_PREFIX ) {
		ATT_PREFIX = /usr
	}

	isEmpty( ATT_THEMES_PREFIX ) {
		ATT_THEMES_PREFIX = $${ATT_PREFIX}/share/games/attal/themes/ #Themes final directory
	}

	isEmpty( ATT_TRANSL_PREFIX ) {
		ATT_TRANSL_PREFIX = $${ATT_PREFIX}/share/games/attal/translations/  #Traslations final directory
	}

	isEmpty( ATT_TEMP_THEMES_PREFIX ) {
		ATT_TEMP_THEMES_PREFIX = $${ATT_THEMES_PREFIX} #Change it if themes final directory is different from installation dir
	}
	
	isEmpty( ATT_TEMP_TRANSL_PREFIX ) {
		ATT_TEMP_TRANSL_PREFIX = $${ATT_TRANSL_PREFIX} #Change it if translations final directory is different from installation dir
	}

	DEFINES += ATT_THEMES_DIR=\\\"$$ATT_THEMES_PREFIX\\\"
	DEFINES += ATT_TRANSL_DIR=\\\"$$ATT_TRANSL_PREFIX\\\"
}

macx {
	CONFIG -= app_bundle	#We don't want multiple bundles - we create one for all binaries
	ATT_PREFIX = . 
	ATT_THEMES_PREFIX = $${ATT_PREFIX}/themes/
	ATT_TRANSL_PREFIX = $${ATT_PREFIX}/i18n/
	ATT_TEMP_THEMES_PREFIX = $${ATT_THEMES_PREFIX}
	ATT_TEMP_TRANSL_PREFIX = $${ATT_TRANSL_PREFIX}
	DEFINES += ATT_THEMES_DIR=\\\"$$ATT_THEMES_PREFIX\\\"
	DEFINES += ATT_TRANSL_DIR=\\\"$$ATT_TRANSL_PREFIX\\\"

	#INCPATH += /Library/Frameworks/SDL.framework/Headers/       #Enable these, if you want to try to compile with sound, but for me sound doesn't work anyway, so I disabled it for now
	#LIBS += -L/Library/Frameworks/SDL.framework/SDL -L/Library/Frameworks/SDL_mixer.framework/SDL_mixer -framework SDL -framework SDL_mixer -framework QtNetwork
	LIBS += -framework QtNetwork 	#Don't know if it's my Qt, but there seems to be a problem if that is missing
}

	# For sound under windows, enter here the path to SDL root dir and uncomment line
	#win32:SDL_DIR = <ENTER PATH>
	# For compilers different from gcc and mingw, uncomment line (not tested)
	#DEFINES +=WIN32VC6
}
