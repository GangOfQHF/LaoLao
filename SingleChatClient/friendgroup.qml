import QtQuick 2.2
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    //Group's delegate
    Component {
        id: groupDelegate
        Column {
            id: groupColumn
            clip: true
            MouseArea {
                width: groupRow.implicitWidth
                height: groupRow.implicitHeight
                //for collapsing and expanding
                onClicked: {
                    for(var i = 1; i < parent.children.length - 1; ++i) {
                        parent.children[i].visible = !parent.children[i].visible
                    }
                }
                //set the friendlist's visibility
                Component.onCompleted: {
                    for(var i = 1; i < parent.children.length - 1; ++i) {
                        parent.children[i].visible = false
                    }
                }

                //Every friendGroup style
                Row {
                    id: groupRow
                    //for indentation
                    Item {
                        height: 1
                        width: 10
                    }
                    //show the groupName
                    Text {
                        text: (groupColumn.children.length > 2 ?
                                   groupColumn.children[1].visible ?
                                       qsTr("-  ") : qsTr("+ ") : qsTr("   ")) + model.groupName
                    }
                }
            }
            Repeater {
                model: chs
                delegate: friendDelegate
                focus: true
            }
        }
    }

    //friend's delegate
    Component {
        id:friendDelegate
        //                MouseArea {
        //                    width: friendRow.implicitWidth
        //                    height: friendRow.implicitHeight
        //                    onClicked: {
        //                        console.log("Selected the friend")
        //                    }
        //                    onDoubleClicked: {
        //                        console.log("begin chat")
        //                    }
        //                    Row {
        //                        id: friendRow
        //                        spacing: 5
        //                        Item {
        //                            height: 1
        //                            width: 15
        //                        }
        ////                        TextField {
        ////                            id: friendremark
        ////                            text: model.remarksName
        ////                            font.pointSize: 15
        ////                        }
        //                        TextField {
        //                            id: textField
        //                            text: model.remarksName
        //                            font.pointSize: 10
        //                            readOnly: true
        //                            menu: Menu {
        //                                MenuItem {
        //                                    text: qsTr("发送消息")
        //                                    onTriggered:
        //                                        console.log("send message")
        //                                }
        //                            }
        //                        }
        ////                        Text {
        ////                            text: model.signup
        ////                            anchors.bottom: friendremark.bottom
        ////                            font.pointSize: 10
        ////                        }
        //                    }
        //                }

        Rectangle {
            id: friendRec
            height: 25
            width: 200
            property string friendName: model.remarksName
            property bool bEntered: false
            color: "lightgray"/*model.bSelected ? "#FCEAA3" : (bEntered ? "#FCF0C1" : "#FFFFFF")*/
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
                id: textField
                anchors.left: split.right
                anchors.leftMargin: 5
                text: model.remarksName
                readOnly: true
                font.pointSize: 10
                menu: Menu {
                    MenuItem {
                        text: qsTr("发送消息")
                        onTriggered:
                            console.log("send message")
                    }
                }
                style: TextFieldStyle{bgColor: friendRec.color; borderWidth: 0}
            }

            MouseArea {
                anchors.fill: friendRec
                hoverEnabled: true
                onEntered: {
                    friendRec.bEntered = true;
                }
                onExited: {
                    friendRec.bEntered = false;
                }
                onClicked: {
                    //                    EcInteraction.friendList.setbSelected(model.groupName, model.account);
                    console.log("selected friend")
                }
                onDoubleClicked: {
                    //                    chatWithFriend(memberRec.friendName, memberRec.friendAccount, EcInteraction.selfAccount);
                    console.log("begin chat")
                }
            }
        }
    }


    ListView {
        anchors.fill: parent
        model:listModel
        delegate: groupDelegate

    }
}
