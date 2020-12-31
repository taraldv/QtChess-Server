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
    QHostAddress senderAddress = readSocket->peerAddress();
    int port = readSocket->peerPort();

    QByteArray data = readSocket->readAll();

    qDebug() << "Addr: " << senderAddress << " Port: " << port << " Data: " << data;
    QByteArray tempArr = data;
    int messageCode = tempArr[0];
    tempArr.remove(0, 1);


    if(messageCode == 0){
        //Host game: code hostName
        int hostNameLength = tempArr[0];
        tempArr.remove(0, 1);
        QByteArray hostNameArray = QByteArray(tempArr, hostNameLength);
        QString hostName(hostNameArray);
        tempArr.remove(0, hostNameLength);
        qDebug() << "starting game with hostName: " << hostName;
        //Check if name exists
        if(findGameWithHost(hostName)<0){

            //Create new game and append to list
            Game newGame(hostName,readSocket);
            games.append(newGame);
            QByteArray output;
            int msg = 1;
            output.append(msg);
            readSocket->write(output);
        } else {
            qDebug() << "Hosting failed, host already exists";
            QByteArray output;
            int msg = 0;
            output.append(msg);
            readSocket->write(output);
        }
    } else if(messageCode == 1){
        //Join game: code playerName hostName
        int playerNameLength = tempArr[0];
        tempArr.remove(0, 1);
        QByteArray playerNameArray = QByteArray(tempArr, playerNameLength);
        QString playerName(playerNameArray);
        tempArr.remove(0, playerNameLength);

        int hostNameLength = tempArr[0];
        tempArr.remove(0, 1);
        QByteArray hostNameArray = QByteArray(tempArr, hostNameLength);
        QString hostName(hostNameArray);
        tempArr.remove(0, hostNameLength);

        qDebug() << "player: " << playerName << " attempting to join host: " << hostName;

        int gameIndex = findGameWithHost(hostName);
        if(gameIndex >= 0){
            games[gameIndex].addPlayer(playerName,readSocket);
            QByteArray output;
            output.append(1);
            readSocket->write(output);
        } else {
            qDebug() << "Could not find game with host: " << hostName;
            QByteArray output;
            int msg = 0;
            output.append(msg);
            readSocket->write(output);
        }
    } else if(messageCode == 2){
        //Move
        int hostNameLength = tempArr[0];
        tempArr.remove(0, 1);
        QByteArray hostNameArray = QByteArray(tempArr, hostNameLength);
        QString hostName(hostNameArray);
        tempArr.remove(0, hostNameLength);
        int gameIndex = findGameWithHost(hostName);
        qDebug() << "Game with host: " << hostName << " attempting to move";
        if(gameIndex >= 0){
            int fromMoveLength = tempArr[0];
            tempArr.remove(0, 1);
            QByteArray fromMoveArray = QByteArray(tempArr, fromMoveLength);
            //QString fromMove(fromMoveArray);
            tempArr.remove(0, fromMoveLength);

            int toMoveLength = tempArr[0];
            tempArr.remove(0, 1);
            QByteArray toMoveArray = QByteArray(tempArr, toMoveLength);
            //QString toMove(toMoveArray);
            tempArr.remove(0, toMoveLength);

            QByteArray output;
            //2 is the code for the clients to read a move
            output.append(2);
            output.append(fromMoveArray.length());
            output.append(fromMoveArray);
            output.append(toMoveArray.length());
            output.append(toMoveArray);

            games[gameIndex].getRecieverSocket(senderAddress)->write(output);
        } else {
            qDebug() << "Move failed, Game with host: " << hostName << " does not exist";
            QByteArray output;
            int msg = 0;
            output.append(msg);
            readSocket->write(output);
        }
    } else if(messageCode == 3){
        //Quit
    }
    //readSocket->write(data);
}

int TcpServerHandler::findGameWithHost(QString host){
    for(int i=0;i<games.size();i++){
        if(games[i].getHostPlayer().compare(host)==0){
            return i;
        }
    }
    return -1;
}
