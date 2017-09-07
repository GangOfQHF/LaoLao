import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Window {
    id: createGroup
    width: 360
    height: 200
    visible: true

    title: qsTr("创建分组")
    x: 600
    y: 200
    color: "#ccffff"

    Text {
        id: title
        text: "创建分组"
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        font.family: "黑体"
        font.pointSize: 56
        style: Text.Raised
        color: "#666666"
    }

    //布局
    Row {
        id: grid
        anchors.top: title.top
        anchors.topMargin: 60
        anchors.left: parent.left
        anchors.leftMargin: 50
        spacing: 13

        Text {
            id: name
            text: qsTr("组名")
            font.bold: true
            font.pointSize: 18
            color: "#006633"
        }
        TextField {
            id: nameField
            width: 230
            height: 30
            placeholderText: "输入分组名，如：好友"
            maximumLength: 15
        }
    }

    Button {
        text: qsTr("确定")
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
                    GradientStop {
                        position: 0
                        color: control.pressed ? "66ffcc#" : "#ccffff"
                    }
                    GradientStop {
                        position: 1
                        color: control.pressed ? "#00ffff" : "#66ffcc"
                    }
                }
            }
        }
        onClicked: {
            //判断创建的分组是否已经存在
            if(!friendList.findGroup(nameField.text) && nameField){
                singlechat.createGroup(nameField.text)
                //设置组的名字，并且设置好友的属性为空
                friendList.loadDataToModel(nameField.text, "", "", "", "")
                createGroup.visible = false
            } else if(friendList.findGroup(nameField.text)){
                errorDialog.text = "好友分组已经存在"
                nameField.text = ""
                errorDialog.visible = true
            } else {
                errorDialog.text = "好友分组名不能为空"
                errorDialog.visible = true
            }
        }
    }

    MessageDialog {
        id:errorDialog
        visible: false
        title: "ERRO Box";
        text: "";
    }
}
