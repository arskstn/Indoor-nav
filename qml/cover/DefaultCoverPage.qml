import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    objectName: "defaultCover"

    CoverTemplate {
        objectName: "applicationCover"
        primaryText: "App"
        secondaryText: qsTr("Indoor Nav")
        icon {
            source: Qt.resolvedUrl("../icons/IndoorNav.svg")
            sourceSize { width: icon.width; height: icon.height }
        }
    }
}
