#!/bin/bash

cd ..

echo PREPARE ENVIRONMENT VARIABLES
export QTDIR=/usr/share/qt4
export QMAKESPEC=/usr/share/qt4/mkspecs/linux-g++
export VERSION=$(grep -m 1 ATTAL_VERSION= config.pro | tr -d [[:alpha:]] | tr -d [[:blank:]] | tr -d = | tr -d _ | tr -d -)
export CURDIR=$(dirs -l)
export ATT_PREFIX=/usr

#echo PREPARE SRC-ROOT
#	make clean 1> /dev/null

echo MAKE SURE WE HARDCODE EVERYTHING WITH /usr PREFIX - DEBIAN STANDARD
	#cat config.pro | sed 's#/usr/local#/usr#g' | tee config-mod.pro
	#mv -f config-mod.pro config.pro
	./autogen.sh 1> /dev/null
	make

echo MAKE SURE WE INSTALL IN A SUBDIRECTORY OF SRC-ROOT, SO WE CAN PACK UP LATER
	make INSTALL_ROOT="$CURDIR"/debian/attal/ install


echo PREPARE attal DEBIAN PACKAGE FOLDERS following debian naming
	
	mkdir -p debian/attal/usr/share/doc/attal 1> /dev/null
	cp HOWTOPLAY.html debian/attal/usr/share/doc/attal/HOWTOPLAY.html 1> /dev/null
	cp AUTHORS debian/attal/usr/share/doc/attal/AUTHORS 1> /dev/null
	cp INSTALL debian/attal/usr/share/doc/attal/INSTALL 1> /dev/null
	cp COPYING debian/attal/usr/share/doc/attal/copyright 1> /dev/null
	cp NEWS debian/attal/usr/share/doc/attal/changelog.Debian 1> /dev/null
	gzip -9 debian/attal/usr/share/doc/attal/changelog.Debian 1> /dev/null

	mkdir -p debian/attal/usr/share/applications
	cp admin/debian/attal.desktop debian/attal/usr/share/applications/attal.desktop 1> /dev/null
	cp admin/debian/attalscenario.desktop debian/attal/usr/share/applications/attalscenario.desktop 1> /dev/null
	cp admin/debian/attalserver.desktop debian/attal/usr/share/applications/attalserver.desktop 1> /dev/null
	cp admin/debian/attaltheme.desktop debian/attal/usr/share/applications/attaltheme.desktop 1> /dev/null

	mkdir -p debian/attal/usr/share/menu
	echo '?package(attal):needs="X11" section="Games/Strategy" \'  > debian/attal/usr/share/menu/attal
	echo 'title="Attal" command="/usr/bin/attal-client" ' >> debian/attal/usr/share/menu/attal
	echo '?package(attal):needs="X11" section="Games/Strategy" \'  > debian/attal/usr/share/menu/attalserver
	echo 'title="Attal-server" command="/usr/bin/attal-server" ' >> debian/attal/usr/share/menu/attalserver
	echo '?package(attal):needs="X11" section="Games/Strategy" \'  > debian/attal/usr/share/menu/attalscenario
	echo 'title="Attal-scenario-editor" command="/usr/bin/attal-scenario-editor" ' >> debian/attal/usr/share/menu/attalscenario
	echo '?package(attal):needs="X11" section="Games/Strategy" \'  > debian/attal/usr/share/menu/attaltheme
	echo 'title="Attal-theme-editor="/usr/bin/attal-theme-editor" ' >> debian/attal/usr/share/menu/attaltheme
	echo '?package(attal):needs="X11" section="Games/Strategy" \'  > debian/attal/usr/share/menu/attalcampaign
	echo 'title="Attal-campaign-editor command="/usr/bin/attal-campaign-editor" ' >> debian/attal/usr/share/menu/attalcampaign

