import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Rectangle {
    property string title: "Error!"
    property string body: "Something Went Wrong."
    property string textBtn1: ""
    property string textBtn2: ""
    signal btn1Clicked
    signal btn2Clicked

    color: "transparent"
    radius: 4
    clip: true

    // Title
    Text {
        text: title
        color: "white"
        anchors {
            top: parent.top
            left: parent.left
            topMargin: 20
            leftMargin: 20
        }
        font.pixelSize: 20
    }

    // Body
    ScrollView {
        anchors {
            fill: parent
            topMargin: 40
            bottomMargin: (textBtn1 || textBtn2) ? 60 : 20
            leftMargin: 20
            rightMargin: 15
        }

        TextArea {
            text: body
            color: "white"
            font.pixelSize: 17
            selectionColor: "#bbffffff"
            selectedTextColor: "black"
            readOnly: true
            wrapMode: Text.WordWrap
            selectByMouse: true
        }
    }

    // Buttons
    GridLayout {
        anchors {
            bottom: parent.bottom
            bottomMargin: 15
            horizontalCenter: parent.horizontalCenter
        }

        // Button 1
        Rectangle {
            width: btn1.implicitWidth + 20
            height: btn1.implicitHeight + 20
            color: btn1Mouse.containsMouse ? "#bb898989" : "#898989"
            radius: 4
            visible: textBtn1 ? true : false

            Text {
                id: btn1
                text: textBtn1
                anchors.centerIn: parent
                color: "black"
            }

            MouseArea {
                id: btn1Mouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: btn1Clicked()
                cursorShape: Qt.PointingHandCursor
            }
        }

        // Button 2
        Rectangle {
            width: btn2.implicitWidth + 20
            height: btn2.implicitHeight + 20
            color: btn2Mouse.containsMouse ? "#bb898989" : "#898989"
            radius: 4
            visible: textBtn2 ? true : false

            Text {
                id: btn2
                text: textBtn2
                anchors.centerIn: parent
                color: "black"
            }

            MouseArea {
                id: btn2Mouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: btn2Clicked()
                cursorShape: Qt.PointingHandCursor
            }
        }
    }

    // Buttons onClicked
    onBtn1Clicked: {
        if (title === "Required programs are not installed" ||
           (title === "Error:" && startupBackend.required().length !== 0))
            startup.installPrograms()
        else if (title === "Qemu Error:") {
            startup.creating = false
            main.mode = "startup"
        }
    }

    function reset() {
        title = "Error!"
        body = "Something Went Wrong."
        textBtn1 = ""
        textBtn2 = ""
    }
}
