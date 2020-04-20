import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Scroll")

    BTimeSelecterForm
    {
        anchors.fill: parent
    }

/*
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
    }*/
}
