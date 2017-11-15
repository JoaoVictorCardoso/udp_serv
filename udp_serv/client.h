#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

class QUdpSocket;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    QUdpSocket *clientSocket;    //CRIA NOSSO CLIENTE VIRTUAL

signals:

public slots:


};

#endif // CLIENT_H
