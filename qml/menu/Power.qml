import QtQuick 2.12
import QtQuick.Layouts 1.12

GridLayout {
    signal action (string type)

    // Power List
    property var powerList: [
        {
            name: "Power On",
            icon: "\uf04b",
            type: "power",
            active: main.powerStatus !== "running"
        },
        {
            name: "Shutdown",
            icon: "\uf011",
            type: "power",
            active: main.powerStatus === "running"
        },
        {
            name: "Restart",
            icon: "\uf01e",
            type: "power",
            active: main.powerStatus === "running"
        },
        {
            name: "Suspend",
            icon: "\uf0c7",
            type: "power",
            active: main.powerStatus === "running"
        }
    ]

    // Power Items
    Repeater {
        model: powerList

        MenuItem {}
    }

    onAction: {
        switch (type) {
            case "Power On":
                if (main.powerStatus === "off")
                    powerBackend.action("start")
                else if (main.powerStatus === "paused")
                    powerBackend.action("resume")
                else
                    powerBackend.action("create")
                break;
            case "Shutdown":
                powerBackend.action("shutdown")
                break;
            case "Restart":
                powerBackend.action("reboot")
                break;
            case "Suspend":
                powerBackend.action("suspend")
        }
    }
}
