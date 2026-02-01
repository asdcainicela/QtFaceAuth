import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

Page {
    id: enrollmentPage

    // Signal to notify Main.qml
    signal enrolled()

    Timer {
        id: timer
        interval: 1500
        onTriggered: {
            console.log("Emitting enrolled signal...")
            enrollmentPage.enrolled()
        }
    }
    background: Rectangle { color: "#121212" }

    MediaDevices {
        id: mediaDevices
    }

    CaptureSession {
        id: captureSession
        camera: Camera {
            id: camera
            cameraDevice: mediaDevices.defaultVideoInput
            focusMode: Camera.FocusModeAuto
        }
        imageCapture: ImageCapture {
            id: imageCapture
            onImageSaved: (id, path) => {
                console.log("Image Saved at:", path);
                // Call C++ to process
                if (userManager.registerFace(userManager.currentUser.id, path)) {
                    statusLabel.text = "Success! Face Registered.";
                    statusLabel.color = "#00ff00";
                    // Proceed to Dashboard after delay
                    timer.start();
                } else {
                    statusLabel.text = "Failed to extract face features.";
                }
            }
        }
        videoOutput: videoOutput
    }
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 20
        anchors.margins: 40

        Label {
            text: "Setup Face Authentication"
            font.pixelSize: 28
            color: "white"
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "Position your face in the center"
            color: "#aaaaaa"
            Layout.alignment: Qt.AlignHCenter
        }

        // Camera Preview Frame
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "black"
            border.color: statusLabel.text === "" ? "#00d2ff" : (statusLabel.text.includes("Success") ? "#00ff00" : "#ff0000")
            border.width: 3
            radius: 12
            clip: true

            VideoOutput {
                id: videoOutput
                anchors.fill: parent
                fillMode: VideoOutput.PreserveAspectCrop
            }
            
            // Modern Scanning Overlay
            Item {
                anchors.fill: parent
                
                // Corner Markers
                Rectangle { width: 30; height: 5; color: "#00d2ff"; x: 20; y: 20 }
                Rectangle { width: 5; height: 30; color: "#00d2ff"; x: 20; y: 20 }
                
                Rectangle { width: 30; height: 5; color: "#00d2ff"; x: parent.width-50; y: 20 }
                Rectangle { width: 5; height: 30; color: "#00d2ff"; x: parent.width-25; y: 20 }
                
                Rectangle { width: 30; height: 5; color: "#00d2ff"; x: 20; y: parent.height-25 }
                Rectangle { width: 5; height: 30; color: "#00d2ff"; x: 20; y: parent.height-50 }
                
                Rectangle { width: 30; height: 5; color: "#00d2ff"; x: parent.width-50; y: parent.height-25 }
                Rectangle { width: 5; height: 30; color: "#00d2ff"; x: parent.width-25; y: parent.height-50 }

                // Scan Line
                Rectangle {
                    id: scanLine
                    width: parent.width; height: 2; color: "#00ff00"; opacity: 0.7
                    
                    SequentialAnimation on y {
                        loops: Animation.Infinite
                        running: true
                        NumberAnimation { from: 0; to: 200; duration: 2000; easing.type: Easing.InOutQuad }
                        NumberAnimation { from: 200; to: 0; duration: 2000; easing.type: Easing.InOutQuad }
                    }
                }
            }
        }

        Label {
            id: statusLabel
            text: ""
            color: "red"
            font.pixelSize: 16
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            text: "CAPTURE & REGISTER"
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            highlighted: true
            
            background: Rectangle {
                color: parent.pressed ? "#0099bb" : "#00d2ff"
                radius: 8
            }
            contentItem: Text {
                text: parent.text
                font.bold: true
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                camera.start() // Ensure started
                imageCapture.captureToFile(""); 
            }
        }
    }
    
    Component.onCompleted: camera.start()
}
