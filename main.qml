import QtQuick
import QtQml
import QtQuick.Layouts
import QtQuick.Controls

ApplicationWindow {
    id: root
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    visible: true
    title: qsTr("Image Compare")
    color: "black"

    footer:     Rectangle {
        id:recordBtn
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: !isCaptureStarted  ? "green": "red"
        width: 60
        height: 60
        radius: 30
        Text {
            width: paintedWidth
            height: 30
            anchors.centerIn: parent
            text: !isCaptureStarted  ? "start": "stop"
        }

        MouseArea {
            anchors.fill: parent
            onClicked:  {
                isCaptureStarted = !isCaptureStarted
                isCaptureStarted ? screenCaptureController.startCapture() : screenCaptureController.stopCapture()
            }
        }
    }

    property bool isCaptureStarted: false

    Rectangle {
        anchors.top: parent.top
        color:"yellow"
        anchors.topMargin: 15
        anchors.bottom: recordBtn.top
        anchors.bottomMargin: 15
        anchors.left:parent.left
        anchors.right: parent.right
        width: parent.width
        height: parent.height
        anchors.leftMargin: 30
        anchors.rightMargin: 30
        GridView {
            id:grid
            width: parent.width
            height: parent.height
            model:screenshotsModel
            anchors.left: parent.left
            anchors.leftMargin: 5
            clip: true
            cellWidth:930
            cellHeight: 930
            delegate: Image {
                width: 920
                height: 920
                asynchronous: true
                source: screenshot
            }
        }
    }
}
