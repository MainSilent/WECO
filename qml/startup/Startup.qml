import QtQuick 2.12

Rectangle {
    property bool creating: false
    property var errorMessage: startupBackend.required()
    signal installPrograms

    color: "transparent"

    // Check Error
    Component.onCompleted: {

        if (errorMessage.length === 0)
            main.mode = "menu"
        else {
            // Virsh install
            if (errorMessage[0] === "virsh") {
                error.title = "Required programs are not installed"
                error.body = errorMessage[1]
                error.textBtn1 = "Install"
                main.mode = "error"
            }
            else if (errorMessage[0] !== "vm.xml not found") {
                error.title = errorMessage[0]
                error.body = errorMessage[1]
                main.mode = "error"
            }
        }
    }

    // Install Button
    onInstallPrograms: {
        loading.title = "Installing, Please wait..."
        loading.animationType = "blink"
        main.mode = "loading"
        startupBackend.startInstall()
    }

    Connections {
        target: startupBackend
        onResultChanged: {
            if (startupBackend.result[0] === "Error:") {
                error.title = startupBackend.result[0]
                error.body = startupBackend.result[1]
                error.textBtn1 = "Retry"
                main.mode = "error"
            }
            else if (startupBackend.result[0] === "Qemu Error:") {
                error.reset()
                error.title = startupBackend.result[0]
                error.body = startupBackend.result[1]
                error.textBtn1 = "Retry"
                main.mode = "error"
            }
            else if (startupBackend.result[0] === "100" && errorMessage[2] !== "vm.xml not found")
                main.mode = "menu"
            else if (startupBackend.result[0] === "100" && errorMessage[2] === "vm.xml not found")
                main.mode = "startup"
        }
    }

    New {
        id: newVM
        anchors {
            fill: parent
            topMargin: 22
            leftMargin: 15
            rightMargin: 15
        }
        color: "transparent"
    }
}
