#ifndef GAME_H
#define GAME_H
#include <QString>
#include <QTcpSocket>

class Game
{
public:
    Game(QString hostPlayer, QTcpSocket* hostSocket);
    void addPlayer(QString joinPlayer, QTcpSocket* joinSocket);
    QString getHostPlayer() const;

private:
   // bool hostTurn = true;
    QString hostPlayer;
    QString joinPlayer;
    QTcpSocket *hostSocket;
    QTcpSocket *joinSocket;
};

#endif // GAME_H
