import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Window{
    id:findFriend
    width:360
    height:400
    visible: true

    title: qsTr("添加好友")
    x:400
    y:200
    color: "#ccffff"

    property var nickname:" "
    property var selfinfo:" "

    Text{
        id:title
        text:"添加好友"
        anchors.top:parent.top
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        font.family: "黑体"
        font.pointSize: 56
        style:Text.Raised
        color: "#666666"
    }

    //提示框
    MessageDialog {
        id: messageDialog
        title: "提示"
        visible: false;
    }


    //布局
    Row{
        id:row1
        anchors.top:title.top
        anchors.topMargin: 60
        anchors.left: parent.left
        anchors.leftMargin: 20
        spacing: 5

        Text{
            id:name
            text:qsTr("好友帐号")
            font.bold: true
            font.pointSize: 13
            color: "#006633"
        }
        TextField{
            id:accountNum
            width:200
            height: 25
            placeholderText: "输入好友帐号"
            maximumLength: 15

        }
        Button{
            width: 50
            text:qsTr("搜索")
            isDefault: true
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
                grid.visible = false
                button.visible = false
                row2.visible = false
                var i = singlechat.findFriend(accountNum.text)
                if(i==1)
                {
                    messageDialog.visible = true
                    messageDialog.text = "该好友已添加"
                }else if(i==0)
                {
                    var s = singlechat.getUserInfo(accountNum.text)
                    nickname = singlechat.getUserNickName(s)
                    selfinfo = singlechat.getUserSelfInfo(s)
                    grid.visible = true
                    button.visible = true
                    row2.visible = true
                }

            }
        }


    }


    Grid{
        id:grid
        anchors.top:row1.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 20
        rows: 2;
        columns: 2;
        rowSpacing: 20;
        columnSpacing: 7;
        visible: false

        Text{
            id:nickName
            text:qsTr("好友昵称")
            font.bold: true
            font.pointSize: 18
            color: "#006633"
        }
        Text{
            text:nickname
            font.bold: true
            font.pointSize: 18
            color: "#006633"
        }
        Text{
            id:selfInfo
            text:qsTr("个人说明")
            font.bold: true
            font.pointSize: 18
            color: "#006633"
        }
        Text{
            text:selfinfo
            font.bold: true
            font.pointSize: 18
            color: "#006633"
        }

    }

    Row{
        id:row2
        anchors.top:grid.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 20
        spacing: 5
        visible: false

        Text{
            text:qsTr("添加说明")
            font.bold: true
            font.pointSize: 13
            color: "#006633"
        }
        TextField{
            id:explain
            width:260
            height: 25
            placeholderText: "输入您的添加说明，如：我是..."
            maximumLength: 30

        }
    }


    Button{
        id:button
        visible: false
        text:qsTr("添加")
        isDefault: true
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin:10
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
            singlechat.requestAdd(singlechat.getSelfAccountN(),accountNum.text,singlechat.selfAccountInfo[0],explain.text)
        }
    }

    Loader{
        id:load
        smooth: true
    }



}
