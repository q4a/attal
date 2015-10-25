
SUBDIRS = common server ia client scenarioEditor themeEditor 

EXTRA_DIST = AUTHORS COPYING ChangeLog INSTALL README TODO 

#MAKE = nice -20 make -j 3

attal.clean : maintainer-clean
	rm -f configure Makefile{,.in}
	rm -Rf */.deps
	rm -f */Makefile{,.in}
	rm -f `find -name core`

AUTOMAKE_OPTIONS = foreign 

tags:
	ctags `find * -name '*.h' -o -name '*.cpp' | grep -v moc | grep /`

