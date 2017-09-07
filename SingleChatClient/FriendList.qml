import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4

Window {
    id:mainWindow
    visible: true
    width: 300
    height: 480
    title: qsTr("SingleChat")
    x:500
    y:200

    property int modelNum:addNoticeItemModel.count

    onClosing: {
        singlechat.clientExit(singlechat.selfAccountNumber)
        Qt.quit()
    }

    Rectangle {
        id:personalShow
        height: 40
        anchors.top: parent.top
        anchors.topMargin: 2
        width: parent.width
        color: "#09A3DC"
    }
    Text {
        id: nickName
        anchors.top: parent.top
        anchors.topMargin: 2
        anchors.horizontalCenter: parent.horizontalCenter
        text: singlechat.selfAccountInfo[0]
        color: "white"
    }
    Text {
        id: signup
        anchors.top: nickName.bottom
        anchors.right: parent.right
        anchors.rightMargin: 30
        text: singlechat.selfAccountInfo[1]
        color: "white"
    }


    Rectangle {
        id: header
        height: 35
        color: "#09A3DC"
        anchors.top: personalShow.bottom
        anchors.topMargin: 1
        width: parent.width
        //message
        Button {
            id: messagePageButon
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            property string textColor: "white"
            style: SCButtonStyle{ button: messagePageButon; buttonText: qsTr("消息");buttonTextColor: messagePageButon.textColor }
            //消息提示
            Rectangle {
                id: logRec
                anchors.left: parent.right
                anchors.leftMargin: -15
                width: 10
                height: 10
                radius: 90
                color: "red"
                visible: false
            }
            onClicked: {
                swipeView.currentIndex = 0
                textColor = "white"
            }
        }
        //friend
        Button {
            id: friendListPageButton
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            style: SCButtonStyle{ button: friendListPageButton; buttonText: qsTr("好友") }
            onClicked: {
                swipeView.currentIndex = 1
            }
        }
        //group
        Button {
            id: groupListPageButton
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin:5
            style: SCButtonStyle{ button: groupListPageButton; buttonText: qsTr("群组") }
            onClicked: {
                swipeView.currentIndex = 2
            }
        }
    }


    SwipeView {
        id: swipeView
        anchors.top: header.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: footer.top
        currentIndex: 0
        //第一页，即消息页
        Page {
            id: firstPage
            ListView {
                id:listView
                anchors.fill: parent
                delegate: MessageDelegate{}
                model: messageModel
                focus: true
                highlight: Rectangle {
                    color: "lightblue"
                }

            }
        }
        //第二页，即选择联系人
        Page {
            id: sencondPage
            ListView {
                anchors.fill: parent
                model:listModel
                delegate: GroupDelegate{}
            }

        }
        function a()
        {
            for(var i = 0; i < listView.model.count; i++)
            {
                var x = listView.model.get(i);
                console.log(x.account)
            }
        }
        //第三页，即群组页，现在还不提供支持
        Page {
            id:thirdPage
            Label {
                text:qsTr("third page")
                anchors.centerIn: parent
            }
        }
    }
    Rectangle {
        id: footer
        height: 35
        color:"#09A3DC"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Button {
            id: zoneButton
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            property string textColor: "white"
            style: SCButtonStyle{ button: zoneButton; buttonText: qsTr("空间");buttonTextColor: zoneButton.textColor }
            onClicked: {
                console.log("zone")
                textColor = "white"
            }
        }


        Button {
            id: createGroupButton
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            property string textColor: "white"
            style: SCButtonStyle{ button: createGroupButton; buttonText: qsTr("添加&创建");buttonTextColor: createGroupButton.textColor}
            Rectangle {
                id: rec
                anchors.left: parent.right
                anchors.leftMargin: -10
                width: 10
                height: 10
                radius: 90
                color: "red"
                visible: modelNum > 0 ? true : false
            }

            menu: Menu{
                id:addMenu
                MenuItem{
                    text: "添加好友"
                    onTriggered: {
                        load.setSource("FindFriend.qml")
                    }
                }
                MenuItem{
                    text:"创建群"
                    onTriggered: {
                        console.log("创建群")
                    }
                }
                MenuItem{
                    text: "创建好友分组"
                    onTriggered: {
                        load.setSource("CreateGroup.qml")
                    }
                }
                MenuItem{
                    id:menuItem
                    text:"添加请求"
                    iconSource: modelNum > 0 ? "Image/red.jpeg" :"Image/image6.jpg"
                    onTriggered: {
                        load.setSource("AddFriendNoticeList.qml")
                        modelNum = 0
                    }

                }
            }

        }

        Connections{
            target: singlechat
            onSig_recvNotice:{
                modelNum = addNoticeItemModel.count
            }
        }

        Loader{
            id:load
            smooth: true
        }

        Button {
            id: exitButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            property string textColor: "white"
            style: SCButtonStyle{ button: exitButton; buttonText: qsTr("退出");buttonTextColor: exitButton.textColor }
            onClicked:
            {
                singlechat.clientExit(singlechat.selfAccountNumber)
                Qt.quit()
            }
        }
    }

    Connections {
        target: singlechat
        onUnReadMsgCountChanged:{
            console.log(count)
            if(!count) {
                logRec.visible = false;
            } else {
                logRec.visible = true;
            }
        }
    }

    Component.onCompleted: {
        if(listView.count != 0){
            logRec.visible = true
        }else{
            logRec.visible = false
        }
        singlechat.callThread();
    }
}

