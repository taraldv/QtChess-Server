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
        qDebug() << hostName;
        //Check if name exists
        if(findGameWithHost(hostName)<0){

            //Create new game and append to list
            Game newGame(hostName,readSocket);
            games.append(newGame);
            readSocket->write("1");
        } else {
            readSocket->write("0");
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

        qDebug() << "player: " << playerName << " host: " << hostName;

        int gameIndex = findGameWithHost(hostName);
        if(gameIndex >= 0){
            games[gameIndex].addPlayer(playerName,readSocket);
            readSocket->write("1");
        } else {
            qDebug() << "Could not find game with host: " << hostName << " " << Q_FUNC_INFO;
            readSocket->write("0");
        }
    } else if(messageCode == 2){
        //Move
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
