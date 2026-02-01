import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Page {
    id: dashboardPage
    
    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#141e30" }
            GradientStop { position: 1.0; color: "#243b55" }
        }
    }

    header: Rectangle {
        height: 60
        color: Qt.rgba(0,0,0,0.5)
        
        // Bottom Border Line
        Rectangle {
            width: parent.width
            height: 1
            color: "#333"
            anchors.bottom: parent.bottom
        }
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: 15
            
            Label {
                text: "QtFaceAuth Command Center"
                font.bold: true
                font.pixelSize: 18
                color: "white"
                Layout.fillWidth: true
            }
            
            Label {
                text: userManager.currentUser.full_name + " (" + userManager.currentUser.role + ")"
                color: "#00d2ff"
                font.pixelSize: 14
            }
            
            ToolButton {
                text: "Logout"
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    font.bold: true
                }
                background: Rectangle {
                    color: parent.pressed ? "#cc2222" : "#ff3333"
                    radius: 4
                }
                onClicked: userManager.logout()
            }
        }
    }

    GridView {
        id: grid
        anchors.fill: parent
        anchors.margins: 20
        cellWidth: 220
        cellHeight: 180
        
        model: ListModel {
            ListElement { name: "Users"; icon: "👤"; action: "users"; adminOnly: true }
            ListElement { name: "Access Logs"; icon: "📜"; action: "logs"; adminOnly: false }
            ListElement { name: "System Config"; icon: "⚙️"; action: "config"; adminOnly: true }
            ListElement { name: "Camera Feed"; icon: "📹"; action: "camera"; adminOnly: false }
        }

        delegate: Item {
            width: grid.cellWidth
            height: grid.cellHeight
            visible: !model.adminOnly || userManager.currentUser.role === "admin"

            Rectangle {
                anchors.fill: parent
                anchors.margins: 10
                color: Qt.rgba(1, 1, 1, 0.05)
                radius: 12
                border.color: hoverHandler.hovered ? "#00d2ff" : "transparent"
                border.width: 1
                
                scale: hoverHandler.hovered ? 1.05 : 1.0
                Behavior on scale { NumberAnimation { duration: 150 } }

                ColumnLayout {
                    anchors.centerIn: parent
                    
                    Text {
                        text: model.icon
                        font.pixelSize: 48
                        Layout.alignment: Qt.AlignHCenter
                        color: "white"
                    }
                    
                    Label {
                        text: model.name
                        font.bold: true
                        font.pixelSize: 16
                        color: "white"
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
                
                HoverHandler { id: hoverHandler }
                TapHandler {
                    onTapped: console.log("Clicked:", model.action)
                }
            }
        }
    }
}
