import QtQuick 2.12

Item {
    readonly property FontLoader fontAwesomeRegular: FontLoader {
        source: "./fa-regular-400.ttf"
    }

    readonly property string regular: fontAwesomeRegular.name
}
