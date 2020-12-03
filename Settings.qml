import QtQuick 2.0
 import QtQuick.Controls 2.2
 import QtQuick.Layouts 1.2

     Rectangle {

        width: 1080
        height: 720
        color: "#145235"

        id: root
        signal back()
    ColumnLayout
    {
        anchors.centerIn:  parent
        width: parent.width
        height: parent.height/2

        Row {

            Text {
                font.pixelSize: 25
                color: "white"
                text: "Źródło video: "
            }



        TextInput {
            id: source
            cursorVisible: true
            text: config.video_path
            width: 250
            height: 50
            font.pixelSize: 25
            color: "white"
//            horizontalAlignment: Qt.AlignRight
        }


        Layout.alignment: Qt.AlignHCenter

}

        Row {


            Text {
                font.pixelSize: 25
                color: "white"
                text: "Plik z wagami: "
            }

        TextInput {
            id: weights
            cursorVisible: true
            text: config.weights_path
            width: 250
            height: 50
            font.pixelSize: 25
            color: "white"
        }
        Layout.alignment: Qt.AlignHCenter

        }

    Row{

        Text {
            font.pixelSize: 25
            color: "white"
            text: "Plik konfiguracyjny: "
        }


        TextInput{
            id: config
            cursorVisible: true
            text: config.cfg_path
            width: 250
            height: 50
            font.pixelSize: 25
            color: "white"
        }
        Layout.alignment: Qt.AlignHCenter


    }

    CustomButton {
        text: "Zapisz"

        Layout.alignment: Qt.AlignHCenter

        onClicked: {
            config.save()
        }
    }

    }


    CustomButton {
        id: back
        text: "Powrót"
        anchors {
            bottom: root.bottom
            right: root.right
            bottomMargin: 10
            rightMargin: 10
        }
        onClicked: root.back()
    }
}
