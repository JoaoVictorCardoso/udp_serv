#include "server.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server w;                   //CHAMA O SERVIDOR REMOTO
    w.show();                   ///exibe o seu aplicativo modesto
    return a.exec();
}
