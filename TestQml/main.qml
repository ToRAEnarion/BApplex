import QtQuick 2.12
import QtQuick.Controls 2.5
import "BTimeSelecter.qml"

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Scroll")

    Loader{
        source: "../BTimeSelecter.qml";
    }

    ScrollView {
        anchors.fill: parent

        ListView {
            width: parent.width
            model: 2
            delegate: ItemDelegate {
                text: "Item " + (index + 1)
                width: parent.width
            }
        }
    }
}
