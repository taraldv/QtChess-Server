#ifndef TCPSERVERHANDLER_H
#define TCPSERVERHANDLER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QtConcurrent/QtConcurrent>
#include <QObject>
#include <QUuid>
#include "game.h"

class TcpServerHandler: public QObject{
    Q_OBJECT
public:
    TcpServerHandler(int portNumber);
    void acceptTcpConnection();
    void readTcpPacket();
private:
    void lostConnection();
    QTcpServer* mTcpServer;
    QList<Game> games;
    bool doesGameExist(QString gameId);
    int getGameIndex(QString gameId);
    QString getNextStringSegement(QByteArray *data);
};

#endif // TCPSERVERHANDLER_H
