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
# define ATT_CROSS_COMPILE if you do a cross compilation from unix to win32)
# Comment 'DEFINES += WITH_SOUND' if you want to disable sound support (sound need SDL and SDL_mixer installed )

# If you are using a version of qt with windows-style as plugin, you may
# try to add these 2 lines 'DEFINES += QT_PLUGIN' and 'LIBS += -lqwindowsstyle' (maybe it is not enough, I do not know

exists( devel.pro ) {
	include( devel.pro )
} else {

	CONFIG += qt
	CONFIG += warn_on
	CONFIG -= debug
	#CONFIG += staticlib

	!isEmpty( ATT_CROSS_COMPILE ) {
		CONFIG += crosslinwin
	}
	
	!isEmpty( ATT_DEBUG ) {
		CONFIG += debug
	}
	
	!isEmpty( ATT_DEBUG_WARNING ) {
		QMAKE_CFLAGS+=-Werror
		QMAKE_CXXFLAGS+=-Werror
	}
	
	isEmpty( ATT_GPROF ) {
		isEmpty( ATT_GCOV ) {
			QMAKE_CFLAGS+=-O2
			QMAKE_CXXFLAGS+=-O2
		}
	}

	!isEmpty( ATT_GPROF ) {
		CONFIG += staticlib
		QMAKE_CFLAGS+=-pg
		QMAKE_CXXFLAGS+=-pg
		QMAKE_LFLAGS+=-pg
	}

	!isEmpty( ATT_GCOV ) {
		QMAKE_CFLAGS+=-fprofile-arcs -ftest-coverage
		QMAKE_CXXFLAGS+=-fprofile-arcs -ftest-coverage
		QMAKE_LFLAGS+=-fprofile-arcs -lgcov
	}
	
	isEmpty( ATTAL_VERSION ) {
	 ATTAL_VERSION=1.0-rc3
	}

	DEFINES += ATTAL_VERSION=\\\"$$ATTAL_VERSION\\\"
	DEFINES += WITH_SOUND

unix:!macx {
	isEmpty( ATT_PREFIX ) {
		ATT_PREFIX = /usr/local
	}
	
	isEmpty( ATT_LIB_PREFIX ) {
		ATT_LIB_PREFIX = $${ATT_PREFIX}/lib/
	}

	isEmpty( ATT_THEMES_PREFIX ) {
		ATT_THEMES_PREFIX = $${ATT_PREFIX}/share/games/attal/themes/ #Themes final directory
	}

	isEmpty( ATT_TRANSL_PREFIX ) {
		ATT_TRANSL_PREFIX = $${ATT_PREFIX}/share/games/attal/translations/  #Traslations final directory
	}
	
	isEmpty( ATT_DOCS_PREFIX ) {
		ATT_DOCS_PREFIX = $${ATT_PREFIX}/share/doc/attal/  #Docs final directory
	}

	isEmpty( ATT_TEMP_THEMES_PREFIX ) {
		ATT_TEMP_THEMES_PREFIX = $${ATT_THEMES_PREFIX} #Change it if themes final directory is different from installation dir
	}
	
	isEmpty( ATT_TEMP_TRANSL_PREFIX ) {
		ATT_TEMP_TRANSL_PREFIX = $${ATT_TRANSL_PREFIX} #Change it if translations final directory is different from installation dir
	}
}

win32 {
	debug {
		CONFIG += console
	}
}

	DEFINES += ATT_THEMES_DIR=\\\"$$ATT_THEMES_PREFIX\\\"
	DEFINES += ATT_TRANSL_DIR=\\\"$$ATT_TRANSL_PREFIX\\\"
	DEFINES += ATT_DOCS_DIR=\\\"$$ATT_DOCS_PREFIX\\\"

	# For sound under windows, enter here the path to SDL root dir and uncomment line
	#win32:SDL_DIR = <ENTER PATH>
	
	# For compilers different from gcc and mingw, uncomment line (not tested)
	#DEFINES +=WIN32VC6
}
