import QtQuick 2.2
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4

Component {
    id: friendDelegate
    Rectangle {
        id: friendRec
        height: 50
        width: 300
        property bool bEntered: false
//        property string currentGroup
        property string friendName: model.nickName
        property string friendAccount: model.account

        //when the group have member,set the visible is true,else set the visible is false
        visible: model.account ? true : false
        Component.onCompleted: {
            console.log("分组内好友的显示情况",friendRec.visible)
        }

        color: bEntered ? "lightblue" : "#FFFFFF"
        radius: 3
        //for indentation
        Item {
            id: split
            height: 1
            width: 15
            anchors.left: parent.left
            anchors.leftMargin: 5
        }
        TextField {
            id: nickNameShow
            width: 70
            anchors.left: split.right
            anchors.leftMargin: 5
            //当备注为空的时候就显示昵称
            text: model.remarksName ? model.remarksName : model.nickName

            readOnly: true
            font.pointSize: 10
            menu: Menu {
                MenuItem {
                    text: qsTr("发送消息")
                    onTriggered: console.log("send message")
                }
                MenuItem {
                    text: qsTr("修改备注")
                    onTriggered: {
                        load.setSource("ModifyRemarks.qml")
                    }
                }

                MenuItem {
                    text: qsTr("移动分组")
                    onTriggered: {
                        changeGroupLoad.setSource("ChangeGroup.qml",{friendNum:friendRec.friendAccount/*,currentGroupName: friendRec.currentGroup*/})
                    }
                }
            }

            style: TextFieldStyle {
                bgColor: friendRec.color
                borderWidth: 0
            }
        }


        Text {
            id: signupShow
            width: 200
            anchors.top: nickNameShow.bottom
            anchors.left: split.right
            anchors.leftMargin: 10
            //when model.signup is null,set the text is null string
            text: model.signup ? "(" + model.signup + ")" : ""
            font.pointSize: 8
            color: "gray"
            elide: Text.ElideRight
        }

        Loader {
            id: load
            smooth: true
        }

        Loader {
            id: changeGroupLoad
            smooth: true
        }

        Loader {
            id: chatWindowLoader
            smooth: true
        }

        MouseArea {
            anchors.fill: friendRec
            hoverEnabled: true
            onEntered: {
                friendRec.bEntered = true
            }
            onExited: {
                friendRec.bEntered = false
            }
            onClicked: {


                //                singlechat.setSelectedFriend(model.groupName,model.account)
                //                console.log(singlechat.selectedFriendAccount)
                //                console.log("selected friend")
            }
            onDoubleClicked: {
                singlechat.setSelectedFriend(model.account)
                console.log(singlechat.selectedFriendAccount)
                singlechat.getChatRecords(friendRec.friendAccount)
                chatWindowLoader.setSource("chatWindow.qml", {
                                               friendName: friendRec.friendName,
                                               friendAccountN: friendRec.friendAccount
                                           })
                console.log("打开聊天窗口")
            }
        }
    }
}
