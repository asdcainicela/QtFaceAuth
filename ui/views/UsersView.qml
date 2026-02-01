import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: usersPage
    
    background: Rectangle { color: "#121212" }
    
    header: ToolBar {
        background: Rectangle { color: "#1f1f1f" }
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            
            ToolButton {
                text: "←"
                font.pixelSize: 20
                contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment:Text.AlignHCenter; verticalAlignment:Text.AlignVCenter }
                background: null
                onClicked: usersPage.StackView.view.pop()
            }
            
            Label {
                text: "User Management"
                font.pixelSize: 20
                font.bold: true
                color: "white"
                Layout.fillWidth: true
            }
            
            Button {
                text: "+ Add User"
                background: Rectangle { color: "#00d2ff"; radius: 4 }
                contentItem: Text { text: parent.text; color: "white"; font.bold: true; horizontalAlignment:Text.AlignHCenter; verticalAlignment:Text.AlignVCenter }
                onClicked: console.log("Open Add User Dialog") // TODO: Open Dialog
            }
        }
    }

    ListView {
        id: usersList
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10
        clip: true
        
        model: ListModel { id: usersModel }

        Component.onCompleted: loadUsers()
        
        function loadUsers() {
            var users = databaseManager.getAllUsers()
            usersModel.clear()
            for (var i = 0; i < users.length; i++) {
                usersModel.append(users[i])
            }
        }
        
        delegate: Rectangle {
            width: usersList.width
            height: 70
            color: "#1e1e1e"
            radius: 8
            border.color: "#333"
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15
                
                Rectangle {
                    width: 40; height: 40
                    radius: 20
                    color: model.has_biometrics ? "#00ff00" : "#555"
                    border.width: 1
                    border.color: "#333"
                    
                    Text {
                        text: model.full_name.charAt(0)
                        anchors.centerIn: parent
                        color: "black"
                        font.bold: true
                    }
                }
                
                ColumnLayout {
                    Layout.fillWidth: true
                    Text { text: model.full_name; color: "white"; font.bold: true; font.pixelSize: 16 }
                    Text { text: "@" + model.username + " • " + model.role; color: "#aaa"; font.pixelSize: 12 }
                }
                
                Label {
                    text: model.is_active ? "Active" : "Inactive"
                    color: model.is_active ? "#00ff00" : "red"
                    font.pixelSize: 12
                }
                
                Button {
                    text: "Edit"
                    flat: true
                    contentItem: Text { text: parent.text; color: "#00d2ff" }
                }
            }
        }
        

    }
}
