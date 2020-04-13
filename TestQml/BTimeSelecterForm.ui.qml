import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    id: element
    width: 400
    height: 600

    Image {
        id: image
        x: 75
        width: 260
        height: 260
        sourceSize.height: 400
        sourceSize.width: 400
        anchors.top: parent.top
        anchors.topMargin: 65
        anchors.horizontalCenter: parent.horizontalCenter
        source: "cadran.png"
        fillMode: Image.Stretch

        ClockCanvas {
            id: clockCanvas
            anchors.rightMargin: 30
            anchors.leftMargin: 30
            anchors.bottomMargin: 30
            anchors.topMargin: 30
            anchors.fill: parent

            Rectangle {
                id: rectangle1
                x: 40
                y: 132
                width: 160
                height: 160
                color: "#ffffff"
                radius: 80
                anchors.verticalCenterOffset: 0
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

    }

    RangeSlider {
        id: rangeSlider
        height: 41
        anchors.top: image.bottom
        anchors.topMargin: 15
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        first.value: 0.25
        second.value: 0.75
    }

    ToolBar {
        id: toolBar
        x: 70
        y: 65
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
    }




}

/*##^## Designer {
    D{i:3;anchors_y:132}D{i:2;anchors_height:200;anchors_width:200;anchors_x:-53;anchors_y:68}
D{i:1;anchors_y:65}D{i:4;anchors_x:27;anchors_y:265}D{i:5;anchors_width:360;anchors_x:20;anchors_y:8}
}
 ##^##*/
