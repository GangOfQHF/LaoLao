import QtQuick 2.0
import QtQuick.Controls.Styles 1.4

TextFieldStyle {
    textColor: "#000000"
    property string bgColor: "#FFFFFF"
    property string borderColor: "#D1D1D1"
    property int borderWidth: 1
    background: Rectangle {
        radius: 3
        color: bgColor
        border.color: borderColor
        border.width: borderWidth
    }
}
