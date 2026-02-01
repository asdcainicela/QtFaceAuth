import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects 

Page {
    id: loginPage
    
    // Background Gradient
    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#0f0c29" }
            GradientStop { position: 0.5; color: "#302b63" }
            GradientStop { position: 1.0; color: "#24243e" }
        }
    }

    Rectangle {
        id: loginCard
        width: 360
        height: 480
        anchors.centerIn: parent
        color: Qt.rgba(1, 1, 1, 0.05)
        radius: 16
        border.color: Qt.rgba(1, 1, 1, 0.1)
        border.width: 1

        ColumnLayout {
            anchors.centerIn: parent
            width: parent.width * 0.8
            spacing: 20

            // Logo / Title
            Label {
                text: "QtFaceAuth"
                font.pixelSize: 32
                font.bold: true
                color: "#ffffff"
                Layout.alignment: Qt.AlignHCenter
            }
            
            Label {
                text: "Secure Access Control"
                font.pixelSize: 14
                color: "#aaaaaa"
                Layout.alignment: Qt.AlignHCenter
                Layout.bottomMargin: 20
            }

            // Username Input
            TextField {
                id: usernameField
                placeholderText: "Username"
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                color: "white"
                font.pixelSize: 15
                
                background: Rectangle {
                    color: Qt.rgba(0, 0, 0, 0.3)
                    radius: 8
                    border.color: usernameField.activeFocus ? "#00d2ff" : "transparent"
                    border.width: 1
                }
                padding: 12
            }

            // Password Input
            TextField {
                id: passwordField
                placeholderText: "Password"
                echoMode: TextInput.Password
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                color: "white"
                font.pixelSize: 15
                
                background: Rectangle {
                    color: Qt.rgba(0, 0, 0, 0.3)
                    radius: 8
                    border.color: passwordField.activeFocus ? "#00d2ff" : "transparent"
                    border.width: 1
                }
                padding: 12
                onAccepted: loginBtn.clicked()
            }

            Label {
                id: errorLabel
                text: ""
                color: "#ff5555"
                font.pixelSize: 13
                visible: text !== ""
                Layout.alignment: Qt.AlignHCenter
            }

            // Login Button
            Button {
                id: loginBtn
                text: "LOGIN"
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                Layout.topMargin: 10
                
                contentItem: Text {
                    text: loginBtn.text
                    font.bold: true
                    font.pixelSize: 14
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: loginBtn.pressed ? "#0099bb" : "#00d2ff"
                    radius: 8
                }

                onClicked: {
                    errorLabel.text = ""
                    if (!userManager.login(usernameField.text, passwordField.text)) {
                        // Error handled by signal
                    }
                }
            }
        }
    }
    
    Connections {
        target: userManager
        function onLoginFailed(reason) {
            errorLabel.text = reason
        }
    }
}
