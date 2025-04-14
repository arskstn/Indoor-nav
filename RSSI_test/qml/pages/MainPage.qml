import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: page.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: "Счётчик нажатий"
            }

            Label {
                id: label
                x: Theme.horizontalPageMargin
                text: "Нажатий: 0"
                color: Theme.highlightColor
                font.pixelSize: Theme.fontSizeLarge

                // Анимация для изменения текста
                Behavior on text {
                    SequentialAnimation {
                        NumberAnimation {
                            target: label
                            property: "opacity"
                            to: 0
                            duration: 200
                        }
                        NumberAnimation {
                            target: label
                            property: "opacity"
                            to: 1
                            duration: 200
                        }
                    }
                }
            }

            Button {
                text: "Клик!"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    counter.value += 1
                    label.text = "Нажатий: " + counter.value
                }
            }

            Button {
                text: "Сброс"
                anchors.horizontalCenter: parent.horizontalCenter
                color: Theme.secondaryColor // Отличный цвет для кнопки сброса
                onClicked: {
                    counter.value = 0
                    label.text = "Нажатий: 0"
                }
            }
        }
    }

    // Счётчик как объект Property
    QtObject {
        id: counter
        property int value: 0
    }
}
