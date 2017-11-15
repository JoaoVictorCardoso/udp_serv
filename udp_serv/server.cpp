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
/*
void Server::redimensiona()
{
    QByteArray pacote = data;//guarda o pacote grande
    if(pacote.size() > 8192 && pacote.size() < 16382){  //se temos que apenas duplicar o tamanho da mensagem.
        std::reverse(pacote.begin(),pacote.end());      //inverte array e armazena metade em uma seguda array.

        QByteArray segundaParteDaArray;                     //cria o segundo vetor temporario.
        segundaParteDaArray = pacote;                     //passa o conteudo do vetor original já invertido.
        segundaParteDaArray.resize(8191);                   //corta a segunda array para conter metade do valor original.

        std::reverse(pacote.begin(),pacote.end());      //REinverte novamente e volta ao original.
        pacote.resize(8191);                              //corta a mensagem ORIGINAL para ser enviada.
    }//depois de tratar até 16384 bytes ou seja 16382 caracteres da tabela ASCII enviamos.
    else if(pacote.size() >16382)
    {                                                               //captura de tras para frente a array
        std::reverse(pacote.begin(),pacote.end());              //inverte array e armazena um terço em um primeiro vetor.
        QByteArray ParteDaArray1;                                   //cria o segundo vetor temporario.
        ParteDaArray1 = pacote;                                   //passa o conteudo do vetor original já invertido.
        ParteDaArray1.resize(8191);                                 //corta a segunda array para conter um terço do valor original.
        std::reverse(pacote.begin(),pacote.end());              //volta ao original
        QByteArray ParteDaArray2 = pacote;                        //cria o segundo vetor temporario.
        ParteDaArray2.chop(8191);                                   //retira a ParteDaArray1 do tamanho original deixando 2 terços.
        std::reverse(ParteDaArray2.begin(),ParteDaArray2.end());    //inverte array e armazena a segunda parte no começo da seguda array.
        ParteDaArray2.resize(8191);                                 //corta a segunda array para conter a segunda metade do valor original.
        QByteArray ParteDaArray3 = pacote;                        //cria o terceiro vetor temporario.
        ParteDaArray3.resize(8191);                                 //armazena o conteudo do inicio da variavel MENSAGEM.

    }   // 24576 BYTES armazenados em 3 diferentes variáveis que serao enviadas em processos distintos.
    else
    {
        try{//o tamanho maximo por unidade pode ser até 8192 bytes
            // socket->writeDatagram(Mensagem, grupoHostClient, 45677);
        }
        catch(qint64 datagramaExedeTamanho){
            if(datagramaExedeTamanho == -1){
                qDebug() << "datagrama excede o tamanho para um pacote UDP";
            }
        }
    }
}
*/
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
