#include "server.h"
#include "ui_server.h"
#include <QUdpSocket>
#include "client.h"

Client cliente;

Server::Server(QWidget *parent) :   QWidget(parent),    ui(new Ui::Server)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);                               //instancia o unico socket necessário para ser server
    ipcliente = QHostAddress("192.168.1.2");
    if(data.length()>8192)
    {
        pegaEpassa(data);//passa o PRIMEIRO PACOTE grande.
    }
    if (iniciado){//inciado é variavel assistente e define se foi a intanciação do objeto que criou essa chamada

        escreve();
        cliente.clientSocket->readDatagram(datagram.data(), datagram.size(), &ipcliente, &porta);
        ui->listWidget->addItem(QString(datagram ));
    }
    connect(cliente.clientSocket, SIGNAL(readyRead()), SLOT(readyData()));
}

Server::~Server()
{
    delete ui;
}

void Server::on_pushButton_clicked()
{
    QString ipc = ipcliente.toString();
    ipc.remove(0,6);
    QByteArray datagrama = QByteArray(QString(ipc  + ": ").toLatin1());
    //QByteArray datagrama = QByteArray(QString(ipcliente.toString() + ": ").toLatin1());
    datagrama.append(ui->lineEdit->text().toLatin1());                //RECEBE O CONTEUDO da entrada
    writer();//bind
    udpSocket->writeDatagram(datagrama,QHostAddress::Broadcast,3636);//EFETUAMOS O ENVIO PARA TODOS OS NÓS DA REDE.
}

void Server::readyData()
{
    if(cliente.clientSocket->hasPendingDatagrams())
    {
    data.resize(cliente.clientSocket->pendingDatagramSize());    //ATUALIZA O TAMANHO DO TEXTO

    cliente.clientSocket->readDatagram(data.data(), data.size(), &ipcliente, &porta);///lê o texto com o tamanho de até 8192 BYTES

    ui->listWidget->addItem(data);                      //ADICIONA A LISTA DE TODOS OS NOSSOS USUÁRIOS
    }
}

void Server::escreve()
{
    iniciado = false;
    datagram = QByteArray(QString(" cliente "+ ipcliente.toString() + " conectado ").toLatin1() );
    writer();
    udpSocket->writeDatagram(datagram,QHostAddress::Broadcast,3636);//EFETUAMOS O ENVIO PARA TODOS OS NÓS DA REDE.
}

void Server::writer()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    cliente.clientSocket->bind(3636,QUdpSocket::ReuseAddressHint);
#endif
#if defined(__APPLE__)
        //BIND PARA APPLE
            cliente.clientSocket->bind(3636,QUdpSocket::ShareAddress);
#endif
#if defined(linux) || defined(__linux) || defined(__linux__)
        //BIND PARA LINUX
            cliente.clientSocket->bind(3636,QUdpSocket::ShareAddress);
#endif
}

void Server::pegaEpassa(QByteArray ba)
{
    if(ba.size() >= 8192)//sabemos q pode ser maior.
    {
        int comparaTamanho = ba.size() - 8192;
        ///descobrimos o tamanho real do pacote atual
        if(comparaTamanho > 8191)
        {//faz comparação para ver a necessidade de se usar a
            //recursividade para o caso de ser um bloco duas vezes
            //maior do que o permitido.
            /** graficamente temos:
             *         ( 0|1|2|.........................|55 ) |56| ....
             * bloco2 [( MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM ) MMMMMMMMMMMMMMMMMMMMMMMM...]   contem o valor original
             * bloco2.remove(0,  55) REMOVE A PRIMEIRA PARTE, DEIXANDO O RESTANTE AQUI DENTRO.
             * bloco2 [ MMMMMMMMMMMMMMMMMMMMMMMM...]
             *
             * ba [( MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM ) MMMMMMMMMMMMMMMMMMMMMMMM...]    contem o valor original do parametro.
             * ba.resize(55)        guarda apenas o que está dentro dos parenteses
             * ba [(MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM)]
             *
             */
            QByteArray bloco2;
            bloco2 = ba;
            //ba ainda tem o tamanho total original.
            bloco2.remove(0,8192);///remove o bloco inicial
            ba.resize(8192);//temos certeza do tamanho.

            fazEnvio(ba);
            pegaEpassa(bloco2);///RECURSIVIDADE
        }else{//se menor que o máximo esse novo pacote inserido.
            QByteArray bloco3;
            bloco3 = ba;
            bloco3.remove(0,8192);
            ba.resize(8192);
            fazEnvio(ba);//envia a primeira parte
            fazEnvio(bloco3);//envia a segunda parte.
        }
    }
    else
    {
        fazEnvio(ba);
    }
}

void Server::fazEnvio(QByteArray pacote)
{
   connect(cliente.clientSocket, &QUdpSocket::readyRead,[&]{
    if(cliente.clientSocket->hasPendingDatagrams())
    {
    pacote.resize(cliente.clientSocket->pendingDatagramSize());    //ATUALIZA O TAMANHO DO TEXTO
    cliente.clientSocket->readDatagram(pacote.data(), pacote.size());///lê o texto com o tamanho de até 8192 BYTES
    ui->listWidget->addItem(QString(pacote));                      //ADICIONA A LISTA DE TODOS OS NOSSOS USUÁRIOS
    }
   });
}

void Server::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}
