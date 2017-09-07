import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4

Window {
    id:mainWindow
    visible: true
    width: 300
    height: 400
    title: qsTr("添加请求")
    x:400
    y:360

    Text{
        id:header
        text:"请求添加好友"
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        font.family: "楷体"
        font.pointSize: 56
        style:Text.Raised
        color: "#ff6666"
    }

    //    Rectangle{
    //        width: parent.width - 10
    //        anchors.top:header.bottom
    //        anchors.topMargin: 5
    //        anchors.leftMargin: 10
    //        anchors.rightMargin: 5
    //        anchors.bottom: parent.bottom
    //        anchors.bottomMargin: 5

    ListView{
        id:lst
        model:addNoticeItemModel
        delegate: AddNoticeDelegate{}
        anchors.top:header.bottom
        anchors.topMargin: 2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        focus: true
        highlight: Rectangle {
            color: "#ffffcc"
        }
    }

}
