// SPDX-FileCopyrightText: Copyright 2025 Open Mobile Platform LLC <community@omp.ru>
// SPDX-License-Identifier: BSD-3-Clause

import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.omp.SimpleBLE 1.0

Page {
    id: page
    allowedOrientations: Orientation.All

    PageHeader {
        id: pageHeader
        title: qsTr("Map View")
    }

    Column {
        anchors {
            top: pageHeader.bottom
            left: parent.left
            right: parent.right
            margins: Theme.paddingLarge
        }
        spacing: Theme.paddingMedium

        Item {
            width: parent.width
            height: Theme.itemSizeExtraLarge * 12

            Image {
                id: mapImage
                width: parent.width
                height: parent.height
                source: "qrc:/icons/kartasmall.png"
                fillMode: Image.Stretch
            }

            Grid {
                id: grid
                anchors.fill: mapImage
                columns: 114
                rows: 92
                spacing: 0

                Repeater {
                    model: bleScanModel.gridData
                    delegate: Repeater {
                        model: modelData.split(",")
                        delegate: Rectangle {
                            width: mapImage.width / 114
                            height: mapImage.height / 92
                            color: {
                                if (modelData === "Z1" || modelData === "Z2" || modelData === "Z3" || modelData === "Z4") {
                                    return "green"
                                } else if (modelData === "B") {
                                    return "red"
                                } else {
                                    return "transparent"
                                }
                            }
                            opacity: modelData === "X" ? 0 : 0.7
                        }
                    }
                }
            }

            // Индикатор позиции (синяя точка 20x20)
            Rectangle {
                id: positionIndicator
                width: 20
                height: 20
                radius: 10
                color: "blue"
                x: (bleScanModel.positionInPixels().x * mapImage.width / 114) - width / 2
                y: mapImage.height - ((bleScanModel.positionInPixels().y * mapImage.height / 92) + height / 2)
                visible: bleScanModel.position.x >= 0 && bleScanModel.position.y >= 0
                z: 1
            }
        }
    }

    BleScanModel {
        id: bleScanModel
        onPositionChanged: {
            console.log("New position:", position.x, position.y)
            positionIndicator.visible = position.x >= 0 && position.y >= 0 // Обновляем видимость
        }
    }
}
