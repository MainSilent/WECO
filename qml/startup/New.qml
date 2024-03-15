import QtQuick 2.12
import Qt.labs.platform 1.0

Rectangle {
    property var spec: startupBackend.spec()
    property var inputData: {'CPU':2, 'RAM': 4096, 'Size': 256}
    signal inputChanged

    Text {
        text: "Create your first Windows ECOSystem"
        font.pixelSize: 22
        font.weight: "Light"
        color: "white"
    }

    Item {
        anchors {
            fill: parent
            topMargin: 50
            bottomMargin: 60
            leftMargin: 12
            rightMargin: 12
        }

        // CPU
        Text {
            text: "CPU"
            width: 90
            height: 20
            font.pixelSize: 20
            font.weight: "Light"
            color: "white"

            TextInput {
                text: "2"
                width: parent.width - 45
                height: parent.height
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                font.pixelSize: 18
                horizontalAlignment: TextInput.AlignHCenter
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1; top: spec['CPU'] }
                color: "white"
                clip: true

                onFocusChanged: {
                    if (text < 1)
                        text = 1
                    else if (text > spec['CPU'])
                        text = spec['CPU']

                    inputData['CPU'] = parseInt(text)
                    inputChanged()
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    anchors.bottom: parent.bottom
                }
            }
        }

        // RAM
        Text {
            text: "RAM"
            width: 150
            height: 20
            anchors {
                top: parent.top
                topMargin: 45
            }
            font.pixelSize: 20
            font.weight: "Light"
            color: "white"

            TextInput {
                text: "4096"
                width: parent.width - 50
                height: parent.height
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                font.pixelSize: 18
                horizontalAlignment: TextInput.AlignHCenter
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1024; top: spec['RAM'] }
                color: "white"
                clip: true

                onFocusChanged: {
                    if (text < 1024)
                        text = 1024
                    else if (text > spec['RAM'])
                        text = spec['RAM']

                    inputData['RAM'] = parseInt(text)
                    inputChanged()
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

        // Storage Size
        Text {
            text: "Storage Size"
            width: 220
            height: 20
            anchors {
                top: parent.top
                topMargin: 90
            }
            font.pixelSize: 20
            font.weight: "Light"
            color: "white"

            TextInput {
                text: "256"
                width: parent.width - 115
                height: parent.height
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                font.pixelSize: 18
                horizontalAlignment: TextInput.AlignHCenter
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 20; top: 1000000 }
                color: "white"
                clip: true

                onFocusChanged: {
                    if (text <= 20)
                        text = 20
                    else if (text > 1000000)
                        text = 1000000

                    inputData['Size'] = parseInt(text)
                    inputChanged()
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    anchors.bottom: parent.bottom
                }
            }

            Text {
                text: "GB"
                anchors {
                    right: parent.right
                    rightMargin: -32
                }
                font.pixelSize: 20
                font.weight: "Light"
                color: "white"
            }
        }

        // Storage Path
        Text {
            text: "Storage Path"
            width: parent.width
            height: 20
            anchors {
                top: parent.top
                topMargin: 140
            }
            font.pixelSize: 20
            font.weight: "Light"
            color: "white"

            TextInput {
                id: diskImageInput
                width: parent.width - 140
                height: parent.height
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                font.pixelSize: 18
                color: "white"
                clip: true

                Rectangle {
                    width: parent.width
                    height: 1
                    anchors.bottom: parent.bottom
                }

                FolderDialog {
                    id: diskImagePath
                    onAccepted: {
                        var foldername = folder.toString().substring(7)+"/WindowsECO.qcow2"
                        diskImageInput.text = foldername
                        inputData['Path'] = foldername
                        inputChanged()
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: diskImagePath.open()
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }

        // Driver ISO
        Text {
            text: "Driver ISO"
            width: parent.width
            height: 20
            anchors {
                top: parent.top
                topMargin: 190
            }
            font.pixelSize: 20
            font.weight: "Light"
            color: "white"

            TextInput {
                id: driverISOInput
                width: parent.width - 122
                height: parent.height
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                font.pixelSize: 18
                color: "white"
                clip: true

                Rectangle {
                    width: parent.width
                    height: 1
                    anchors.bottom: parent.bottom
                }

                FileDialog {
                    id: driverISOFile
                    nameFilters: "ISO files (*.iso *.ISO *.ISo *.IsO *.iSO *.isO *.iSo *.Iso)"
                    onAccepted: {
                        var filename = file.toString().substring(7)
                        driverISOInput.text = filename
                        inputData['DriverISO'] = filename
                        inputChanged()
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: driverISOFile.open()
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }

        // Windows ISO
        Text {
            text: "Windows ISO"
            width: parent.width
            height: 20
            anchors {
                top: parent.top
                topMargin: 240
            }
            font.pixelSize: 20
            font.weight: "Light"
            color: "white"

            TextInput {
                id: windowsISOInput
                width: parent.width - 140
                height: parent.height
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                font.pixelSize: 18
                color: "white"
                clip: true

                Rectangle {
                    width: parent.width
                    height: 1
                    anchors.bottom: parent.bottom
                }

                FileDialog {
                    id: windowsISOFile
                    nameFilters: "ISO files (*.iso *.ISO *.ISo *.IsO *.iSO *.isO *.iSo *.Iso)"
                    onAccepted: {
                        var filename = file.toString().substring(7)
                        windowsISOInput.text = filename
                        inputData['WindowsISO'] = filename
                        inputChanged()
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: windowsISOFile.open()
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }
    }

    // Create button
    Rectangle {
        id: createBtn
        width: createBtnText.implicitWidth + 20
        height: createBtnText.implicitHeight + 20
        anchors {
            bottom: parent.bottom
            bottomMargin: 15
            horizontalCenter: parent.horizontalCenter
        }
        color: createBtnMouse.containsMouse ? "#bb898989" : "#898989"
        radius: 4
        visible: false

        Text {
            id: createBtnText
            text: !creating ? "Create" : "Please Wait..."
            anchors.centerIn: parent
            color: "black"
        }

        MouseArea {
            id: createBtnMouse
            anchors.fill: parent
            hoverEnabled: true
            onEntered: focus = true
            onClicked: {
                if (!creating) {
                    startupBackend.startNewMachine(inputData)
                    creating = true
                }
            }
            cursorShape: Qt.PointingHandCursor
        }
    }

    onInputChanged: if (Object.keys(inputData).length === 5) createBtn.visible = true

    Connections {
        target: startupBackend
        onCreated: main.mode = "menu"
    }
}
