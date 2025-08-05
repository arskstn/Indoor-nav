%define __provides_exclude_from ^%{_datadir}/%{name}/lib/.*$
%define __requires_exclude ^(libsimplebluez.*|libsimpledbus.*|libsimpleble.*)*$

Name: ru.auroraos.BleScanner
Summary: BLE Scanner
Version: 0.1.0
URL:     https://developer.auroraos.ru/open-source
Release: 1
License: BSD-3-Clause
Source0: %{name}-%{version}.tar.bz2
BuildRequires: pkgconfig(auroraapp)
BuildRequires: pkgconfig(Qt5Core)
BuildRequires: pkgconfig(Qt5Qml)
BuildRequires: pkgconfig(Qt5Quick)
BuildRequires: pkgconfig(Qt5Gui)
BuildRequires: pkgconfig(Qt5DBus)
BuildRequires: cmake >= 2.6.3
Requires: sailfishsilica-qt5 >= 0.10.9
Requires: nemo-qml-plugin-notifications-qt5

%description
The project provides an example of using SimpleBLE library to scaning BLE devices.

%prep
%autosetup

%build

mkdir -p $RPM_BUILD_DIR/libs/simpleble \
$RPM_BUILD_DIR/libs/simplebluez \
$RPM_BUILD_DIR/libs/simpledbus

%cmake -S$RPM_SOURCE_DIR/../libs/simplebluez \
-B$RPM_BUILD_DIR/libs/simplebluez \
-DBUILD_SHARED_LIBS=OFF \
-DCMAKE_INSTALL_PREFIX:PATH=$RPM_BUILD_DIR;

%make_build -C $RPM_BUILD_DIR/libs/simplebluez

%cmake -S$RPM_SOURCE_DIR/../libs/simpleble \
-B$RPM_BUILD_DIR/libs/simpleble \
-DCMAKE_INSTALL_PREFIX:PATH=$RPM_BUILD_DIR \
-DBUILD_SHARED_LIBS=OFF;

%make_build -C $RPM_BUILD_DIR/libs/simpleble

%cmake -S$RPM_SOURCE_DIR/../libs/simpledbus \
-B$RPM_BUILD_DIR/libs/simpledbus \
-DCMAKE_INSTALL_PREFIX:PATH=$RPM_BUILD_DIR \
-DBUILD_SHARED_LIBS=OFF;

%make_build -C $RPM_BUILD_DIR/libs/simpledbus

cd $RPM_BUILD_DIR

%qmake5
%make_build


%install
%make_install

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%defattr(644,root,root,-)
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
