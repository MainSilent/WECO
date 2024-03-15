import QtQuick 2.12
import ".."

Item {
    property int maxRAM: startupBackend.spec()['RAM']

    Back {}

    Text {
        text: "RAM"
        width: 150
        height: 20
        anchors.centerIn: parent
        font.pixelSize: 20
        font.weight: "Light"
        color: "white"

        TextInput {
            text: settingsBackend.getRAM()
            width: parent.width - 50
            height: parent.height
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            font.pixelSize: 18
            horizontalAlignment: TextInput.AlignHCenter
            inputMethodHints: Qt.ImhDigitsOnly
            validator: IntValidator { bottom: 1024; top: maxRAM }
            color: "white"
            clip: true

            onTextChanged: {
                if (text > maxRAM)
                    text = maxRAM

                if (text >= 1024)
                    settingsBackend.setRAM(text)
            }

            Rectangle {
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
            }
        }

        Text {
            text: "MiB"
            anchors {
                right: parent.right
                rightMargin: -41
            }
            font.pixelSize: 20
            font.weight: "Light"
            color: "white"
        }
    }
}
