[RU](README.ru.md) | [**EN**](README.md)

---

# BLE Scanner

The project demonstrates working with the SimpleBLE library - a cross-platform library for Bluetooth Low Energy (BLE), developed for simplicity and ease of use of the BlueZ 5 DBus API. This project implements and demonstrates the capabilities of the library for scanning space. Output in the format of a list of found devices, displaying their addresses and names. Methods for connecting to devices and outputting services and characteristics included in the services are implemented.

Build status:
1. example - [![pipeline status](https://gitlab.com/omprussia/examples/BleScanner/badges/example/pipeline.svg)](https://gitlab.com/omprussia/examples/BleScanner/-/commits/example)
2. dev - [![pipeline status](https://gitlab.com/omprussia/examples/BleScanner/badges/dev/pipeline.svg)](https://gitlab.com/omprussia/examples/BleScanner/-/commits/dev)


<a name="Table-of-contents"></a>
## Table of Contents

1. [Table of Contents](#Table-of-contents)
2. [Compatibility](#Compatibility)
3. [Features of use and build](#Features-of-use-and-build)
4. [Branch info](#Branch-info)
5. [Install and Run](#Install-and-Run)
6. [Screenshots](#Screenshots)
7. [Project Structure](#Project-structure)
8. [Terms of Use and Participation in Development](#Terms-of-use-and-participation-in-development)

<a name="Compatibility"></a>
## Compatibility

The project is compatible with all the current versions of the Aurora OS.

<a name="Features-of-use-and-build"></a>
## Features of use and build

This example being built using the Aurora SDK: [Documentation](https://developer.auroraos.ru/doc/sdk/app_development/work/create/examples#build)

<a name="Branch-info"></a>
## Branch info

[Branches](https://developer.auroraos.ru/doc/software_development/examples#branches)

<a name="Install-and-Run"></a>
## Install and Run

The project being built in the usual way using the Aurora SDK: [Documentation](https://developer.auroraos.ru/doc/sdk/app_development/work/build).

<a name="Screenshots"></a>
## Screenshots

![Screenshots](screenshots/screenshots.png)

<a name="Project-structure"></a>
## Project Structure

The project has a standard structure
of an application based on C++ and QML for Aurora OS.

* **[ru.auroraos.BleScanner.pro](ru.auroraos.BleScanner.pro)** file
  describes the subdirectories project that contains two subprojects where the first is responsible
  for building the SimpleBLE library, and the second describes the structure of the application for
  the qmake build system.
* **[icons](app/icons)** directory contains the application icons for different screen resolutions.
* **[qml](app/qml)** directory contains the QML source code and the UI resources.
  * **[cover](app/qml/cover)** directory contains the application cover implementations.
  * **[icons](app/qml/icons)** directory contains the additional custom UI icons.
  * **[pages](app/qml/pages)** directory contains the application pages.
  * **[BleScanner.qml](app/qml/BleScanner.qml)** file
    provides the application window implementation.
* **[rpm](rpm)** directory contains the rpm-package build settings.
  **[ru.auroraos.BleScanner.spec](rpm/ru.auroraos.BleScanner.spec)** file is used by rpmbuild tool.
* **[src](app/src)** directory contains the C++ source code.
  * **[main.cpp](app/src/main.cpp)** file is the application entry point.
* **[translations](app/translations)** directory contains the UI translation files.
* **[ru.auroraos.BleScanner.desktop](app/ru.auroraos.BleScanner.desktop)** file
  defines the display and parameters for launching the application.

<a name="Terms-of-use-and-participation-in-development"></a>
## Terms of Use and Participation in Development

The source code of the project is provided under [the license](LICENSE.BSD-3-CLAUSE.md), which allows its use in third-party applications.

[Contributor License Agreement](https://gitlab.com/omprussia/wiki/-/wikis/CLA)
The [contributor agreement](CONTRIBUTING.md) documents the rights granted by contributors of the Open Mobile Platform.

Information about the contributors is specified in the [AUTHORS](AUTHORS.md) file.

[Code of conduct](CODE_OF_CONDUCT.md) is a current set of rules of the Open Mobile Platform which informs you how we expect the members of the community will interact while contributing and communicating.

