import QtQuick 2.12

Rectangle {
    property int percent: 0
    property string title: ""
    property string detail: ""
    property string animationType: "blink"

    color: "transparent"

    Image {
        id: mainLoadingImage
        width: 450
        height: 10
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 100
        }
        //opacity: animationType === "progress" ? 0.2 : 1
        opacity: animationType === "progress" ? 0.5 : 1
        source: "qrc:/qml/images/sharpLoader.svg"

        // Blink Animation
        NumberAnimation on opacity {
            id: loadingOpacityAnim1
            from: 1
            to: 0.2
            duration: 2000
            running: animationType === "blink"
            onStopped: loadingOpacityAnim2.start()
        }

        NumberAnimation on opacity {
            id: loadingOpacityAnim2
            from: 0.2
            to: 1
            duration: 2000
            running: animationType === "blink"
            onStopped: loadingOpacityAnim1.start()
        }
    }

    // Blink Title
    Text {
        text: title
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: mainLoadingImage.top
            topMargin: -24
        }
        color: "white"
        visible: animationType === "blink"
    }

    // Progress Image
    Image {
        width: 450
        height: 10
        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 100
        }
        source: "qrc:/qml/images/sharpLoader.svg"
        //visible: animationType === "progress"
        visible: false
    }

    // Percent Text
    Text {
        text: percent + "%"
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: mainLoadingImage.top
            topMargin: -24
        }
        color: "white"
        visible: animationType === "progress"
    }

    // Detail Text
    Text {
        text: detail
        width: 412
        anchors {
            top: mainLoadingImage.top
            left: mainLoadingImage.left
            leftMargin: 30
            topMargin: 20
        }
        color: "white"
        clip: true
        visible: animationType === "progress"
    }
}
