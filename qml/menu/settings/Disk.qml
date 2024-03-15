import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import Qt.labs.platform 1.0
import ".."

Item {
    Back {}
    Add {
        onAddClicked: diskPath.open()
    }

    FileDialog {
        id: diskPath
        onAccepted: settingsBackend.addDisk(file)
    }

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
                model: settingsBackend.diskDevices
                delegate: device
            }
        }
    }

    Component {
        id: device

        Rectangle {
            width: 450
            height: 40
            color: "transparent"
            border.color: "#aaffffff"
            radius: 3
            clip: true
            Layout.alignment: Qt.AlignHCenter

            Text {
                id: usbName
                width: 400
                text: modelData.split('/')[modelData.split('/').length - 1]
                color: "white"
                font.pixelSize: 16
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 10
               }
               clip: true
            }

            Text {
                text: "X"
                color: mouseDiskDelete.containsMouse ? "red" : "white"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 10
                visible: modelData.split('/')[modelData.split('/').length - 1] !== "WindowsECO.qcow2"

                MouseArea {
                    id: mouseDiskDelete
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: settingsBackend.deleteDisk(modelData)
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }
    }
}
