import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    id: element
    width: 400
    height: 600
    enabled: true
    property alias rangeone: rangeone
    property alias clock: clock

    Image {
        id: image
        x: 75
        width: 260
        height: 260
        enabled: connectedItemToolBar.checked
        sourceSize.height: 400
        sourceSize.width: 400
        anchors.top: parent.top
        anchors.topMargin: 65
        anchors.horizontalCenter: parent.horizontalCenter
        source: "cadran.png"
        fillMode: Image.Stretch

        ClockCanvas {
            id: clock
            start: startTime.value
            end: endTime.value
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

                RoundButton {
                    id: roundButton
                    x: 50
                    y: 45
                    width: 75
                    height: 75
                    text: qsTr("+")
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.letterSpacing: -0.2
                    display: AbstractButton.IconOnly

                    Image {
                        id: image1
                        x: 0
                        y: 0
                        width: 65
                        height: 65
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "invertblue.png"
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }
    }

    RangeSlider {
        id: rangeone
        height: 33
        enabled: connectedItemToolBar.checked
        second.value: 1400
        first.value: startTime.value
        stepSize: 5.2
        to: 1439
        anchors.top: image.bottom
        anchors.topMargin: 15
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
    }

    TumblerTime {
        id: startTime
        x: 36
        y: 392
        width: 78
        height: 56
        enabled: connectedItemToolBar.checked
        value: 28
    }

    TumblerTime {
        id: endTime
        x: 114
        y: 392
        width: 84
        height: 56
        enabled: connectedItemToolBar.checked
        value: 2
    }

    Connections {
        target: rangeone.first
        onMoved: startTime.value = rangeone.first.value
    }

    Connections {
        target: rangeone.second
        onMoved: endTime.value = rangeone.second.value
    }

    ConnectedItemToolBar {
        id: connectedItemToolBar
        height: 54
        enabled: true
        checked: true
        title: "Pouet"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
    }
}














/*##^## Designer {
    D{i:3;anchors_y:132}D{i:2;anchors_height:200;anchors_width:200;anchors_x:"-53";anchors_y:68}
D{i:1;anchors_y:65}D{i:6;anchors_x:27;anchors_y:265}
}
 ##^##*/
