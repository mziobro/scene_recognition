import QtQuick 2.0
import QtQuick 2.0
import QtMultimedia 5.9
import QtQuick.Layouts 1.3


Rectangle {
    id: background
    color: "#145235"
    width: 1080
    height: 720
    signal back()
    property alias imageTimer: timer
    Image {
        id: image
        anchors.verticalCenter: parent.verticalCenter
        objectName: "img"
        anchors.centerIn: parent
//        source: "image://openCVimageCapture/run"
        asynchronous : true

        Timer{
            id: timer
            interval: 3000
            property int name: 1
            onTriggered: {
                console.log("Triggered")
//                image.source =  "image://openCVimageCapture/run" + name.toString()
//                name = name + 1
                recognizedRoom.text = videoWriter.recognize();
            }
            running: false
            repeat: true
        }
    }


    Text {
        id: recognizedRoom
        anchors.top: image.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: image.horizontalCenter
        font {
            pixelSize: 40
        }
        color: "white"
        text: "Pomieszczenie"
    }

    CustomButton {
        id: back
        text: "Powrót"
        anchors {
            bottom: background. bottom
            right: background.right
            bottomMargin: 10
            rightMargin: 10
        }
        onClicked: background.back()
    }
}