echo PREPARE attal-themes-medieval DEBIAN PACKAGE FOLDERS following debian naming
	mkdir -p debian/attal-themes-medieval/usr/share/games/attal/themes
	mv debian/attal/usr/share/games/attal/themes/medieval debian/attal-themes-medieval/usr/share/games/attal/themes/
	mkdir -p debian/attal-themes-medieval/usr/share/doc/attal-themes-medieval 1> /dev/null

	mkdir debian/attal-themes-medieval/DEBIAN
	export SIZE=$(du -s debian/attal-themes-medieval/ | tr -d [[:alpha:]] | tr -d [[:blank:]])
	echo 	Package: attal-themes-medieval > debian/attal-themes-medieval/DEBIAN/control
	echo	Version: $VERSION >> debian/attal-themes-medieval/DEBIAN/control
	echo	Section: contrib >> debian/attal-themes-medieval/DEBIAN/control
	echo	Priority: optional >> debian/attal-themes-medieval/DEBIAN/control
	echo	Architecture: all >> debian/attal-themes-medieval/DEBIAN/control
	echo  'Recommends: attal (>= 1.0~rc2)' >> debian/attal-themes-medieval/DEBIAN/control
	echo	Installed-Size: $SIZE >> debian/attal-themes-medieval/DEBIAN/control
	echo	Maintainer: Lusum \(lusum@sourceforge.net\) >> debian/attal-themes-medieval/DEBIAN/control
	echo	Description: Attal - Lords of Doom >> debian/attal-themes-medieval/DEBIAN/control
	echo  " Attal is an turn-based strategy game " >> debian/attal-themes-medieval/DEBIAN/control
	echo  " that can be played alone (against AI) " >> debian/attal-themes-medieval/DEBIAN/control
	echo  " or against other through a network. " >> debian/attal-themes-medieval/DEBIAN/control
	echo  " " >> debian/attal-themes-medieval/DEBIAN/control
	echo  " This package provides the medieval theme for Attal" >> debian/attal-themes-medieval/DEBIAN/control



echo PREPARE attal-themes-cyberpunk DEBIAN PACKAGE FOLDERS following debian naming
	mkdir -p debian/attal-themes-cyberpunk/usr/share/games/attal/themes
	mv debian/attal/usr/share/games/attal/themes/cyberpunk debian/attal-themes-cyberpunk/usr/share/games/attal/themes/

	cp -Rf debian/attal-themes-medieval/usr/share/doc/ debian/attal-themes-cyberpunk/usr/share/
	mv debian/attal-themes-cyberpunk/usr/share/doc/attal-themes-medieval debian/attal-themes-cyberpunk/usr/share/doc/attal-themes-cyberpunk

	mkdir debian/attal-themes-cyberpunk/DEBIAN
	export SIZE=$(du -s debian/attal-themes-cyberpunk/ | tr -d [[:alpha:]] | tr -d [[:blank:]])
	echo 	Package: attal-themes-cyberpunk > debian/attal-themes-cyberpunk/DEBIAN/control
	echo	Version: $VERSION >> debian/attal-themes-cyberpunk/DEBIAN/control
	echo	Section: contrib >> debian/attal-themes-cyberpunk/DEBIAN/control
	echo	Priority: optional >> debian/attal-themes-cyberpunk/DEBIAN/control
	echo	Architecture: all >> debian/attal-themes-cyberpunk/DEBIAN/control
	echo  'Recommends: attal (>= 1.0~rc2)' >> debian/attal-themes-medieval/DEBIAN/control
	echo	Installed-Size: $SIZE >> debian/attal-themes-cyberpunk/DEBIAN/control
	echo	Maintainer: Lusum \(lusum@sourceforge.net\) >> debian/attal-themes-cyberpunk/DEBIAN/control
	echo	Description: Attal - Lords of Doom >> debian/attal-themes-cyberpunk/DEBIAN/control
	echo  " Attal is an turn-based strategy game " >> debian/attal-themes-cyberpunk/DEBIAN/control
	echo  " that can be played alone (against AI) " >> debian/attal-themes-cyberpunk/DEBIAN/control
	echo  " or against other through a network. " >> debian/attal-themes-cyberpunk/DEBIAN/control
	echo  " " >> debian/attal-themes-cyberpunk/DEBIAN/control
	echo  " This package provides the cyberpunk theme for Attal (unusable)" >> debian/attal-themes-cyberpunk/DEBIAN/control


