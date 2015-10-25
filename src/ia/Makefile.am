# include files
INCLUDES= -I.. $(all_includes)
attal_appsdir = $(kde_appsdir)/Games/Attal

# compilation
CXXFLAGS = -pipe -Wall -g # -O2

#linkage
LDFLAGS = #-s 


noinst_LIBRARIES = libattal_ia.a

# libattal_ia.a
libattal_ia_a_SOURCES =

libattal_ia_a_METASOURCES = USE_AUTOMOC

noinst_HEADERS =


bin_PROGRAMS = attal-ia

#attal_ia
attal_ia_METASOURCES = USE_AUTOMOC
attal_ia_LDFLAGS = $(all_libraries)
attal_ia_SOURCES = 	\
	aiInterface.cpp	\
	analyst.cpp	\
	main.cpp
attal_ia_LDADD   = ../ia/libattal_ia.a ../server/libattal_server.a ../common/libattal.a -lqt -lXext -lX11 #$(LIBSOCKET)

CLEANFILES = $(libattal_ia_a_METASOURCES) $(attal_ia_METASOURCES) attal-*.log

# make messages.po. Move this one to ../po/ and "make merge" in po
# the -x is for skipping messages already translated in kdelibs
messages:
	$(XGETTEXT) -C -ki18n -x ../../../kdelibs/po/kde.pot $(libattal_client_a_SOURCES) $(attal_client_SOURCES) && mv messages.po ../po/attalclient.pot

#(un)install stuff
install-data-local:
	$(mkinstalldirs) $(attal_appsdir)/
	$(INSTALL_DATA) attal-client.kdelnk $(attal_appsdir)/attal-client.kdelnk
	$(mkinstalldirs) $(kde_icondir)/
	$(INSTALL_DATA) attal-editor.xpm $(kde_icondir)/attal-editor.xpm
	$(mkinstalldirs) $(kde_minidir)/
	$(INSTALL_DATA) mini-attal-client.xpm $(kde_minidir)/mini-attal-client.xpm

uninstall-local:
	-rm -f $(attal_appsdir)/attal-client.kdelnk
	-rm -f $(kde_icondir)/attal-client.xpm
	-rm -f $(kde_minidir)/mini-attal-client.xpm
