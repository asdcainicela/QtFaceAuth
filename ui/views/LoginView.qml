import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: loginPage
    background: Rectangle { color: "#1e1e1e" }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20
        width: 300

        Label {
            text: "QtFaceAuth"
            font.pixelSize: 32
            font.bold: true
            color: "white"
            Layout.alignment: Qt.AlignHCenter
        }

        TextField {
            id: usernameField
            placeholderText: "Username"
            Layout.fillWidth: true
            color: "black"
            palette.base: "white"
        }

        TextField {
            id: passwordField
            placeholderText: "Password"
            echoMode: TextInput.Password
            Layout.fillWidth: true
            color: "black"
            palette.base: "white"
            onAccepted: loginBtn.clicked()
        }

        Label {
            id: errorLabel
            text: ""
            color: "#ff5555"
            visible: text !== ""
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            id: loginBtn
            text: "Login"
            Layout.fillWidth: true
            highlighted: true
            onClicked: {
                errorLabel.text = ""
                // Call C++ Backend
                if (!userManager.login(usernameField.text, passwordField.text)) {
                    // Logic handles signal, but we can set error here too if sync
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
