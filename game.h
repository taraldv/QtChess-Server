#ifndef GAME_H
#define GAME_H
#include <QString>
#include <QTcpSocket>
#include <QHostAddress>

class Game{
public:
    Game(QString hostPlayer, QTcpSocket* hostSocket);
    void addPlayer(QString joinPlayer, QTcpSocket* joinSocket);
    QString getHostPlayer() const;
    QTcpSocket *getRecieverSocket(QTcpSocket *senderSocket);
private:
   // bool hostTurn = true;
    QString hostPlayer;
    QString joinPlayer;
    QTcpSocket *hostSocket;
    QTcpSocket *joinSocket;
};

#endif // GAME_H
