echo "Building Makefile..."

if [ -z "$QTDIR" ]
	then
		echo "You have to set the qt4 libs directory : export QTDIR= "
	exit
fi

if [ -n "$ATT_PREFIX" ]
	then
	pref="ATT_PREFIX=$ATT_PREFIX"
fi

if [ -n "$ATT_LIB_PREFIX" ]
	then
	lib="ATT_LIB_PREFIX=$ATT_LIB_PREFIX"
fi

if [ -n "$ATT_THEMES_PREFIX" ]
	then
	par2="ATT_THEMES_PREFIX=$ATT_THEMES_PREFIX"
fi

if [ -n "$ATT_TRANSL_PREFIX" ]
	then
	par3="ATT_TRANSL_PREFIX=$ATT_TRANSL_PREFIX"
fi

if [ -n "$ATT_DOCS_PREFIX" ]
	then
	attdocs="ATT_DOCS_PREFIX=$ATT_DOCS_PREFIX"
fi

if [ -n "$ATT_TEMP_THEMES_PREFIX" ]
	then
	par5="ATT_TEMP_THEMES_PREFIX=$ATT_TEMP_THEMES_PREFIX"
fi

if [ -n "$ATT_TEMP_TRANSL_PREFIX" ]
	then
	par6="ATT_TEMP_TRANSL_PREFIX=$ATT_TEMP_TRANSL_PREFIX"
fi

if [ -z "$ATT_QMAKE" ]
	then
	ATT_QMAKE=$QTDIR/bin/qmake
fi

if [ -z "$ATT_LUPDATE" ]
	then
	ATT_LUPDATE=$QTDIR/bin/lupdate
fi

if [ -z "$ATT_LRELEASE" ]
	then
	ATT_LRELEASE=$QTDIR/bin/lrelease
fi

if [ -n "$ATT_CROSS_COMPILE" ]
	then
	unset par1
	unset par2
	unset par3
	cross="ATT_CROSS_COMPILE=$ATT_CROSS_COMPILE"
fi

if [ -n "$ATT_GPROF" ]
	then
	gprof="ATT_GPROF=$ATT_GPROF"
fi

if [ -n "$ATT_GCOV" ]
	then
	gcov="ATT_GCOV=$ATT_GCOV"
fi

if [ -n "$ATT_DEBUG" ]
	then
	debug="ATT_DEBUG=$ATT_DEBUG"
fi

if [ -n "$ATT_DEBUG_WARNING" ]
	then
	debugW="ATT_DEBUG_WARNING=$ATT_DEBUG_WARNING"
fi


echo "$ATT_QMAKE $pref $lib $par2 $par3 $par5 $par6 $attdocs $cross $gprof $gcov $debug $debugW -o Makefile Makefile.pro"
$ATT_QMAKE $pref $lib $par2 $par3 $par5 $par6 $attdocs $cross $gprof $gcov $debug $debugW -o Makefile Makefile.pro

echo "Done"

echo "Creating national language files in i18n..."
$ATT_LUPDATE -noobsolete -silent server/server.pro 2>/dev/null
$ATT_LUPDATE -noobsolete -silent client/client.pro 2>/dev/null
$ATT_LUPDATE -noobsolete -silent ai/ai.pro 2>/dev/null
$ATT_LUPDATE -noobsolete -silent scenarioEditor/scenarioEditor.pro 2>/dev/null
$ATT_LUPDATE -noobsolete -silent themeEditor/themeEditor.pro 2>/dev/null
$ATT_LUPDATE -noobsolete -silent campaingEditor/campaignEditor.pro 2>/dev/null
$ATT_LUPDATE -noobsolete -silent libServer/libServer.pro 2>/dev/null
$ATT_LUPDATE -noobsolete -silent libClient/libClient.pro 2>/dev/null
$ATT_LUPDATE -noobsolete -silent libCommon/libCommon.pro 2>/dev/null
$ATT_LUPDATE -noobsolete -silent libAi/libAi.pro 2>/dev/null
$ATT_LUPDATE -noobsolete -silent libFight/libFight.pro 2>/dev/null
$ATT_LUPDATE -noobsolete -silent themes/themes.pro 2>/dev/null
echo "Done"

echo "Compiling existing translations..."
$ATT_LRELEASE -silent i18n/??/attal_*_??.ts
$ATT_LRELEASE -silent i18n/??/attal_*_??.ts
$ATT_LRELEASE -silent i18n/??/attal_*_??.ts
$ATT_LRELEASE -silent i18n/??/attal_*_??.ts
$ATT_LRELEASE -silent i18n/??/campaign_*_??.ts
$ATT_LRELEASE -silent i18n/??/theme_*_??.ts
$ATT_LRELEASE -silent i18n/??/scenario_*_??.ts
echo "Done"

echo "You may now 'make && make install'"



