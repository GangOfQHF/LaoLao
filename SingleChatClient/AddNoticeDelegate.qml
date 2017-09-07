import QtQuick 2.2
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Component {
    id: addNoticeDelegate

    Item{
        id:item
        anchors.fill: parent
        MouseArea{
            anchors.fill: parent
            onClicked: {
                item.ListView.view.currentIndex = index
            }

        }

        Column {
            id: noticeColumn
            spacing: 2
            Text{
                id:name
                text:"昵称："+model.nickname
                font.bold: true
                font.family: "楷体"
                font.pointSize: 40
                style:Text.Raised
                color: "#ff6666"
            }
            Text{
                id:explain
                text:"添加说明:" + model.addExplain
                font.bold: true
                font.family: "楷体"
                font.pointSize: 40
                style:Text.Raised
                color: "#ff6666"
            }
            Text{
                id:addtime
                text:"添加时间:" + model.time
                font.bold: true
                font.family: "楷体"
                font.pointSize: 40
                style:Text.Raised
                color: "#ff6666"
            }
            Row{
                id:row
                //            anchors.bottom:parent.bottom
                //            anchors.bottomMargin: 2
                //            anchors.right: parent.right
                //            anchors.rightMargin: 5
                spacing: 5
                Button{
                    id:button1
                    text:qsTr("同意")
                    isDefault: true
                    style: ButtonStyle {
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 25
                            border.width: control.activeFocus ? 2 : 1
                            border.color: "#888"
                            radius: 4
                            color: (control.hovered || control.pressed) ? "#ff6666":"#ffcccc"

                        }
                    }
                    onClicked: {
                        singlechat.sendAnswerNotice("0",singlechat.selfAccountNumber,model.accountNum,singlechat.selfAccountInfo[0],model.nickname,Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss"))
                    }
                }



                Button{
                    id:button2
                    text:qsTr("拒绝")
                    isDefault: true
                    style: ButtonStyle {
                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 25
                            border.width: control.activeFocus ? 2 : 1
                            border.color: "#888"
                            radius: 4
                            color: (control.hovered || control.pressed) ? "#ff6666":"#ffcccc"
                        }
                    }
                    onClicked: {
                        singlechat.sendAnswerNotice("1",singlechat.selfAccountNumber,model.accountNum,singlechat.selfAccountInfo[0],model.nickname,Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss"))
                    }
                }
            }

        }
    }
}

