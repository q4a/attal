#!/bin/sh

cd ../..
mv config.pro config.pro.bak
cp admin/macx-bundle/config.pro ./

# These are the appropriate lines for the default Trolltech package of Qt 4.3
export QTDIR=/usr/ 
export QMAKESPEC=/usr/local/Qt4.3/mkspecs/macx-g++/

# Do not modify below this line
./autogen.sh
make

mkdir -p Attal.app/Contents/MacOS/
mkdir Attal.app/Contents/Resources/
#mkdir Attal.app/Contents/SharedFrameWorks/

cp attal-client attal-theme-editor attal-ai attal-server attal-scenario-editor attal-campaign-editor Attal.app/Contents/MacOS/
#cp admin/macx-bundle/*.xpm Attal.app/Contents/MacOS/
cp admin/macx-bundle/starter Attal.app/Contents/MacOS/
cp -r themes Attal.app/Contents/Resources/
cp -r i18n Attal.app/Contents/Resources/
cp admin/macx-bundle/attal.icns Attal.app/Contents/Resources/

#cp /Library/Frameworks/QtGui.framework/Versions/Current/QtGui

echo "APPL????" > Attal.app/Contents/PkgInfo
echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<!DOCTYPE plist SYSTEM \"file://localhost/System/Library/DTDs/PropertyList.dtd\">
<plist version=\"0.9\">
<dict>
        <key>CFBundleIconFile</key>
        <string>attal.icns</string>
        <key>CFBundlePackageType</key>
        <string>APPL</string>
        <key>CFBundleGetInfoString</key>
        <string>Created by Qt/QMake</string>
        <key>CFBundleSignature</key>
        <string>????</string>
        <key>CFBundleExecutable</key>
        <string>starter</string>
        <key>NOTE</key>
        <string>Please, do NOT change this file -- It was generated by Qt/QMake.</string>
</dict>
</plist>" > Attal.app/Contents/Info.plist

mkdir Attal.app/Contents/Frameworks/
cp -R /Library/Frameworks/QtCore.framework  Attal.app/Contents/Frameworks/
cp -R /Library/Frameworks/QtGui.framework  Attal.app/Contents/Frameworks/
cp -R /Library/Frameworks/QtNetwork.framework  Attal.app/Contents/Frameworks/
cp -R /Library/Frameworks/QtXml.framework Attal.app/Contents/Frameworks/
     
install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore Attal.app/Contents/Frameworks/QtCore.framework/Versions/4.0/QtCore
install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui Attal.app/Contents/Frameworks/QtGui.framework/Versions/4.0/QtGui 
install_name_tool -id @executable_path/../Frameworks/QtNetwork.framework/Versions/4.0/QtNetwork  Attal.app/Contents/Frameworks/QtNetwork.framework/Versions/4.0/QtNetwork
install_name_tool -id @executable_path/../Frameworks/QtXml.framework/Versions/4.0/QtXml Attal.app/Contents/Frameworks/QtXml.framework/Versions/4.0/QtXml

install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore Attal.app/Contents/Frameworks/QtGui.framework/Versions/4.0/QtGui
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore Attal.app/Contents/Frameworks/QtNetwork.framework/Versions/4.0/QtNetwork
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore Attal.app/Contents/Frameworks/QtXml.framework/Versions/4.0/QtXml

for i in Attal.app/Contents/MacOS/*; do install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore $i; done
for i in Attal.app/Contents/MacOS/*; do install_name_tool -change QtGui.framework/Versions/4/QtGui  @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui $i; done
for i in Attal.app/Contents/MacOS/*; do install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework//Versions/4.0/QtNetwork $i; done
for i in Attal.app/Contents/MacOS/*; do install_name_tool -change QtXml.framework/Versions/4/QtXml @executable_path/../Frameworks/QtXml.framework/Versions/4.0/QtXml $i; done

cp *.dylib Attal.app/Contents/Frameworks/
for j in Attal.app/Contents/MacOS/*; do for i in *.dylib; do install_name_tool -change $i @executable_path/../Frameworks/$i $j; done; done
for i in *.dylib; do install_name_tool -id @executable_path/../$i  Attal.app/Contents/Frameworks/$i; done
for i in *.dylib; do for j in Attal.app/Contents/Frameworks/*.dylib; do install_name_tool -change $i @executable_path/../Frameworks/$i $j; done; done
for i in Attal.app/Contents/Frameworks/*.dylib; do install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore $i; done
for i in Attal.app/Contents/Frameworks/*.dylib; do install_name_tool -change QtGui.framework/Versions/4/QtGui  @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui $i; done
for i in Attal.app/Contents/Frameworks/*.dylib; do install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework//Versions/4.0/QtNetwork $i; done
for i in Attal.app/Contents/Frameworks/*.dylib; do install_name_tool -change QtXml.framework/Versions/4/QtXml @executable_path/../Frameworks/QtXml.framework/Versions/4.0/QtXml $i; done

strip -u -r Attal.app/Contents/MacOS/attal-*
strip -S -X -x Attal.app/Contents/Frameworks/libAttal*
rm Attal.app/Contents/Frameworks/libAttal*.*.dylib
cd Attal.app/Contents/Frameworks/
ln -s libAttalAi.dylib libAttalAi.2.0.0.dylib
ln -s libAttalAi.dylib libAttalAi.2.0.dylib
ln -s libAttalAi.dylib libAttalAi.2.dylib
ln -s libAttalClient.dylib libAttalClient.9.0.0.dylib
ln -s libAttalClient.dylib libAttalClient.9.0.dylib
ln -s libAttalClient.dylib libAttalClient.9.dylib
ln -s libAttalFight.dylib libAttalFight.8.0.0.dylib
ln -s libAttalFight.dylib libAttalFight.8.0.dylib
ln -s libAttalFight.dylib libAttalFight.8.dylib
ln -s libAttalServer.dylib libAttalServer.9.0.0.dylib
ln -s libAttalServer.dylib libAttalServer.9.0.dylib
ln -s libAttalServer.dylib libAttalServer.9.dylib