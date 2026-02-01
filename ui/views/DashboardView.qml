import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: dashboardPage
    background: Rectangle { color: "#2d2d2d" }
    
    header: ToolBar {
        Label {
            text: "Dashboard - " + userManager.currentUser.full_name
            anchors.centerIn: parent
        }
        ToolButton {
            text: "Logout"
            anchors.right: parent.right
            onClicked: userManager.logout()
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20

        Label {
            text: "Welcome, " + userManager.currentUser.role
            font.pixelSize: 24
            color: "white"
        }

        // Placeholder for future User Management Button
        Button {
            text: "Manage Users"
            visible: userManager.currentUser.role === "admin"
            onClicked: console.log("Go to Users View")
        }
    }
}
