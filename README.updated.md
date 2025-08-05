<document filename="README.updated.md">
[**RU**](README.updated.ru.md) | [EN](README.updated.md)
<hr>
<h1>BLE Scanner with Triangulation and Map</h1>
<p>The project extends the functionality of the original <code>ru.auroraos.BleScanner</code>, which demonstrated the use of the SimpleBLE library—a cross-platform Bluetooth Low Energy (BLE) library designed for ease of use with the BlueZ 5 DBus API. The application now not only scans the environment and displays a list of devices with their addresses and names but also implements position triangulation based on RSSI from BLE beacons, displays the position on a map, and provides enhanced diagnostics. It supports device connection methods and the display of services and characteristics.</p>
<p>Build Status:</p>
<ol>
<li>example - <a href="https://gitlab.com/omprussia/examples/BleScanner/-/commits/example"><image-card alt="pipeline status" src="https://gitlab.com/omprussia/examples/BleScanner/badges/example/pipeline.svg"></image-card></a></li>
<li>dev - <a href="https://gitlab.com/omprussia/examples/BleScanner/-/commits/dev"><image-card alt="pipeline status" src="https://gitlab.com/omprussia/examples/BleScanner/badges/dev/pipeline.svg"></image-card></a></li>
</ol>
<p><a name="Contents"></a></p>
<h2>Contents</h2>
<ol>
<li><a href="#Contents">Contents</a></li>
<li><a href="#Compatibility">Compatibility</a></li>
<li><a href="#Usage-and-Build-Features">Usage and Build Features</a></li>
<li><a href="#Branch-Information">Branch Information</a></li>
<li><a href="#Installation-and-Launch">Installation and Launch</a></li>
<li><a href="#Screenshots">Screenshots</a></li>
<li><a href="#Project-Structure">Project Structure</a></li>
<li><a href="#Usage-Rules-and-Development-Participation">Usage Rules and Development Participation</a></li>
<li><a href="#Project-Enhancements">Project Enhancements</a>
<ul>
<li><a href="#Adding-Triangulation-and-Position-Calculation">Adding Triangulation and Position Calculation</a></li>
<li><a href="#Updating-Beacon-Coordinates">Updating Beacon Coordinates</a></li>
<li><a href="#Displaying-Position-on-Map">Displaying Position on Map</a></li>
<li><a href="#Synchronization-of-blescanmodel">Synchronization of bleScanModel</a></li>
<li><a href="#Diagnostics-and-Debugging">Diagnostics and Debugging</a></li>
<li><a href="#Error-Handling-and-Stability">Error Handling and Stability</a></li>
<li><a href="#The-Beacons">The Beacons</a></li>
<li><a href="#The-Map">The Map</a></li>
</ul>
</li>
</ol>
<p><a name="Compatibility"></a></p>
<h2>Compatibility</h2>
<p>The project is compatible with all current versions of the Aurora OS.</p>
<p><a name="Usage-and-Build-Features"></a></p>
<h2>Usage and Build Features</h2>
<p>This sample application is built using the Aurora SDK: <a href="https://developer.auroraos.ru/doc/sdk/app_development/work/create/examples#build">Documentation</a></p>
<p><a name="Branch-Information"></a></p>
<h2>Branch Information</h2>
<p><a href="https://developer.auroraos.ru/doc/software_development/examples#branches">Branches</a></p>
<p><a name="Installation-and-Launch"></a></p>
<h2>Installation and Launch</h2>
<p>The project is built in the standard manner using the Aurora SDK: <a href="https://developer.auroraos.ru/doc/sdk/app_development/work/build">Documentation</a>.</p>
<p><a name="Screenshots"></a></p>
<h2>Screenshots</h2>
<p><image-card alt="screenshots" src="screenshots/screenshots.png"></image-card></p>
<p><a name="Project-Structure"></a></p>
<h2>Project Structure</h2>
<p>The project follows a standard application structure for C++ and QML on the Aurora OS.</p>
<ul>
<li>File <strong><a href="ru.auroraos.BleScanner.pro">ru.auroraos.BleScanner.pro</a></strong> describes a subdirs project containing two subprojects: one for building the SimpleBLE library and the other for defining the application structure for the qmake build system.</li>
<li>Directory <strong><a href="app/icons">icons</a></strong> contains application icons for various screen resolutions.</li>
<li>Directory <strong><a href="app/qml">qml</a></strong> contains QML source code and user interface resources.
<ul>
<li>Directory <strong><a href="app/qml/cover">cover</a></strong> includes application cover implementations.</li>
<li>Directory <strong><a href="app/qml/icons">icons</a></strong> contains additional user interface icons.</li>
<li>Directory <strong><a href="app/qml/pages">pages</a></strong> includes application pages.</li>
<li>File <strong><a href="app/qml/BleScanner.qml">BleScanner.qml</a></strong> provides the application window implementation.</li>
</ul>
</li>
<li>Directory <strong><a href="rpm">rpm</a></strong> contains rpm package build settings. File <strong><a href="rpm/ru.auroraos.BleScanner.spec">ru.auroraos.BleScanner.spec</a></strong> is used by the rpmbuild tool.</li>
<li>Directory <strong><a href="app/src">src</a></strong> contains C++ source code.
<ul>
<li>File <strong><a href="app/src/main.cpp">main.cpp</a></strong> serves as the application entry point.</li>
</ul>
</li>
<li>Directory <strong><a href="app/translations">translations</a></strong> contains user interface translation files.</li>
<li>File <strong><a href="app/ru.auroraos.BleScanner.desktop">ru.auroraos.BleScanner.desktop</a></strong> defines the application’s display and launch parameters.</li>
</ul>
<p><a name="Usage-Rules-and-Development-Participation"></a></p>
<h2>Usage Rules and Development Participation</h2>
<p>The project’s source code is provided under the <a href="LICENSE.BSD-3-CLAUSE.md">license</a>, which permits its use in third-party applications.</p>
<p><a href="https://gitlab.com/omprussia/wiki/-/wikis/CLA.ru">Contributor License Agreement</a> <a href="CONTRIBUTING.md">Contributor Agreement</a> governs the rights granted by contributors of Open Mobile Platform LLC.</p>
<p>Information about contributors is listed in the <a href="AUTHORS.md">AUTHORS</a> file.</p>
<p><a href="CODE_OF_CONDUCT.md">Code of Conduct</a> is an active set of rules from Open Mobile Platform LLC, informing community members of expectations for interaction and collaboration on projects.</p>
<p><a name="Project-Enhancements"></a></p>
<h2>Project Enhancements</h2>
<p><a name="Adding-Triangulation-and-Position-Calculation"></a></p>
<h3>Adding Triangulation and Position Calculation</h3>
<p>The goal was to determine the device’s position in space based on signals from four BLE beacons (ESP32_PICO_V3_BLE_1 to ESP32_PICO_V3_BLE_4). Triangulation was implemented using classical trilateration with three beacons to calculate (x, y) coordinates in meters through a system of distance equations. An alternative Nearest Neighbor method was added, determining the position as the coordinates of the beacon with the highest RSSI. The distance calculation is handled by the <code>calculateDistance(int rssi)</code> method, based on an empirical signal attenuation model (FSPL) with a starting value of -59 dBm at 1 meter and 2 dB attenuation per doubling of distance. Implementation is in <code>blescanmodel.cpp</code>: <code>triangulatePosition()</code> outputs distances and position to <code>qdebug()</code>, <code>nearestNeighborPosition()</code> selects the nearest beacon, and <code>calculateTriangulation()</code> and <code>calculateNearestNeighbor()</code> update <code>m_position</code>. These methods are declared in <code>blescanmodel.h</code> with <code>Q_INVOKABLE</code> for QML access.</p>
<p><a name="Updating-Beacon-Coordinates"></a></p>
<h3>Updating Beacon Coordinates</h3>
<p>The aim was to adapt the system to new beacon coordinates, measured from the bottom-left corner of the map, where each cell equals 10 cm. Coordinates were updated based on data: Z1 (1.3, 9.0), Z2 (11.2, 6.9), Z3 (11.3, 2.7), Z4 (5.6, 1.6) meters. Changes are implemented in <code>triangulatePosition()</code> and <code>nearestNeighborPosition()</code> in <code>blescanmodel.cpp</code>, where coordinates are stored in <code>QMap&#x3C;QString, QPointF> beaconPositions</code>.</p>
<p><a name="Displaying-Position-on-Map"></a></p>
<h3>Displaying Position on Map</h3>
<p>The goal was to display the calculated position as a blue dot on the map (<code>MapPage.qml</code>) with redrawing on button press. The <code>positionInPixels()</code> method converts meters to map pixels (114×92 pixels, corresponding to 11.4×9.2 meters), implemented in <code>blescanmodel.cpp</code>. Visualization is achieved via a <code>Rectangle</code> in <code>MapPage.qml</code> with a "blue" color and 20×20 size. The <code>position</code> property with a <code>positionChanged</code> signal was added for QML updates.</p>
<p><a name="Synchronization-of-blescanmodel"></a></p>
<h3>Synchronization of bleScanModel</h3>
<p>The goal was to resolve issues with different <code>bleScanModel</code> instances. It is now global, defined in <code>MainPage.qml</code> and passed to <code>MapPage.qml</code> via the <code>bleModel</code> parameter. Local creation in <code>MapPage.qml</code> was removed. Implementation: the "Map View" button in <code>MainPage.qml</code> uses <code>pageStack.push(..., {bleModel: bleScanModel})</code>, while <code>MapPage.qml</code> accepts <code>property var bleModel: null</code> with a check.</p>
<p><a name="Diagnostics-and-Debugging"></a></p>
<h3>Diagnostics and Debugging</h3>
<p>The goal was to enhance tracing for error resolution. <code>qDebug()</code> statements were added in <code>positionInPixels()</code>, <code>triangulatePosition()</code>, and <code>updateDiagnosticMessage()</code> in <code>blescanmodel.cpp</code>.</p>
<p><a name="Error-Handling-and-Stability"></a></p>
<h3>Error Handling and Stability</h3>
<p>The goal was to eliminate incorrect trilateration values (e.g., 175, 151 m). Validity checks for distances and result limits (0–11.4 m, 0–9.2 m) were added in <code>triangulatePosition()</code> in <code>blescanmodel.cpp</code> using <code>qBound()</code>. Additionally, the ability to mark "dead zones" on the map was introduced. In the CSV file defining the room map, the symbol "B" designates areas with dead zones.</p>
<p><a name="The-Beacons"></a></p>
<h3>The Beacons</h3>
<p>Beacons are implemented on ESP32-pico-v3 with a transmission power of 9 dBm in the firmware. The choice of ESP32 is due to its availability, BLE support, and programmability for transmitting identifiers (e.g., "ESP32_PICO_V3_BLE_1" to "ESP32_PICO_V3_BLE_4"). Interaction with beacons is handled via <code>updateDeviceInfo()</code> in <code>blescanmodel.cpp</code>. For use in larger areas, ESP32 models with an IPEX connector for a 2.4 GHz antenna are recommended to extend coverage, reducing the number of beacons needed. Note! Using a different solution requires recalculating TX power and other parameters to account for signal attenuation differences, with formulas provided in the documentation.</p>
<p><a name="The-Map"></a></p>
<h3>The Map</h3>
<p>The map is represented by <code>kartasmall.png</code> in <code>MapPage.qml</code>, sized at 114×92 pixels (11.4×9.2 meters, each cell 10 cm). Markers include: "B" for restricted zones, "X" for empty spaces, and "Z1"–"Z4" for beacons, displayed as green dots via <code>Grid</code> and <code>Repeater</code> with data from the <code>gridData</code> model in <code>BleScanModel</code>. Markers are stored in a CSV file, currently located in the <code>qml/icons</code> directory. Commented code for downloading the room map from a server exists, intended for real-world navigation where clients download the required map. A server implementation might use another device or modify an existing beacon’s firmware, though the latter is discouraged due to potential Wi-Fi/BLE interference affecting signal stability.</p>
<h3>To-do</h3>
<p>The project remains active with opportunities for further development. As of this documentation, preparations exist for RSSI fingerprinting and IMU Fusion using an accelerometer and other sensors, with a Kalman filter for position estimation using BLE RSSI and additional data.</p>
<ul>
<li>An urgent issue persists with rendering the client’s position.</li>
<li>Code for server communication to download the room map is currently commented out.</li>
<li>Code for server communication to send the current position is also commented out.</li>
<li>The server version with 3D room visualization has errors in Wi-Fi RSSI calculation (potential for detecting moved/damaged beacons), battery level (currently via ADC, with a voltage divider as an experimental solution), and plans for a dedicated battery module. A 3D model file for the beacon case supports up to a 3S-18650 battery (3.7V), three LEDs for error/charge indication, and a kill-switch.</li>
</ul>
<p>Fork Author: Arseniy Kostin (@arskstn)<br>
Previous Firmware Author: Maxim Simdyanov (@NotoriusM)<br>
Original Project Author: Open Mobile Platform LLC, original documentation is in the application directory.</p>
</document>
