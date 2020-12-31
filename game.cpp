#include "game.h"

Game::Game(QString hostPlayer,QTcpSocket* hostSocket)
    :hostPlayer(hostPlayer),hostSocket(hostSocket){

}

void Game::addPlayer(QString _joinPlayer, QTcpSocket *_joinSocket){
    joinPlayer = _joinPlayer;
    joinSocket = _joinSocket;
}

QString Game::getHostPlayer() const
{
    return hostPlayer;
}