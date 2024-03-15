import QtQuick 2.12

Rectangle {
    signal addClicked()

    width: 35
    height: 35
    color: !mouseBack.containsMouse ? "transparent" : "#44ffffff"
    radius: 3
    anchors {
        top: parent.top
        right: parent.right
        topMargin: 10
        rightMargin: 10
    }

    Text {
        text: "+"
        color: "#ccffffff"
        font.pixelSize: 35
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: -5
        anchors.leftMargin: 7
    }

    MouseArea {
        id: mouseBack
        anchors.fill: parent
        onClicked: addClicked()
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
    }
}
