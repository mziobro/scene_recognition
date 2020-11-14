import QtQuick 2.0
import QtMultimedia 5.9
import QtQuick.Layouts 1.3
 import QtQuick.Controls 2.2

Rectangle {
    id: background
    color: "#145235"
    width: 1080
    height: 720
    signal back()
    property alias calibTimer : timer

    Image{
        id: image

        anchors.verticalCenter: parent.verticalCenter
        objectName: "img"
        anchors.centerIn: parent
        source: "image://openCVimageCapture/test"
        asynchronous : true

        Timer{
            id: timer
            interval: 3000
            property int name: 1
            onTriggered: {
//                console.log("Triggered")
//                image.source =  "image://openCVimageCapture/test" + name.toString()
//                name = name + 1
            }
            running: false
            repeat: true
        }
    }

    CustomButton {
        id: button1
        anchors.top: image.top
        anchors.right: parent.right
        anchors.rightMargin: 20
        text: "Start"
        onClicked: {
            color = Qt.darker(Qt.darker("#145235"))
            console.log("Start clicked")
            videoWriter.record(area.text)
        }
    }
    CustomButton {
        id: button2
        text: "Stop"
        anchors.top: button1.bottom
        anchors.topMargin: 10
        anchors.left: button1.left
        anchors.leftMargin: 0
        onClicked: {
            button2.animation.start()
            button1.color = Qt.darker("#145235")
            console.log("Stop clicked")
            videoWriter.stop()
        }
    }
    TextInput{
        id: area
        cursorVisible: true
        anchors.top: button2.bottom
        anchors.left: button2.left
        anchors.leftMargin: 20
        text: "Pomieszczenie"
        width: 130
        height: 50
        font.pixelSize: 25
        color: "white"
        horizontalAlignment: TextInput.AlignHCenter
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
    Connections {
        target: videoWriter
        onStartCalibrate: calibratingInprogressPopup.open()
        onEndCalibrate: calibratingInprogressPopup.close()
    }


    Popup {
        id: calibratingInprogressPopup
        modal: true
        closePolicy: Popup.CloseOnEscape
        width: background.width/2
        height: background.height/3
        y: background.height/2 - background.height/3
        x: background.width/2 - background.width/4

//        Column{
//            height: parent.height
//            anchors.verticalCenter: parent.verticalCenter
//        BusyIndicator {
//            id: busy
//            running: calibratingInprogressPopup.opened
//            Layout.alignment: Qt.AlignHCenter
//        }
        Text {
            font.pixelSize: 30
            text: "Trwa kalibracja,\n proszę czekać"
            color: "white"
            anchors.centerIn: parent
        }
//        }

        background: Rectangle {

            color: Qt.darker(Qt.darker("darkgray"))
            radius: 5
        }

    }
}
