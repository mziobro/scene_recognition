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
    signal open()

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
                console.log("Triggered")
                image.source =  "image://openCVimageCapture/test" + name.toString()
                name = name + 1
            }
            running: false
            repeat: true
        }
    }


    Text {
        text: area.text
        width: 130
        height: 50
        font.pixelSize: 25
        color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: image.bottom
        anchors.topMargin: 20
    }

    CustomButton {
        id: startStopButton
        anchors.top: image.top
        anchors.right: parent.right
        anchors.rightMargin: 20
        text: "Start"
        onClicked: {

            if (state === "NotRunning")
            {
                console.log("Start clicked")
                videoWriter.start_recording(area.text)
                state = "Running"
            }
            else
            {
                console.log("Stop clicked")
                videoWriter.stop_recording()
                state = "NotRunning"

            }
        }
        state: "NotRunning"

        states: [
            State {
                name: "Running"
                PropertyChanges {
                    target: startStopButton
                    text: "Stop"

                }
            },
            State {
                name: "NotRunning"
                PropertyChanges {
                    target: startStopButton
                    text: "Start"

                }
            }
        ]
    }

    CustomButton {
        text: "Dodaj"
        anchors.top: startStopButton.bottom
        anchors.topMargin: 20
        anchors.right: startStopButton.right
        onClicked: {
            open()
            area.visible = true
        }
    }

    CustomButton {
        id: back
        text: "Powrót"
        anchors {
            bottom: background.bottom
            right: background.right
            bottomMargin: 10
            rightMargin: 10
        }
        onClicked: background.back()
    }


    CustomButton {
        id: calibrate
        text: "Kalibracja"
        anchors {
            bottom: back.top
            right: back.right
            bottomMargin: 10
            rightMargin: 10
        }
        onClicked: {
            calibrator.calibrate()
            popupText.text = "Trwa kalibracja,\n proszę czekać"
            area.visible = false
            busy.visible = true
            calibratingInprogressPopup.open()
            okButton.visible = false
            console.log('calibratong')
        }
    }

    Popup {
        id: calibratingInprogressPopup
        modal: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnEnter
        width: background.width/2
        height: background.height/3
        y: background.height/2 - background.height/3
        x: background.width/2 - background.width/4

        Column{
            anchors.fill: parent

            Text {
                id: popupText
                font.pixelSize: 30
                text: "Trwa kalibracja,\n proszę czekać"
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }

            BusyIndicator {
                id: busy
                running: calibratingInprogressPopup.opened
                visible: false
                Layout.alignment: Qt.AlignHCenter
            }

            TextInput{
                id: area
                visible: false
                cursorVisible: true
                text: "Pomieszczenie"
                width: 250
                height: 50
                font.pixelSize: 25
                color: "white"
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Qt.AlignRight
            }

            CustomButton {
                id: okButton
                visible: true
                text: "OK"
                onClicked: {
                    calibratingInprogressPopup.close();
                }
            }
        }


        background: Rectangle {
            color: Qt.darker(Qt.darker("darkgray"))
            radius: 5
        }

        Connections{
            target: calibrator
            onCalibrationEnd:
            {
                okButton.visible = true
                popupText.text =  "Zakończono kalibrację";
            }
        }
    }


    onOpen: {
        videoWriter.run()
        popupText.text = "Wprowadź nazwę pomieszczenia: "
        area.visible = true
        calibratingInprogressPopup.open()
    }

}
