#ifndef GAME_H
#define GAME_H
#include <QString>
#include <QTcpSocket>
#include <QHostAddress>

class Game{
public:
    Game(QString gameId, QString hostPlayer, QTcpSocket* hostSocket);
    void addPlayer(QString joinPlayer, QTcpSocket* joinSocket);
    QString getHostPlayer() const;
    QTcpSocket *getRecieverSocket(QTcpSocket *senderSocket);
    QString getJoinPlayer() const;
    bool socketExists(QTcpSocket* socket);
    QString getGameId() const;

private:
    // bool hostTurn = true;
    QString gameId;
    QString hostPlayer;
    QString joinPlayer;
    QTcpSocket *hostSocket = nullptr;
    QTcpSocket *joinSocket = nullptr;
};

#endif // GAME_H
