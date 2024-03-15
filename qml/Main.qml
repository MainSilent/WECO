import QtQuick 2.12
import QtQuick.Window 2.12
import "startup"
import "general"
import "fonts"

Window {
    id: root
    width: 680
    height: 480
    visible: true
    title: "ECO"
    color: "transparent"
    flags: Qt.FramelessWindowHint
    // Don't let the window to resize
    minimumHeight: height
    minimumWidth: width
    maximumHeight: height
    maximumWidth: width

    Rectangle {
        property bool serverConnected: false
        property string powerStatus: ""
        property string mode: "startup"
        property string path: ""

        id: main
        anchors.fill: parent
        color: "#ee000000"
        radius: 5.5

        Column {
            anchors.fill: parent

            Toolbar {
                width: parent.width
                height: 45
            }

            Startup {
                id: startup
                width: parent.width
                height: parent.height - 45
                visible: main.mode === "startup"
            }

            Menu {
                id: menu
                width: parent.width
                height: parent.height - 45
                visible: main.mode === "menu"
            }

            Loader {
               width: parent.width
               height: parent.height - 45
               source: main.mode == "path" ? main.path : ""
               visible: main.mode === "path"
            }

            Error {
                id: error
                width: parent.width
                height: parent.height - 45
                visible: main.mode === "error"
            }

            Loading {
                id: loading
                width: parent.width
                height: parent.height - 45
                animationType: "progress"
                visible: main.mode === "loading"
            }
        }
    }

    Connections {
        target: powerBackend
        onStatusChanged: main.powerStatus = powerBackend.status
        Component.onCompleted: main.powerStatus = powerBackend.status
    }

    Connections {
        target: serverBackend
        onConnectedChanged: main.serverConnected = serverBackend.connected
    }

    FontAwesome {}
}
