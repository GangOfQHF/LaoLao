import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Window {
    id:changeGroup
    width: 260
    height: 300
    visible: true

    title: qsTr("移动分组")
    x:700
    y:100

    property string friendNum: ""
    property string selectedGroupName:""
    Component {
        id: groupListViewDele
        Rectangle {
            id: groupRec
            property bool bEntered: false
            height: 30
            width: parent.width
            color: bEntered ? "lightblue" : "#ffffff"

            Text {
                id: groupName
                text: model.groupName
                font.pixelSize: 20
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    selectedGroupName = model.groupName
                    groupRec.ListView.view.currentIndex = index
                }
                onEntered:
                    groupRec.bEntered = true
                onExited:
                    groupRec.bEntered = false
            }
        }
    }

    //    Rectangle {
    //        id: bgRec
    //        anchors.top: parent.top
    //        anchors.topMargin: 10
    //        anchors.horizontalCenter: parent.horizontalCenter
    //        width: parent.width - 10
    //        height: 30
    //        color: "lightgray"
    Text {
        id: tips
        text: qsTr("Groups:")
        font.pixelSize: 15
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        color: "red"
    }
    //    }

    ListView {
        id:groupListView
        anchors.top:tips.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: 200
        height: 100
        model: listModel
        delegate: groupListViewDele
        focus: true
    }

    Button {
        id: okButton
        //        anchors.horizontalCenter: parent.horizontalCenter
        //        anchors.verticalCenter: parent.verticalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: groupListView.left
        anchors.leftMargin: 10
        property string textColor: "white"
        style: SCButtonStyle{ button: okButton; buttonText: qsTr("确定移动");buttonTextColor: okButton.textColor}
        onClicked:{
            var groupName = friendList.getGroupName(friendNum)
            //当选择了好友分组并且选择的分组不是当前分组才移动分组
            if(selectedGroupName && selectedGroupName != groupName){
                singlechat.moveFriendGroup(groupHandler(groupName))
                friendList.changeDataToModel(friendNum,selectedGroupName)
                changeGroup.visible = false
            } else if(selectedGroupName == groupName){
                changeGroup.visible = false
            } else if(!selectedGroupName) {
                messageDialog.text = "请选择一个分组"
                messageDialog.visible = true
            }
        }
    }

    MessageDialog {
        id: messageDialog;
        title: "ERRO Box";
        text: "";
        visible: false;
    }

    function groupHandler(group) {
        return friendNum + "^" + group + "^" +
                selectedGroupName;
    }
}
