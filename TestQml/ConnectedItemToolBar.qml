import QtQuick 2.0
import QtQuick.Controls 2.3
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4

ToolBar {
    property alias title: label.text
    property alias checked : swith.checked

    RowLayout {
        anchors.fill: parent
        ToolButton {
            iconSource: "previous.png"
            anchors.margins: 0
        }

        Label {
            id: label
            text: "Title"
            font.pixelSize: 30
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            Layout.fillWidth: true
            enabled: swith.checked
        }

        ToolButton {
            iconSource: "menu.png"
            anchors.margins: 0
        }

        Switch {
            id: swith
            style: SwitchStyle {
                groove: Rectangle {
                        implicitWidth: 48
                        implicitHeight: 28
                        radius: 14
                        border.color: control.activeFocus ? "darkblue" : "gray"
                        color: control.checked ? "#AABBDD" : "#DDDDDD"
                        border.width: 1
                }
                handle: Rectangle{
                    implicitWidth: 28
                    implicitHeight: 28
                    radius: 14
                    border.width: 1
                    color: control.checked ? "#557799" : "gray"
                }
            }
        }
    }
}
