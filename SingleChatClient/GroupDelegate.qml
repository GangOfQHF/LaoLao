import QtQuick 2.2
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4


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
                for (var i = 1; i < parent.children.length - 1; ++i) {
                    parent.children[i].visible = !parent.children[i].visible
                }
            }
            //set the friendlist's visibility
            Component.onCompleted: {
                for (var i = 1; i < parent.children.length - 1; ++i) {
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
                    text: (groupColumn.children.length
                           >1 ? groupColumn.children[1].visible ? qsTr("-  ") : qsTr(
                                                                       "+ ") : qsTr(
                                                                       "   ")) + model.groupName
                }
            }
        }
        Repeater {
            model: chs
            delegate: FriendDelegate {/*currentGroup: model.groupName*/}
            focus: true
        }
    }
}
