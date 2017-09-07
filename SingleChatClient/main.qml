import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

Window {
    width: 240
    height: 550
    visible: false
    Component.onCompleted: {
        logOnLoader.setSource("logOn.qml")
    }

    Loader {
        id: logOnLoader
        smooth: true
    }
}

