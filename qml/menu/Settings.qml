import QtQuick 2.12
import QtQuick.Layouts 1.12

GridLayout {
    property int opacityAnimationToggle: -1
    property string direction: "right"

    // The reason I set `id` instead of using `index` is for the opacity animation

    // Right Settings List
    property var settingsListRight: [
        {
            Id: 4,
            name: "USB",
            name: "USB, N/A",
            icon: "\uf8e9",
            type: "settings",
            //active: main.powerStatus != "off" ? true : false
            active: false
        },
        {
            Id: 3,
            //name: "Disk",
            name: "Disk, N/A",
            icon: "\uf0a0",
            type: "settings",
            active: false
        },
        {
            Id: 2,
            name: "RAM, N/A",
            icon: "\uf538",
            type: "settings",
            active: false
            //active: main.powerStatus == "off" ? true : false
        },
        {
            Id: 1,
            name: "CPU, N/A",
            icon: "\uf2db",
            type: "settings",
            active: false
            //active: main.powerStatus == "off" ? true : false
        },
        {
            Id: 0,
            name: "< Settings",
            icon: "\uf013",
            type: "settings",
            active: true
        }
    ]

    // Left Settings List
    property var settingsListLeft: [
        {
            Id: 0,
            name: "Settings >",
            icon: "\uf013",
            type: "settings",
            active: true
        },
        {
            Id: 1,
            //name: "Network",
            name: "Network, N/A",
            icon: "\uf6ff",
            type: "settings",
            active: false
        },
        {
            Id: 2,
            //name: "PCI",
            name: "PCI, N/A",
            icon: "\uf550",
            type: "settings",
            active: false
        },
        {
            Id: 3,
            //name: "Filesystem",
            name: "Filesystem, N/A",
            icon: "\uf07b",
            type: "settings",
            active: false
        },
        {
            Id: 4,
            name: "All",
            icon: "\uf57d",
            type: "settings",
            active: true
        }
    ]

    // Settings Items
    Repeater {
        model: direction === "right" ? settingsListRight : settingsListLeft

        MenuItem {
            type: "settings"
            opacityToggle: opacityAnimationToggle
        }
    }
}
