#include "game.h"

Game::Game(QString gameId, QString hostPlayer,QTcpSocket* hostSocket)
    :gameId(gameId), hostPlayer(hostPlayer),hostSocket(hostSocket){

}

void Game::addPlayer(QString _joinPlayer, QTcpSocket *_joinSocket){
    joinPlayer = _joinPlayer;
    joinSocket = _joinSocket;
}

QString Game::getHostPlayer() const
{
    return hostPlayer;
}

QTcpSocket *Game::getRecieverSocket(QTcpSocket *senderSocket){
    if(senderSocket->peerAddress() == hostSocket->peerAddress()
            && senderSocket->peerPort() == hostSocket->peerPort()){
        return joinSocket;
    } else {
        return hostSocket;
    }
}

QString Game::getJoinPlayer() const
{
    return joinPlayer;
}

bool Game::socketExists(QTcpSocket *socket){
    QHostAddress socketAddr = socket->peerAddress();
    int socketPort = socket->peerPort();

    bool matchesHost = false;
    bool matchesJoin = false;

    if(hostSocket){
        QHostAddress hostAddr = hostSocket->peerAddress();
        int hostPort = hostSocket->peerPort();
        matchesHost = (hostAddr == socketAddr && hostPort == socketPort);
    }
    if(joinSocket){
        QHostAddress joinAddr = joinSocket->peerAddress();
        int joinPort = joinSocket->peerPort();
        matchesJoin = (joinAddr == socketAddr && joinPort == socketPort);
    }
    return matchesJoin || matchesHost;
}

QString Game::getGameId() const
{
    return gameId;
}
