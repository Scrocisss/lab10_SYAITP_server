#include "server.h"
#include <QDataStream>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    if (!listen(QHostAddress::Any, 1234)) {
        qDebug() << "Unable to start the server";
    } else {
        qDebug() << "Server started";
    }
}
void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *client = new QTcpSocket;
    client->setSocketDescriptor(socketDescriptor);
    connect(client, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &Server::onDisconnected);
    clients << client;


    connectionCounter++;
    updateClients();
}
void Server::onReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    QDataStream in(client);
    in.setVersion(QDataStream::Qt_5_15);

    while (!in.atEnd()) {
        QByteArray messageData;
        in >> messageData;
        QString message = QString::fromUtf8(messageData);

        QString clientMessage = "Клиент №" + QString::number(clients.indexOf(client) + 1) + ": " + message;
        emit messageReceived(clientMessage);

        for (QTcpSocket *otherClient : clients) {
            if (otherClient != client) {
                QDataStream out(otherClient);
                out.setVersion(QDataStream::Qt_5_15);
                out << clientMessage.toUtf8();
            }
        }
    }
}





void Server::onDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    clients.removeOne(client);
    client->deleteLater();
    updateClients();
}
void Server::updateClients()
{
    QStringList clientList;
    int i = 1;
    for (QTcpSocket *client : clients) {
        clientList << "Клиент №" + QString::number(i);
        i++;
    }
    emit clientsUpdated(clientList);
}
QVariantList Server::getClientsList() const
{
    QVariantList clientList;
    int i = 1;
    for (QTcpSocket *client : clients) {
        clientList.append("Клиент №" + QString::number(i));
        i++;
    }
    return clientList;
}
void Server::sendMessageToClients(const QString& message)
{
    for (QTcpSocket* client : clients) {
        QDataStream out(client);
        out.setVersion(QDataStream::Qt_5_15);
        out << message.toUtf8();
    }
}
