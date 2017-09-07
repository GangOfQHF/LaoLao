import QtQuick 2.0
import QtQuick.Controls.Styles 1.4

ButtonStyle{
    property int buttonRadius: 3
    property int buttonImplicitWidth: 60
    property int buttonImplicitHeight: 25
    property string buttonText: ""
    property string buttonTextColor: "white"
    property int buttonTextSize: 9
    property string hoveredColor: "#3CC3F5"
    property string exitedColor: "#09A3DC"
    property string borderColor: "#004B77"
    property variant button
    background: Rectangle {
        id: backRec
        implicitWidth: buttonImplicitWidth
        implicitHeight: buttonImplicitHeight
        radius: buttonRadius
        color: button.hovered || button.focus ? hoveredColor : exitedColor
        border.width: button.hovered || button.focus ? 1 : 0
        border.color: borderColor
        MouseArea {
            onPressed: {
                backRec.color =  borderColor;
            }
            onReleased: {
                backRec.color = button.hovered ? hoveredColor : exitedColor;
            }
        }
        Text {
            anchors.centerIn: parent
            text: qsTr(buttonText)
            //            font.family: SystemFont
            font.pointSize: buttonTextSize
            color: buttonTextColor
        }
    }
}
