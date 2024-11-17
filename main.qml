// Main.qml
import QtQuick 2.14
import QtQuick.Controls 2.14
import CustomComponents 1.0

ApplicationWindow {
    visible: true
    width: 1080
    height: 720

    Rectangle {
        id: rect
        width: 800
        height: 600
        anchors.centerIn: parent
        color: "red"

        EmbeddedApplication {
            id: embeddedApp
        }

        Component.onCompleted: {
            embeddedApp.launchAndEmbed(EXE_PATH);
        }
    }

    Button {
        text: "Launch and Embed Application"
        anchors.top: rect.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        // onClicked: embeddedApp.launchAndEmbed(EXE_PATH);
    }
}
