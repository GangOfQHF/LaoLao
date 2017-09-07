import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.2


//聊天界面
Window {
    id:chattingWindow
    title: qsTr("Chat")
    width:600;
    height:400;
    visible: true
    modality: Qt.ApplicationModal

    property string  friendName: ""
    property string  friendAccountN: ""
    property string  selfAccountN: singlechat.selfAccountNumber
    property string  selfNickName: singlechat.selfAccountInfo[0]

    onClosing: {
        singlechat.removeChatMsgModel()
        singlechat.clearSelectedFriend();
        console.log("退出聊天，清空聊天记录")
    }

    //整个界面
    Rectangle
    {
        id:rec
        visible: true
        anchors.fill:parent

        Rectangle{
            id:msgRct
            width:parent.width
            height:parent.height*2/3-parent.height/17
            anchors.top:parent.top
            anchors.topMargin: 10
            anchors.bottom: sendRec.top
            anchors.bottomMargin: 2
            ListView {
                id: chatMsgView
                anchors.fill: parent
                model:chatMsgItemModel
                delegate: friendMessageDelegate
            }
        }

        Component {
            id: friendMessageDelegate

            Item{
                width:rec.width/3 + rec1.width + 3
                height: rec2.height + 10

                //好友备注/自己
                Rectangle{
                    id:rec1
                    width: nickName.width + 1
                    height: nickName.height + 10
                    Text{
                        id:nickName
                        x:model.direction == 1 ? 20 : rec.width-70
                        text:model.nickname
                        color: "red"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                //好友消息
                Rectangle{
                    id:rec2
                    x:model.direction == 1 ? nickName.x + nickName.width + 5 : nickName.x-width
                    width: rec.width/3
                    height: msg.height+ time.height
                    color: "lightblue"
                    Text{
                        id:msg
                        width:parent.width
                        wrapMode: Text.Wrap
                        text:model.message
                    }

                    Text{
                        id:time
                        width:parent.width
                        wrapMode: Text.Wrap
                        text:model.time
                        color: "gray"
                        anchors.top: msg.bottom
                    }

                }
            }

        }


        //发送按钮
        Rectangle{
            id: sendRec
            width:parent.width
            height:parent.height/18
            //            anchors.top:msgRct.bottom
            anchors.bottom: textView.top
            anchors.bottomMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 3
            Rectangle{
                width: parent.width/17
                height: parent.height
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.bottomMargin: 3
                Button{
                    id:sendButton
                    objectName: "buttonObject"
                    property string textColor: "white"
                    anchors.fill:parent
                    style:SCButtonStyle{button: sendButton;buttonText: qsTr("发送");buttonTextColor: sendButton.textColor}
                    onClicked: {
                        if(textView.text) {
                            handleMessage();
                            chatMsgView.positionViewAtIndex(chatMsgView.count - 1,chatMsgView.End)
                        }
                        else {
                            warningDialog.text = "发送内容不能为空"
                            warningDialog.visible = true
                        }
                    }

                }

            }
        }

        //输入区域
        TextArea{
            id:textView;
            x:0
            y:parent.height-height
            width:parent.width
            height:parent.height/3
            wrapMode: TextEdit.WordWrap
            style:TextAreaStyle{
                backgroundColor: "lightblue"
                textColor: "black"
                selectedTextColor: "#a00000"
                selectionColor: "lightblue"
            }

        }
        //发送内容为空时的提示框

        MessageDialog {
            id: warningDialog;
            title: "warning";
            text: "";
            visible: false;
        }
    }
    function handleMessage(){
        console.log("send Message:" + textView.text)
        var time = Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss");
        console.log(time)
        chatMsgModel.addDataToModel("me",textView.getText(0,textView.length),0,time)
        singlechat.sendMessage(messageHandler(time))
        textView.text = ""
    }

    function messageHandler(timeMsg) {
        var msg = chattingWindow.selfAccountN + "^" + chattingWindow.friendAccountN + "^"
                + chattingWindow.selfNickName + "^" + timeMsg + "^" + textView.text
        return msg;
    }
}







