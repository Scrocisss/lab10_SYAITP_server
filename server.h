#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include <QTcpSocket>
class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    QVariantList getClientsList() const;
public slots:
    void sendMessageToClients(const QString& message);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
private slots:
    void onReadyRead();
    void onDisconnected();
    void updateClients();
private:
    QList<QTcpSocket*> clients;
    int connectionCounter = 0;  // новый счетчик подключений
    int clientCounter = 1;
signals:
    void messageReceived(const QString &message);
    void clientsUpdated(const QStringList &clients);


};

#endif // SERVER_H
