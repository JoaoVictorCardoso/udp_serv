#include "client.h"
#include <QUdpSocket>
#include "server.h"

Client::Client(QObject *parent) : QObject(parent)
{
    clientSocket = new QUdpSocket(this);        //INSTANCIAMOS O NOSSO CLIENTE
}
