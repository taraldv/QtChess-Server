#include "tcpserverhandler.h"

TcpServerHandler::TcpServerHandler(int portNumber): QObject(nullptr){
    mTcpServer = new QTcpServer();
    connect(mTcpServer, &QTcpServer::newConnection, this, &TcpServerHandler::acceptTcpConnection);
    mTcpServer->listen(QHostAddress::Any, portNumber);
    qDebug() << "TCP Listening on port:" << portNumber;
}

void TcpServerHandler::acceptTcpConnection(){
    QTcpSocket* socket = mTcpServer->nextPendingConnection();
    if (!socket){
        qDebug() << "Error: got invalid pending connection!";
    }

    connect(socket, &QIODevice::readyRead, this, &TcpServerHandler::readTcpPacket);
    //connect(tcpServerConnection, &QAbstractSocket::errorOccurred, this, &SocketHandler::displayError);
}


void TcpServerHandler::readTcpPacket(){

    QTcpSocket* readSocket = qobject_cast<QTcpSocket*>(sender());
    /*QHostAddress senderAddress = readSocket->peerAddress();
    int port = readSocket->peerPort();*/

    QByteArray data = readSocket->readAll();


    //qDebug() << "Addr: " << senderAddress << " Port: " << port << " Data: " << data;

    int messageCode = data[0];
    data.remove(0, 1);


    if(messageCode == 0){
        //Host game: code hostName

        QString hostName = getNextStringSegement(&data);
        qDebug() << "starting game with hostName: " << hostName;
        //Create new game and append to list
        QString key = QUuid::createUuid().toString(QUuid::Id128).left(20);
        Game newGame(key, hostName, readSocket);
        games.append(newGame);
        QByteArray output;
        //4 Means clients will store the gameId
        int msg = 4;
        output.append(msg);
        output.append(key.length());
        output.append(key);
        readSocket->write(output);

    } else if(messageCode == 1){
        //Join game: code playerName gameId

        QString playerName = getNextStringSegement(&data);
        QString gameId = getNextStringSegement(&data);
        qDebug() << "player: " << playerName << " attempting to join host: " << gameId;
        int gameIndex = getGameIndex(gameId);
        if(gameIndex >= 0){
            games[gameIndex].addPlayer(playerName,readSocket);
            QByteArray output;
            output.append(1);
            readSocket->write(output);
        } else {
            qDebug() << "Could not find game with gameId: " << gameId;
            QByteArray output;
            int msg = 0;
            output.append(msg);
            readSocket->write(output);
        }
    } else if(messageCode == 2){
        //Move: code gameId from to
        QString gameId = getNextStringSegement(&data);

        qDebug() << "Game with id: " << gameId << " attempting to move";
        if(doesGameExist(gameId)){
            int gameIndex = getGameIndex(gameId);

            QString from = getNextStringSegement(&data);
            QString to = getNextStringSegement(&data);

            QByteArray output;
            //2 is the code for the clients to read a move
            output.append(2);
            output.append(from.length());
            output.append(from);
            output.append(to.length());
            output.append(to);

            games[gameIndex].getRecieverSocket(readSocket)->write(output);
        } else {
            qDebug() << "Move failed, Game with id: " << gameId << " does not exist";
            QByteArray output;
            int msg = 0;
            output.append(msg);
            readSocket->write(output);
        }
    } else if(messageCode == 3){
        //Request games: code
        QByteArray output;
        //3 is the code for clients to accept a list of games
        output.append(3);
        //Append the amount of games
        output.append(games.size());
        for(int i=0;i<games.size();i++){
            QString hostName = games[i].getHostPlayer();
            QString gameId = games[i].getGameId();
            output.append(hostName.length());
            output.append(hostName);
            output.append(gameId.length());
            output.append(gameId);
        }
        readSocket->write(output);
    } else if(messageCode == 4){
        //Quit?
    }
    //readSocket->write(data);*/
}

bool TcpServerHandler::doesGameExist(QString gameId){
    return (getGameIndex(gameId) >= 0);
}

int TcpServerHandler::getGameIndex(QString gameId){
    for(int i=0;i<games.size();i++){
        if(games[i].getGameId().compare(gameId)==0){
            return i;
        }
    }
    return -1;
}

QString TcpServerHandler::getNextStringSegement(QByteArray *data){
    int segmentLength = *data[0];
    data->remove(0, 1);
    QByteArray segment = QByteArray(*data, segmentLength);
    QString stringSegment(segment);
    data->remove(0, segmentLength);
    return stringSegment;
}

