import QtQuick 2.0
Item
{
    property int start: 0
    property int end: 589
    Rectangle{
        anchors.fill: parent
        radius: width/2
        color: "#5B5555"
    }
    Canvas {
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();

            var centreX = width / 2;
            var centreY = height / 2;
/*
            ctx.beginPath();
            ctx.fillStyle = "black";
            ctx.moveTo(centreX, centreY);
            ctx.arc(centreX, centreY, width / 2, 0, Math.PI * 0.5, false);
            ctx.lineTo(centreX, centreY);
            ctx.fill();*/

            ctx.beginPath();
            ctx.fillStyle = "#E85555";
            ctx.moveTo(centreX, centreY);
            ctx.arc(centreX, centreY, width / 2, 1.0*(start-360)*Math.PI /720, 1.0*(end-start-360)*Math.PI /720, false);
            ctx.lineTo(centreX, centreY);
            ctx.fill();
        }
    }
}
