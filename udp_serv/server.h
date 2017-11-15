#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QHostAddress>

namespace Ui {
class Server;
}

class QUdpSocket; //cria a classe UDP


class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    bool iniciado = true;

    QByteArray datagram;
    ~Server();

private slots:
    void on_pushButton_clicked();       //O BOTÃO PARA COMPARTILHAR COM TODOS
    void readyData();
    void on_lineEdit_returnPressed();

private:
    Ui::Server *ui;
    QUdpSocket *udpSocket;      //cria o SOCKET
    QByteArray data;            //TEXTO QUE VAMOS ESCREVER
    QHostAddress ipcliente;
    quint16 porta;
  //  void redimensiona();
    void escreve();
    void writer();
    void pegaEpassa(QByteArray ba);
    void fazEnvio(QByteArray pacote);

};

#endif // SERVER_H
