import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

ApplicationWindow {
    id: window
    visible: true
    width: 1280
    height: 800
    title: "QtFaceAuth"
    color: "#222" // Dark background by default

    // System Properties
    property bool isDarkTheme: true
    property bool isVertical: width < height

    // Header
    Rectangle {
        id: header
        height: 60
        width: parent.width
        color: "#111"
        z: 10

        Text {
            text: "QtFaceAuth System"
            color: "#FFF"
            font.pixelSize: 20
            font.bold: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
        }

        Text {
            text: new Date().toLocaleTimeString()
            color: "#888"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
        }
    }

    // Main Layout (Row for Horizontal, Column for Vertical)
    RowLayout {
        anchors.top: header.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 0

        // Sidebar Navigation
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 80
            color: "#1a1a1a"

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                // Quick Repeater for Menu Items
                Repeater {
                    model: [
                        {icon: "📷", label: "Scan", index: 0},
                        {icon: "👥", label: "Users", index: 1},
                        {icon: "📜", label: "Logs", index: 2},
                        {icon: "⚙️", label: "Config", index: 3}
                    ]

                    Rectangle {
                        Layout.preferredWidth: 60
                        Layout.preferredHeight: 60
                        Layout.alignment: Qt.AlignHCenter
                        color: stackView.currentIndex === modelData.index ? "#333" : "transparent"
                        radius: 8

                        Text {
                            anchors.centerIn: parent
                            text: modelData.icon
                            font.pixelSize: 24
                        }
                        
                        MouseArea {
                            anchors.fill: parent
                            onClicked: stackView.currentIndex = modelData.index
                        }
                    }
                }
                
                Item { Layout.fillHeight: true } // Spacer
            }
        }

        // Content Area
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#000"

            StackLayout {
                id: stackView
                anchors.fill: parent
                currentIndex: 0
                
                // Index 0: Dashboard (Camera)
                Item {
                    Text {
                        text: "🎥 CAMERA FEED PLACEHOLDER"
                        color: "#0F0"
                        anchors.centerIn: parent
                        font.pixelSize: 24
                    }
                    
                    Rectangle {
                        width: 300
                        height: 300
                        anchors.centerIn: parent
                        color: "transparent"
                        border.color: "#0F0"
                        border.width: 2
                    }
                }

                // Index 1: Users
                Item {
                    Text { text: "Users View"; color: "white"; anchors.centerIn: parent }
                }

                // Index 2: Logs
                Item {
                    Text { text: "Logs View"; color: "white"; anchors.centerIn: parent }
                }

                // Index 3: Config
                Item {
                    Text { text: "Settings View"; color: "white"; anchors.centerIn: parent }
                }
            }
        }
    }
}
