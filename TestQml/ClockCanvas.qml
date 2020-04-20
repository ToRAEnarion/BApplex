import QtQuick 2.0
Item
{
    property int start: 0
    property int end: 255

    property alias startAnim: startAnim
    property alias endAnim: endAnim

    Rectangle{
        anchors.fill: parent
        radius: width/2
        color: parent.enabled ? "#66686B" : "#777777"
    }
    onStartChanged: canva.requestPaint()
    onEndChanged: canva.requestPaint()
    onEnabledChanged: canva.requestPaint()

    NumberAnimation {id: startAnim; property: "start"; to: rangeone.first.value; duration: 1000 }
    NumberAnimation { id: endAnim; property: "end"; to: rangeone.second.value; duration: 1000 }

    Canvas {
        anchors.fill: parent
        id: canva

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();

            var centreX = width / 2;
            var centreY = height / 2;

            var st1 = start;
            var end1 = end;

/*
            ctx.beginPath();
            ctx.fillStyle = "black";
            ctx.moveTo(centreX, centreY);
            ctx.arc(centreX, centreY, width / 2, 0, Math.PI * 0.5, false);
            ctx.lineTo(centreX, centreY);
            ctx.fill();*/

            ctx.beginPath();
            ctx.fillStyle =  parent.enabled ? "#667FD8" : "#555E77";
            ctx.moveTo(centreX, centreY);
            ctx.arc(centreX, centreY, width / 2, 1.0*(st1-360)*Math.PI /720, 1.0*(end1-359)*Math.PI /720, false);
            ctx.lineTo(centreX, centreY);
            ctx.fill();
        }
    }
}
