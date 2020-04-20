import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Extras 1.4

Tumbler {
    id: root
    property int value

    onValueChanged: {
        if(hours.currentIndex != Math.floor(value / 60))
            setCurrentIndexAt(0, Math.floor(value / 60));
        if(minutes.currentIndex != value % 60)
            setCurrentIndexAt(1, value % 60);
    }

    TumblerColumn
    {
        id: hours
        width: 30
        onCurrentIndexChanged: root.value = hours.currentIndex * 60 + minutes.currentIndex
        model: ListModel {
            Component.onCompleted: {
                for (var i = 0; i < 24; ++i) {
                    append({value: i.toString().padStart(2, "0")});
                }
            }
        }
    }

    TumblerColumn {
        id: minutes
        width: 30
        onCurrentIndexChanged: root.value = hours.currentIndex * 60 + minutes.currentIndex
        model: ListModel {
            Component.onCompleted: {
                for (var i = 0; i < 60; ++i) {
                    append({value: i.toString().padStart(2, "0")});
                }
            }
        }
    }
}
