import QtQuick 2.12
import "menu"

Rectangle {
    property bool animating: false

    color: "transparent"

    // 1# Services Row
    Item {
        width: parent.width
        height: parent.height / 3
        anchors.top: parent.top

        Services {
            anchors.fill: parent
            anchors.margins: 10
            columns: 4
        }
    }

    // 2# Settings Row
    Item {
        width: parent.width
        height: parent.height / 3
        anchors.verticalCenter: parent.verticalCenter

        Settings {
            anchors.fill: parent
            anchors.margins: 10
            columns: 5
        }
    }

    // 3# Power Row
    Item {
        width: parent.width
        height: parent.height / 3
        anchors.bottom: parent.bottom

        Power {
            anchors.fill: parent
            anchors.margins: 10
            columns: 4
        }
    }
}
