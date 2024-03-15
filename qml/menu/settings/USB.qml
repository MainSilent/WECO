import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import ".."

Item {
    Back {}

    ScrollView {
        anchors {
            fill: parent
            top: parent.top
            bottom: parent.bottom
            topMargin: 50
            bottomMargin: 20
        }
        clip: true

        ColumnLayout {
            x: root.width / 6
            spacing: 12

            Repeater {
                model: settingsBackend.usbDevices
                delegate: device
            }
        }
    }

    Component {
        id: device

        Rectangle {
            width: 450
            height: 40
            color: modelData.connected ? "#aaffffff" :  "transparent"
            border.color: "#aaffffff"
            radius: 3
            clip: true
            Layout.alignment: Qt.AlignHCenter

            Text {
                id: usbName
                width: 430
                text: modelData.name
                color: modelData.connected ? "black" : "white"
                font.pixelSize: 16
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 10
               }
               clip: true
            }

            MouseArea {
                id: mouseUSB
                anchors.fill: parent
                onClicked: settingsBackend.setDevice(modelData.vendor, modelData.product)
                cursorShape: Qt.PointingHandCursor
            }
        }
    }
}
