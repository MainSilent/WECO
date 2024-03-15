import QtQuick 2.12

Rectangle {
    color: "transparent"

    // Hide Button
    Rectangle {
        width: 48
        height: parent.height
        anchors.right: parent.right
        color: "transparent"

        Text {
            text: "X"
            anchors.centerIn: parent
            font.pixelSize: 17
            color: "white"
        }

        Rectangle {
            width: 1
            height: parent.height - 18
            anchors.verticalCenter: parent.verticalCenter

            // On Mouse Enter
            ColorAnimation on color {
                to: "red"
                duration: 400
                running: hideButtonMouse.containsMouse
            }
            // On Mouse Leave
            ColorAnimation on color {
                to: "#aaffffff"
                duration: 300
                running: !hideButtonMouse.containsMouse
            }
        }

        MouseArea {
            id: hideButtonMouse
            anchors.fill: parent
            onClicked: Qt.quit()
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
        }
    }

    // Title
    Text {
        text: "ECO"
        anchors.centerIn: parent
        font.pixelSize: 20
        font.weight: "Light"
        color: "white"
    }

    // Status Indicator
    Rectangle {
        width: 17
        height: width
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 11
        }
        color: main.serverConnected ? "#aa00ff00"
               : main.powerStatus === "running" ? "#cc0000"
               : main.powerStatus === "paused" ? "#cccc00" : "grey"
        radius: width / .5

        // Status Detail
        Text {
            text: main.serverConnected ? "Connected"
                  : main.powerStatus === "running" ? "Running (ECO Service not Connected)"
                  : main.powerStatus === "paused" ? "Paused" : "Powered Off"
            anchors {
                left: parent.left
                leftMargin: 24
            }
            color: "white"
            visible: statusIndicatorMouse.containsMouse
        }

        MouseArea {
            id: statusIndicatorMouse
            width: 30
            height: width
            hoverEnabled: true
        }
    }

    // Bottom Border
    Rectangle {
        height: .6
        width: parent.width - 20
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        // On Mouse Enter
        ColorAnimation on color {
            to: "red"
            duration: 300
            running: hideButtonMouse.containsMouse
        }
        // On Mouse Leave
        ColorAnimation on color {
            to: "#eeffffff"
            duration: 200
            running: !hideButtonMouse.containsMouse
        }
    }

    // Window Drag Handler
    MouseArea {
        property var clickPos
        anchors.fill: parent
        onPressed: {
            clickPos = { x: mouse.x, y: mouse.y }
        }
        onPositionChanged: {
            root.x = startupBackend.cursorPos().x - clickPos.x
            root.y = startupBackend.cursorPos().y - clickPos.y
        }
        visible: !hideButtonMouse.containsMouse
    }
}
