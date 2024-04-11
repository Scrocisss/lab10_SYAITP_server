import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Controls.Material 2.15

ApplicationWindow {
    visible: true
    width: 600
    height: 800
    title: "Сервер чата"

    ColumnLayout {
        anchors.fill: parent
        spacing: 10

        ListView {
            id: messageList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: dataModel
            delegate: Text {
                text: modelData
                wrapMode: Text.WordWrap
            }
        }

        Rectangle {
            id: separator
            Layout.fillWidth: true
            height: 2
            color: "lightgrey"
        }

        Label {
            text: "Подключенные клиенты:"
            font.bold: true
            Layout.fillWidth: true
            padding: 10
        }

        ListView {
            id: clientList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: server.getClientsList()
            delegate: Text {
                text: modelData
                wrapMode: Text.WordWrap
            }
        }

        TextField {
            id: messageInput
            placeholderText: "Введите сообщение"
            Layout.fillWidth: true
            onAccepted: {
                var message = messageInput.text.trim()
                if (message !== "") {
                    server.sendMessageToClients(message)
                    messageInput.text = ""
                }
            }
        }

        Button {
            id: sendButton
            text: "Отправить"
            Layout.fillWidth: true
            onClicked: {
                var message = messageInput.text.trim()
                if (message !== "") {
                    server.sendMessageToClients(message)
                    messageInput.text = ""
                }
            }
        }
    }

    ListModel {
        id: dataModel
    }

    Connections {
        target: server
        function onMessageReceived(message) {
            dataModel.append({ "modelData": message })
            messageList.positionViewAtEnd()
        }
        function onClientsUpdated(clients) {
            clientList.model = clients;
        }
    }
}
