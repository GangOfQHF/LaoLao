import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Window{
    id:modifyRemarks
    width:360
    height:200
    visible: true

    title: qsTr("修改好友备注")
    x:400
    y:200
    color: "#ccffff"


    Text{
        id:title
        text:"修改好友备注"
        anchors.top:parent.top
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        font.family: "黑体"
        font.pointSize: 56
        style:Text.Raised
        color: "#666666"
    }

    //布局
    Row{
        id:grid
        anchors.top:title.top
        anchors.topMargin: 60
        anchors.left: parent.left
        anchors.leftMargin: 50
        spacing: 13

        Text{
            id:name
            text:qsTr("备注")
            font.bold: true
            font.pointSize: 18
            color: "#006633"
        }
        TextField{
            id:nameField
            width:230
            height: 30
            placeholderText: "输入新备注，如：小明"
            maximumLength: 15

        }


    }

    Button{
        text:qsTr("确定")
        isDefault: true
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        style: ButtonStyle {
            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 25
                border.width: control.activeFocus ? 2 : 1
                border.color: "#888"
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "66ffcc#" : "#ccffff" }
                    GradientStop { position: 1 ; color: control.pressed ? "#00ffff" : "#66ffcc" }
                }
            }
        }
        onClicked: {
            singlechat.modifyRemarks(msgHandler());
            model.remarksName = nameField.text
            modifyRemarks.visible = false
        }
    }

    function msgHandler()
    {
        var s = model.account + "^" + nameField.text
        return s
    }



}
