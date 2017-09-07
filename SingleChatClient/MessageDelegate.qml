import QtQuick 2.2
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4


Component {
    id:messageDelegate

    Item {
        id:wrapper
        width: parent.width
        height: 50

        MouseArea {
            anchors.fill: parent
            onClicked: {
                wrapper.ListView.view.currentIndex = index
                mouse.accepted = true
            }
            onDoubleClicked: {

                singlechat.setSelectedFriend(/*model.groupName,*/model.friNum)
                console.log(singlechat.selectedFriendAccount)
                singlechat.getChatRecords(model.friNum)
                chatWindowLoader.setSource("chatWindow.qml",{friendName:model.nackName,
                                               friendAccountN:model.friNum})

                singlechat.subOfflineMsgCount()
                console.log("打开聊天窗口")
//                singlechat.changeOfflineMsgCount(wrapper.ListView.view.model.count);
            }
            onPressAndHold: {
                //删除某条好友消息
                msgHandler.removeMsg(index)
//                singlechat.changeOfflineMsgCount(wrapper.ListView.view.model.count);
                singlechat.subOfflineMsgCount()
            }
        }

        Loader {
            id: chatWindowLoader
            smooth: true
        }

        ColumnLayout {
            anchors.left: parent.left
            anchors.leftMargin: 15
            anchors.verticalCenter: parent.verticalCenter
            spacing: 5
            Text {
                id:nickName
                text:model.nackName
                color: wrapper.ListView.isCurrentItem ? "blue" : "black"
                font.pixelSize: 18
            }
            Text {
                id: friendmessage
                text: model.message
                font.pixelSize: 14
                color: "gray"
            }
        }
    }
}
