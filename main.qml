import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    id: root
    visible: true

    width: 1080
    height: 720
    title: qsTr("Aplikacja Magisterska")
    color: "#145235"

    Rectangle{
        id: menuu
        color: "#145235"
        height: 720
        width: 1080

    ListView {
        id: menu
        anchors.centerIn: parent
        width: parent.width/2
        height: 230
        focus: true
        model: model
        spacing: 10

        ListModel{
            id: model

            ListElement{
                textt: "Kalibracja"
            }
            ListElement{
                textt: "Start aplikacji"
            }
//            ListElement{
//                textt: "Ustawienia"
//            }
            ListElement{
                textt: "Wyjście"
            }
        }
        delegate: menuDelegate
        highlight: Rectangle {
            color: Qt.lighter("#145235")
            width: parent.width
            height: 50
        }
    }

    }
    Kalibrator {
        id: kalibracja
        onBack: {
            calibTimer.stop()
            stack.replace(menuu)
            videoWriter.stop = true
    }}

    Aplikacja{
        id: aplikacja
        visible: false
        onBack: {
            stack.replace(menuu)
            console.log("Replacing with menu")
        }
    }

//    Settings {
//        id: settings
//        onBack: {
//            stack.replace(menuu)
//        }
//    }

    StackView{
        id: stack
        initialItem: menuu
        anchors.fill: parent
    }




    Component {
        id: menuDelegate
        Rectangle{
            width: root.width/2
            height: 50
            color: Qt.darker("#145235")
            radius: 4
            Text {
                anchors.centerIn: parent
                text: textt;
                font.pixelSize: 24
                color: "white"
            }

            MouseArea{
                anchors.fill: parent
                onClicked:{
                    if(textt == "Kalibracja")
                    {
                        kalibracja.open()
                        stack.replace(kalibracja)
                        kalibracja.calibTimer.start()
                    }
                    else if (textt == "Start aplikacji")
                    {
                        kalibracja.calibTimer.stop()
                        stack.replace(aplikacja)
                        aplikacja.open()
                    }
                    else if (textt == "Wyjście"){
                        Qt.quit()
                    }
                }
            }
        }
    }
}
