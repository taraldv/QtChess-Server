#include <QCoreApplication>
#include <QDebug>
#include <tcpserverhandler.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << QT_VERSION_STR;
    int portNumber = 2233;
    new TcpServerHandler(portNumber);
    return a.exec();
}
