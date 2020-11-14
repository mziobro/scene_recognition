import QtQuick 2.0


Rectangle {
    id: root
    property string text: ""
    color: Qt.darker("#145235")
    signal clicked()
    height: 50
    width: 130
    radius: 4
    property alias animation: pressedAnimation
    Text {
        anchors.centerIn: parent
        text: root.text
        color: "white"
        font {
            bold: true
            pixelSize: 25
        }
    }
    MouseArea{
        anchors.fill: parent
        onClicked: {
            root.clicked()
            console.log("Hey, "  + text + "was clicked")
        }
    }
    SequentialAnimation{
        id: pressedAnimation
        ColorAnimation {
            from: Qt.darker("#145235")
            to: Qt.darker(Qt.darker("#145235"))
            duration: 200
        }
        ColorAnimation {
            from: Qt.darker(Qt.darker("#145235"))
            to: Qt.darker("#145235")
            duration: 200
        }
    }
}



