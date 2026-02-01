import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "views"

ApplicationWindow {
    id: window
    width: 1280
    height: 720
    visible: true
    title: "QtFaceAuth - Security System"
    color: "#121212"

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "views/LoginView.qml"
    }

    Connections {
        target: userManager
        function onSessionChanged() {
            if (userManager.isLoggedIn) {
                stackView.replace(null, "views/DashboardView.qml")
            } else {
                stackView.replace(null, "views/LoginView.qml")
            }
        }
    }
}
