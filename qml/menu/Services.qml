import QtQuick 2.12
import QtQuick.Layouts 1.12

GridLayout {
    // Services List
    property var servicesList: [
        {
            name: "Main Display",
            icon: "\uf108",
            type: "services",
            active: powerBackend.status !== "off" && powerBackend.status !== ""
        },
        {
            name: "Windows List",
            icon: "\uf2fa",
            type: "services",
            active: main.serverConnected
        },
        {
            name: "Programs",
            icon: "\uf37e",
            type: "services",
            active: main.serverConnected
        },
        {
            //name: "Notifications",
            name: "Notifications, N/A",
            icon: "\uf0f3",
            type: "services",
            //active: true
            active: false
        }
    ]

    // Services Items
    Repeater {
        model: servicesList

        MenuItem {}
    }
}
