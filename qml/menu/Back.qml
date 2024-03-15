import QtQuick 2.12

Rectangle {
    width: 40
    height: 30
    color: !mouseBack.containsMouse ? "transparent" : "#44ffffff"
    radius: 3
    anchors {
        top: parent.top
        left: parent.left
        topMargin: 10
        leftMargin: 10
    }

    Text {
        text: "←"
        color: "#ccffffff"
        font.pixelSize: 35
        anchors.centerIn: parent
    }

    MouseArea {
        id: mouseBack
        anchors.fill: parent
        onClicked: main.mode = "menu"
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
    }
}