rm -Rf debian/attal/usr/share/games/attal/themes/


echo PREPARE attal DEBIAN PACKAGE FOLDERS
	cp -Rf debian/attal-themes-medieval/usr/share/doc/ debian/attal/usr/share/
	mv debian/attal/usr/share/doc/attal-themes-medieval debian/attal/usr/share/doc/attal	

	mkdir debian/attal/DEBIAN
	export SIZE=$(du -s debian/attal | tr -d [[:alpha:]] | tr -d [[:blank:]])
	echo 	Package: attal > debian/attal/DEBIAN/control
	echo	Version: $VERSION >> debian/attal/DEBIAN/control
	echo	Section: contrib >> debian/attal/DEBIAN/control
	echo	Priority: optional >> debian/attal/DEBIAN/control
	echo	Architecture: i386 >> debian/attal/DEBIAN/control
	echo	'Depends: attal-themes-medieval (>= 1.0~rc2), libc6 (>= 2.7-1), libgcc1 (>= 1:4.1.1-21),libqt4-core (>= 4.4.0) ,libqt4-gui (>= 4.4.0) ,libsdl-mixer1.2' >> debian/attal/DEBIAN/control
	echo	Installed-Size: $SIZE >> debian/attal/DEBIAN/control
	echo	Maintainer: Lusum \(lusum@sourceforge.net\) >> debian/attal/DEBIAN/control
	echo	Description: Attal - Lords of Doom >> debian/attal/DEBIAN/control
	echo  " Attal is an turn-based strategy game " >> debian/attal/DEBIAN/control
	echo  " that can be played alone (against AI) " >> debian/attal/DEBIAN/control
	echo  " or against other through a network. " >> debian/attal/DEBIAN/control

	echo '#!/bin/sh' > debian/attal/DEBIAN/postinst
	echo 'set -e' >> debian/attal/DEBIAN/postinst
	echo 'if [ "$1" = "configure" ] && [ -x "`which update-menus 2>/dev/null`" ]; then' >> debian/attal/DEBIAN/postinst
	echo '	update-menus' >> debian/attal/DEBIAN/postinst
	echo 'fi' >> debian/attal/DEBIAN/postinst
	echo 'if [ "$1" = "configure" ]; then' >> debian/attal/DEBIAN/postinst
	echo '	ldconfig' >> debian/attal/DEBIAN/postinst
	echo 'fi' >> debian/attal/DEBIAN/postinst
	chmod 755 debian/attal/DEBIAN/postinst

	echo '#!/bin/sh' > debian/attal/DEBIAN/postrm
	echo 'set -e' >> debian/attal/DEBIAN/postrm
	echo 'if [ -x "`which update-menus 2>/dev/null`" ]; then' >> debian/attal/DEBIAN/postrm
	echo '	update-menus' >> debian/attal/DEBIAN/postrm
	echo 'fi' >> debian/attal/DEBIAN/postrm
	echo 'if [ "$1" = "remove" ]; then' >> debian/attal/DEBIAN/postrm
	echo '	ldconfig' >> debian/attal/DEBIAN/postrm
	echo 'fi' >> debian/attal/DEBIAN/postrm
	chmod 755 debian/attal/DEBIAN/postrm

echo BUILD THE PACKAGES
	dpkg-deb -b debian/attal/ attal_"$VERSION"_i386.deb
	dpkg-deb -b debian/attal-themes-medieval/ attal-themes-medieval_"$VERSION"_i386.deb
	dpkg-deb -b debian/attal-themes-cyberpunk/ attal-themes-cyberpunk_"$VERSION"_i386.deb

#echo CLEAN UP
	rm -Rf debian 1> /dev/null
	make clean 1> /dev/null
