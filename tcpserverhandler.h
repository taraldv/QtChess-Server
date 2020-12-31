#ifndef TCPSERVERHANDLER_H
#define TCPSERVERHANDLER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QtConcurrent/QtConcurrent>
#include <QObject>
#include "game.h"

class TcpServerHandler: public QObject{
    Q_OBJECT
public:
    TcpServerHandler(int portNumber);
    void acceptTcpConnection();
    void readTcpPacket();
private:
    QTcpServer* mTcpServer;
    QList<Game> games;
    int findGameWithHost(QString host);
};

#endif // TCPSERVERHANDLER_H
