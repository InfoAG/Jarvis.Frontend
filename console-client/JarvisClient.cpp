#include "JarvisClient.h"

JarvisClient::JarvisClient(const QString &server, quint16 port, const QString &name, const QString &pwd) : iStream(&streamBuf, QIODevice::ReadOnly), oStream(&socket), sendQueueStream(&sendQueue, QIODevice::WriteOnly)
{
    QObject::connect(&socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(&socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(server, port, name, pwd);
}

void JarvisClient::connect(const QString &server, quint16 port, const QString &name, const QString &pwd)
{
    socket.connectToHost(server, port);
    this->name = name;
    this->pwd = pwd;
}

void JarvisClient::connected()
{
    oStream << static_cast<quint8>(1); //version
    connectionState = Version;
}

void JarvisClient::readyRead()
{
    QString buffer_2, buffer_3;
    QList<ModulePackage> moduleList;

    streamBuf += socket.readAll();
    do {
        iStream.device()->reset();
        switch (connectionState) {
        case Version:
            if (pop_front()) {
                oStream << static_cast<quint8>(1) << name << pwd;
                connectionState = Login;
            } else connectionState = ServerVersion;
            break;
        case ServerVersion:
            serverVersion = pop_front();
            emit error(WrongVersion);
            connectionState = Version;
        case Login:
            if (pop_front()) setLoop();
            else {
                emit error(BadLogin);
                connectionState = ClientAction;
            }
            break;
        case Loop:
            switch (pop_front()) {
            case 0: connectionState = ClientEntered; break;
            case 1: connectionState = FuncDef; break;
            case 2: connectionState = VarDef; break;
            case 3: connectionState = NewScope; break;
            case 4: connectionState = Msg; break;
            case 5: connectionState = ClientLeft;
            }
            break;
        case ClientEntered:
            iStream >> buffer >> buffer_2;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                userLists[buffer].append(buffer_2);
                emit newClient(buffer, buffer_2);
                setLoop();
            } else return;
            break;
        case ClientLeft:
            iStream >> buffer >> buffer_2;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                userLists[buffer].removeOne(buffer_2);
                emit clientLeft(buffer, buffer_2);
                setLoop();
            } else return;
            break;
        case FuncDef:
            iStream >> buffer >> buffer_2;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                emit newFunction(buffer, buffer_2);
                setLoop();
            } else return;
            break;
        case VarDef:
            iStream >> buffer >> buffer_2;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                emit newVariable(buffer, buffer_2);
                setLoop();
            } else return;
            break;
        case NewScope:
            iStream >> buffer;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                emit newScope(buffer);
                setLoop();
            } else return;
            break;
        case Msg:
            iStream >> buffer >> buffer_2 >> buffer_3;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                emit msgInScope(buffer, buffer_2, buffer_3);
                setLoop();
            } else return;
            break;
        case ScopeInfo:
            iStream >> userLists[buffer];
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                setLoop();
            } else return;
            break;
        case Modules:
            iStream >> moduleList;
            if (iStream.status() == QDataStream::Ok) {
                resetStreamBuf();
                emit receivedModules(moduleList);
                setLoop();
            } else return;
            break;
        }
        if (socket.bytesAvailable()) streamBuf += socket.readAll();
    } while (! streamBuf.isEmpty());
}


void JarvisClient::enterScope(const QString &name)
{
    sendQueueStream << static_cast<quint8>(0) << name;
    addedToQueue();
    buffer = name;
    connectionState = ScopeInfo;
}

void JarvisClient::leaveScope(const QString &name)
{
    sendQueueStream << static_cast<quint8>(1) << name;
    addedToQueue();
}

void JarvisClient::msgToScope(const QString &scope, const QString &msg)
{
    sendQueueStream << static_cast<quint8>(2) << scope << msg;
    addedToQueue();
}

void JarvisClient::requestModules()
{
    sendQueueStream << static_cast<quint8>(3);
    addedToQueue();
    connectionState = Modules;
}
