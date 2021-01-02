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

QString Game::getGameId() const
{
    return gameId;
}
