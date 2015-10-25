%define cvs 1.0-rc1
Summary: A turn based strategy game
Name: attal
Version: 1.0-rc1
Release: 0.%cvs.1ark
License: GPL                                                                    
Group: Amusement/Games                                                    
Source: %name-src-%cvs.tar.lz
Source1: %name-themes-%cvs.tar.lz
#Patch: attal-qt-4.2.patch
BuildRoot: /tmp/%{name}-buildroot

%description
Attal is a turn based strategy game

%prep
%setup -q -n %name-src -a 1
#%patch -p1 -b .qt42~
mv %name-themes themes
export QTDIR=/usr/lib/qt4
export ATT_PREFIX=%_prefix
./autogen.sh

%build 
export LD_LIBRARY_PATH=`pwd`
make %?_smp_mflags

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%_bindir
mkdir -p $RPM_BUILD_ROOT%_libdir                                             
mkdir -p $RPM_BUILD_ROOT%_datadir/games/attal/

install -m 755 attal-server $RPM_BUILD_ROOT%_bindir/attal-server
install -m 755 attal-ai $RPM_BUILD_ROOT%_bindir/attal-ai
install -m 755 attal-client $RPM_BUILD_ROOT%_bindir/attal-client
install -m 755 attal-scenario-editor $RPM_BUILD_ROOT%_bindir/attal-scenario-editor
install -m 755 attal-theme-editor $RPM_BUILD_ROOT%_bindir/attal-theme-editor
install -m 755 attal-campaign-editor $RPM_BUILD_ROOT%_bindir/attal-campaign-editor
install -m 755 *.so* $RPM_BUILD_ROOT%_libdir
cp -r themes $RPM_BUILD_ROOT%_datadir/games/attal/

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README TODO COPYING 
%_bindir/attal-server
%_bindir/attal-ai
%_bindir/attal-client
%_bindir/attal-scenario-editor
%_bindir/attal-theme-editor
%_bindir/attal-campaign-editor
%_libdir/*
%_datadir/games/attal/*

%changelog
* Sun May 21 2006 Lusum
First try
