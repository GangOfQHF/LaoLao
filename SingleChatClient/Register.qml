import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

//注册界面
Window {
    id:registerWindow;
    width: 360;
    height: 500;
    visible: true;
    title: qsTr("注册界面");
    x: 400;
    y: 200;

    property var result;
    property var str;
    //背景图片
    Image {
        id: image;
        source: "Image/image6.jpg";
        anchors.fill: parent;
        fillMode: Image.PreserveAspectCrop;
    }
    //注册头部标题
    Text {
        id:  title;
        text: qsTr("注册");
        anchors.top:parent.top;
        anchors.topMargin: 15;
        anchors.horizontalCenter: parent.horizontalCenter;
        font.bold: true;
        font.family: "黑体"
        font.pointSize: 56;
        style: Text.Raised;
    }
    //注册布局
    Grid {
        id:grid;
        anchors.top:title.top;
        anchors.topMargin: 40;
        anchors.left: parent.left;
        anchors.leftMargin: 50;
        rows: 6;
        columns: 2;
        rowSpacing: 10;
        columnSpacing: 5;

        Text {
            id: text1;
            text: qsTr("昵称");
            font.bold: true;
            font.pointSize: 18;
        }
        TextField {
            id:textField1;
            width: 200;
            height: 30;
            placeholderText: "输入一个名字,如：zhangsan";
            maximumLength: 7;
        }
        Text {
            id: text2;
            text: qsTr("生日");
            font.bold: true;
            font.pointSize: 18;
        }
        TextField {
            id:textField2;
            width: 200;
            height: 30;
            placeholderText: "形如：1995-11-08";
            validator: RegExpValidator{
                regExp: (/[1-3][0-9][0-9][0-9][-][0-1][0-9][-][0-3][0-9]/)
            }
        }
        Text {
            id: text3;
            text: qsTr("密码");
            font.bold: true;
            font.pointSize: 18;
        }
        TextField {
            id:textField3;
            width: 200;
            height: 30;
            placeholderText: "输入6～13位的字母或者数字";
            echoMode: TextInput.Password;
            maximumLength: 13;
        }
        Text {
            id: text4;
            text: qsTr("密码");
            font.bold: true;
            font.pointSize: 18;
        }
        TextField {
            id:textField4;
            width: 200;
            height: 30;
            placeholderText: "重新输入密码";
            echoMode: TextInput.Password;
            maximumLength: 13;
        }
        Text {
            id: text5;
            text: qsTr("地址");
            font.bold: true;
            font.pointSize: 18;
        }
        TextField {
            id:textField5;
            width: 200;
            height: 30;
            placeholderText: "如：chongqing";
            maximumLength: 20;
//            validator: RegExpValidator{
//                regExp: (/^[A-z]*$/)
//            }
        }
        Text {
            id:text6;
            text: qsTr("个人说明");
            font.bold: true;
            font.pointSize: 18;
        }
        TextField {
            id:textField6;
            width: 200;
            height: 30;
            maximumLength: 50;
            placeholderText: "编写你的座右铭";
        }
    }
    //CheckBox布局
    Row {
        anchors.top:grid.bottom;
        anchors.topMargin: 15;
        anchors.left: parent.left;
        anchors.leftMargin: 50;
        spacing: 10;
        Text {
            id: text7;
            text: qsTr("工作");
            font.bold: true;
            font.pointSize: 18;
        }
        CheckBox {
            id:student;
            text:qsTr("学生");
            exclusiveGroup: mos;
            checked: true;
            activeFocusOnPress: true;
            style: checkStyle;
            Component.onCompleted: str = student.text
            onClicked: {
                str = student.text;
            }
        }
        CheckBox {
            id:teacher;
            text:qsTr("教师");
            exclusiveGroup: mos;
            activeFocusOnPress: true;
            style: checkStyle;
            onClicked: {
                str = teacher.text;
            }
        }
        CheckBox {
            id:other;
            text:qsTr("其他职业");
            exclusiveGroup: mos;
            activeFocusOnPress: true;
            style: checkStyle;
            onClicked: {
                str = other.text;
            }
        }
    }

    ExclusiveGroup {
        id:mos;
    }

    Component {
        id:checkStyle;
        CheckBoxStyle {
            indicator: Rectangle {
                implicitHeight: 15;
                implicitWidth: 15;
                radius: 10;
                border.color: control.hovered ? "black" : "gray";
                border.width: 1;
                Rectangle {
                    anchors.fill: parent;
                    visible: control.checked;
                    color: "black";
                    radius: 10;
                    anchors.margins: 3;
                }
            }
            label: Text {
                color: control.activeFocus ? "blue" : "black";
                text: control.text;
                font.pointSize: 18;
            }
        }
    }
    //注册完成点击“确定”按钮
    Button {
        text: qsTr("确定");
        isDefault: true;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 140;
        anchors.left: parent.left;
        anchors.leftMargin: 230;
        style: ButtonStyle {
            background: Rectangle{
                implicitWidth: 70;
                implicitHeight: 25;
                opacity: 0.3;
                border.width: control.pressed ? 2 : 1;
                border.color: (control.hovered || control.pressed) ? "blue" : "#888888";
            }
        }
        onClicked: {
            if(check_textInput() == 1){
                result = randomNum();
                console.log(messageHander());
                singlechat.registerToDatabase(messageHander());
                messageDialog.title = "您的账号";
                messageDialog.text = "您的账号为： "+ result;
                messageDialog.visible = true;
                logOnLoader.setSource("logOn.qml",{accountNum: result});
                registerWindow.visible = false;
            }
        }
    }

    //加载登录页面的Loader
    Loader {
        id:logOnLoader;
        smooth: true;
    }
    //错误提示框
    MessageDialog {
        id: messageDialog
        title: "错误提示框"
        visible: false;
    }
    //检查注册时的输入错误
    function check_textInput(){
        var newDate = new Date();
        if(textField3.text != textField4.text){
            messageDialog.text = "密码输入不匹配，请重新输入密码！";
            messageDialog.visible = true;
            textField3.text = "";
            textField4.text = "";
        }
        else if(!textField1.text){
            messageDialog.text = "昵称输入不能为空，请完善信息";
            messageDialog.visible = true;
        }
        else if(!textField2.text){
            messageDialog.text = "生日输入不能为空，请完善信息";
            messageDialog.visible = true;
        }
        else if(!textField3.text){
            messageDialog.text = "密码输入不能为空，请完善信息";
            messageDialog.visible = true;
        }
        else if(!textField4.text){
            messageDialog.text = "重新输入密码框不能为空，请完善信息";
            messageDialog.visible = true;
        }
        else if(!textField5.text){
            messageDialog.text = "地址输入不能为空，请完善信息";
            messageDialog.visible = true;
        }
        else if(!textField6.text){
            messageDialog.text = "个人说明输入不能为空，请完善信息";
            messageDialog.visible = true;
        }
        else if(!textField1.text || !textField2.text || !textField3.text || !textField4.text || !textField5.text){
            messageDialog.text = "输入不能为空，请完善信息";
            messageDialog.visible = true;
        }
        else if(textField3.text.length < 6)
        {
            messageDialog.text = "密码不能小于6位数";
            messageDialog.visible = true;
        }

        else if(Number(textField2.text.slice(0,4)) > newDate.getFullYear() || (textField2.text[5] == 1 && textField2.text[6] > 2) || (textField2.text[8] == 3 && textField2.text[9] > 1) || textField2.text.length < 10){
            messageDialog.text = "您输入的日期不存在，请重新输入！";
            messageDialog.visible = true;
            textField2.text = "";
        }

        else {
            return 1;
        }
    }
    //按时间分配注册账号
    function randomNum(){
        var date = new Date();
        var num = date.getTime();
        var str = num.toString();
        var result = str.substring((str.length-6),str.length);
        return result;
    }
    //获取注册信息
    function messageHander(){
        var msg = result + "^" + textField4.text + "^" + textField1.text + "^" + textField6.text + "^" + textField2.text + "^" + str + "^" + textField5.text;
        return msg;
    }
}
