import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Effects

Page {
    id: loginPage
    
    property bool faceMode: true
    
    Component.onCompleted: {
        if (faceMode) camera.start()
    }

    MediaDevices { id: mediaDevices }
    CaptureSession {
        id: captureSession
        camera: Camera { 
            id: camera
            active: faceMode 
        }
        imageCapture: ImageCapture {
            id: imageCapture
            onImageSaved: (id, path) => {
                console.log("Login capture saved:", path)
                statusLabel.text = "Verifying..."
                statusLabel.color = "yellow"
                
                if (userManager.authenticateWithFace(path)) {
                    statusLabel.text = "Face Recognized! Logging in..."
                    statusLabel.color = "#00ff00"
                } else {
                    statusLabel.text = "Face not recognized. Try again."
                    statusLabel.color = "red"
                }
            }
        }
        videoOutput: videoOutput
    }

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

            // 1. Header
            ColumnLayout {
                spacing: 5
                Layout.alignment: Qt.AlignHCenter
                
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
                }
            }

            // 2. Mode Switcher
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 10
                
                Button {
                    text: "Face ID"
                    highlighted: faceMode
                    onClicked: { faceMode = true; camera.start() }
                    background: Rectangle {
                        color: faceMode ? "#00d2ff" : "transparent"
                        border.color: "#00d2ff"
                        radius: 4
                    }
                    contentItem: Text { text: parent.text; color: faceMode?"white":"#00d2ff"; horizontalAlignment:Text.AlignHCenter; verticalAlignment:Text.AlignVCenter }
                }

                Button {
                    text: "Password"
                    highlighted: !faceMode
                    onClicked: { faceMode = false; camera.stop() }
                    background: Rectangle {
                        color: !faceMode ? "#00d2ff" : "transparent"
                        border.color: "#00d2ff"
                        radius: 4
                    }
                    contentItem: Text { text: parent.text; color: !faceMode?"white":"#00d2ff"; horizontalAlignment:Text.AlignHCenter; verticalAlignment:Text.AlignVCenter }
                }
            }

            // 3. Face Login Area
            ColumnLayout {
                visible: faceMode
                Layout.fillWidth: true
                spacing: 15

                Rectangle {
                    Layout.fillWidth: true
                    height: 200
                    color: "black"
                    border.color: "#333"
                    radius: 8
                    clip: true

                    VideoOutput {
                        id: videoOutput
                        anchors.fill: parent
                        fillMode: VideoOutput.PreserveAspectCrop
                    }

                    Rectangle {
                        width: parent.width; height: 2; color: "#00ff00"; opacity: 0.7
                        visible: faceMode
                        SequentialAnimation on y {
                            loops: Animation.Infinite; running: faceMode
                            NumberAnimation { from: 0; to: 200; duration: 1500 }
                            NumberAnimation { from: 200; to: 0; duration: 1500 }
                        }
                    }
                }
                
                Button {
                    text: "SCAN FACE"
                    Layout.fillWidth: true
                    background: Rectangle { color: "#00d2ff"; radius: 4 }
                    contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment:Text.AlignHCenter; verticalAlignment:Text.AlignVCenter }
                    onClicked: imageCapture.captureToFile("")
                }
            }

            // 4. Password Login Area
            ColumnLayout {
                visible: !faceMode
                Layout.fillWidth: true
                spacing: 15
                
                TextField {
                    id: usernameField
                    placeholderText: "Username"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 45
                    color: "white"
                    background: Rectangle {
                        color: "transparent"; border.color: parent.activeFocus ? "#00d2ff" : "#555"; border.width: 1; radius: 4
                    }
                }

                TextField {
                    id: passwordField
                    placeholderText: "Password"
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                    color: "white"
                    background: Rectangle {
                        color: "transparent"; border.color: parent.activeFocus ? "#00d2ff" : "#555"; border.width: 1; radius: 4
                    }
                    onAccepted: loginButton.clicked()
                }

                Button {
                    id: loginButton
                    text: "LOGIN"
                    Layout.fillWidth: true
                    height: 40
                    background: Rectangle { color: parent.pressed ? "#0099bb" : "#00d2ff"; radius: 4 }
                    contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment:Text.AlignHCenter; verticalAlignment:Text.AlignVCenter }
                    onClicked: {
                        if (userManager.login(usernameField.text, passwordField.text)) {
                            statusLabel.text = "Login Successful"; statusLabel.color = "#00ff00";
                        } else {
                            statusLabel.text = "Invalid credentials"; statusLabel.color = "red";
                        }
                    }
                }
            }

            // 5. Footer Status
            Label {
                id: statusLabel
                text: ""
                color: "red"
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 14
            }
        }
    }

    Connections {
        target: userManager
        function onLoginFailed(reason) {
            statusLabel.text = reason
            statusLabel.color = "red"
        }
    }
}
