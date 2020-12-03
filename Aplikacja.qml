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
    signal processingDone()
    signal open()

    onOpen: {
        videoWriter.recognize()
    }
                property int name: 1

    Image {
        id: image
        anchors.verticalCenter: parent.verticalCenter
        objectName: "img"
        anchors.centerIn: parent
        asynchronous : true
        source: "image://openCVimageCapture/run"

        Connections {
            target: videoWriter
            onProcessingDone: {
                image.source =  "image://openCVimageCapture/run" + name.toString()
                name = name + 1
                videoWriter.recognize()
            }
            onNew_desc: {
                console.log("New desc!")
                recognizedRoom.text = videoWriter.desc
            }
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
