import QtQuick 2.12
import ".."

Item {
    property int maxCPU: startupBackend.spec()['CPU']

    Back {}

    Text {
        text: "CPU"
        width: 90
        height: 20
        anchors.centerIn: parent
        font.pixelSize: 20
        font.weight: "Light"
        color: "white"

        TextInput {
            text: settingsBackend.getCPU()
            width: parent.width - 45
            height: parent.height
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            font.pixelSize: 18
            horizontalAlignment: TextInput.AlignHCenter
            inputMethodHints: Qt.ImhDigitsOnly
            validator: IntValidator { bottom: 1; top: maxCPU }
            color: "white"
            clip: true

            onTextChanged: {
                if (!text)
                    text = ''
                else if (text < 1)
                    text = 1
                else if (text > maxCPU)
                    text = maxCPU

                if (text)
                    settingsBackend.setCPU(parseInt(text))
            }

            Rectangle {
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
            }
        }
    }
}
