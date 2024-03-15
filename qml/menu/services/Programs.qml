import QtQuick 2.12
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

        Grid {
            id: programsGrid
            anchors.left: parent.left
            anchors.leftMargin: 10
            columns: 6
            rowSpacing: 12
            columnSpacing: 12

            Repeater {
                model: serverBackend.programs()
                delegate: program
            }
        }
    }

    Component {
        id: program

        Rectangle {
            width: 100
            height: width
            color: programMouse.containsMouse ? "#33ffffff" : "transparent"
            radius: 3
            clip: true

            Image {
                width: parent.width - 40
                height: parent.height - 40
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                source: "file:///home/%1/.local/share/ECO/icons/%2.png".arg(username).arg(modelData["name"])
                mipmap: true
            }

            Text {
                width: 100
                text: modelData["name"]
                color: "white"
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.bottomMargin: 3
                anchors.leftMargin: 3
                font.pixelSize: 13
            }

            MouseArea {
                id: programMouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: serverBackend.execute(modelData["exec"])
            }
        }
    }
}
