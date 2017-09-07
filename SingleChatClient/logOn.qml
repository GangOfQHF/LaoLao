import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2

Window{
    id:logOnWindow;
    width: 360;
    height: 260;
    visible: true;
    title: qsTr("登录");
    x: 500;
    y: 200;

    property string accountNum: ""

    Image {
        id: background;
        source: "Image/image2.jpg";
        anchors.fill: parent;
        fillMode: Image.PreserveAspectCrop;
    }

    Text {
        id: title
        text: qsTr("登录")
        anchors.top:parent.top;
        anchors.topMargin: 15;
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true;
        font.family: "黑体"
        font.pointSize: 54
        style: Text.Raised;
        color: "#ff0084"
    }

    Grid {
        id: grid;
        anchors.top:title.bottom;
        anchors.topMargin: 60;
        anchors.horizontalCenter: parent.horizontalCenter;
        rows: 2;
        columns: 2;
        columnSpacing: 10;
        rowSpacing: 15;

        Text {
            id: user;
            text: qsTr("帐号");
            font.bold: true
            color: "#ff00f7"
            font.pointSize: 18;
        }

        TextField {
            id:userName;
            width: 180;
            height: 30;
            maximumLength: 6
            text: logOnWindow.accountNum
            placeholderText: "输入6位数的用户帐号";
        }

        Text {
            id: passWord;
            text: qsTr("密码");
            color: "#ff00f7"
            font.bold: true;
            font.pointSize: 18;
        }
        TextField {
            id:key;
            width: 180;
            height: 30;
            maximumLength: 13
            placeholderText: "输入6～13位数的密码";
            echoMode: TextInput.Password;
        }
    }

    Row {
        anchors.top: grid.bottom;
        anchors.topMargin: 30;
        anchors.left: parent.left;
        anchors.leftMargin: 140;
        spacing: 10;

        Button {
            id:logon_button;
            text: qsTr("登录");

            //            isDefault: true;
            style: ButtonStyle {
                background: Rectangle{
                    implicitWidth: 70;
                    implicitHeight: 25;
                    opacity: 0.2;
                    border.width: control.pressed ? 2 : 1;
                    border.color: (control.hovered || control.pressed) ? "green" : "#888888";
                }
            }
            onClicked: {
                if(1 == check_text()) {
                    if(logOn()){
                        friendListLoader.setSource("FriendList.qml");
                        logOnWindow.visible = false;
                    }
                }
            }
            Keys.enabled: true;
            Keys.onReturnPressed: {
                if(check_text()) {
                    if(logOn()){
                        friendListLoader.setSource("FriendList.qml");
                        logOnWindow.visible = false;
                    }
                }
            }
        }

        Button {
            text: qsTr("注册");
            style: ButtonStyle {
                background: Rectangle{
                    implicitWidth: 70;
                    implicitHeight: 25;
                    opacity: 0.2;
                    border.width: control.pressed ? 2 : 1;
                    border.color: (control.hovered || control.pressed) ? "green" : "#888888";
                }
            }

            onClicked: {
                registerLoader.setSource("Register.qml");
                logOnWindow.visible = false;
            }
        }
    }

    Loader {
        id: friendListLoader;
        smooth: true;
    }

    Loader {
        id: registerLoader;
        smooth: true;
    }

    MessageDialog {
        id: messageDialog;
        title: "ERRO Box";
        text: "";
        visible: false;
    }

    function check_text(){
        if(userName.text.length < 6){
            messageDialog.text = "用户帐号的格式不对，只能输入6位！";
            messageDialog.visible = true;
        }
        else if(key.text.length < 6){
            messageDialog.text = "用户密码格式不对，输入6~13位数!";
            messageDialog.visible = true;
        } else return 1;
    }

    function logOn(){
        console.log("123")
        if(singlechat.requestVd(userName.text,key.text)) {
            console.log("登录成功");
            singlechat.getAccount(userName.text);
            singlechat.getFriends(userName.text);
//            singlechat.getFriendFromQml(allFri);
            singlechat.getOfflineMessage(userName.text);
            singlechat.getOffNotice(userName.text);
            return 1;
        } else {
            messageDialog.text = "帐户或密码不正确";
            messageDialog.visible = true;
            key.text = "";
            return 0;
        }
    }
}
