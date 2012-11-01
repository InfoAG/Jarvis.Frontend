#include "JarvisClient.h"

JarvisClient::JarvisClient(const QString &server, quint16 port, const QString &nick, const QString &pwd) : iStream(&streamBuf, QIODevice::ReadOnly), oStream(&socket)
{
    connectSlots();
    connect(server, port, nick, pwd);
}

void JarvisClient::connect(const QString &server, quint16 port, const QString &nick, const QString &pwd)
{
    socket.connectToHost(server, port);
    nick_ = nick;
    this->pwd = pwd;
}

void JarvisClient::connected()
{
    oStream.resetStatus();
    oStream << static_cast<quint8>(1); //version
    connectionState = Version;
}

void JarvisClient::readyRead()
{
    streamBuf += socket.readAll();
    do {
        iStream.device()->reset();
        switch (connectionState) {
        case Version:
            if (pop_front()) {
                oStream << static_cast<quint8>(1) << nick_ << pwd;
                connectionState = Login;
            } else connectionState = ServerVersion;
            break;
        case ServerVersion:
            serverVersion_ = pop_front();
            emit error(WrongVersion);
            connectionState = Version;
        case Login:
            if (pop_front()) connectionState = InitInfo;
            else {
                emit error(BadLogin);
                connectionState = ClientAction;
            }
            break;
        case InitInfo: {
                QList<ModulePackage> moduleListBuffer;
                QStringList stringListBuffer;
                iStream >> stringListBuffer >> moduleListBuffer;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit receivedInitInfo(stringListBuffer, moduleListBuffer);
                    connectionState = Loop;
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case Loop:
            switch (pop_front()) {
            case 0: connectionState = ClientEntered; break;
            case 1: connectionState = FuncDeclaration; break;
            case 2: connectionState = VarDeclaration; break;
            case 3: connectionState = NewRoom; break;
            case 4: connectionState = Msg; break;
            case 5: connectionState = ClientLeft; break;
            case 6: connectionState = PkgLoaded; break;
            case 7: connectionState = PkgUnloaded; break;
            case 8: connectionState = RoomInfoHead; break;
            case 9: connectionState = RoomDeleted; break;
            case 10:
                oStream << static_cast<quint8>(7);
                break;
            case 11: connectionState = VarDefinition; break;
            case 12: connectionState = FuncDefinition; break;
            }
            break;
        case ClientEntered: {
                QString room, name;
                iStream >> room >> name;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit newClient(room, name);
                    connectionState = Loop;
                } else  {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case ClientLeft: {
                QString room, name;
                iStream >> room >> name;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit clientLeft(room, name);
                    connectionState = Loop;
                } else  {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case FuncDeclaration: {
                QString room, returnType;
                FunctionSignature sig;
                iStream >> room >> sig >> returnType;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit declaredFunction(room, sig, returnType);
                    connectionState = Loop;
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case FuncDefinition: {
                QString room, id, def;
                QList<QPair<QString, QString>> arguments;
                iStream >> room >> id >> arguments >> def;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit definedFunction(room, id, arguments, def);
                    connectionState = Loop;
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case VarDeclaration: {
                QString room, identifier, type;
                iStream >> room >> type >> identifier;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit declaredVariable(room, identifier, type);
                    connectionState = Loop;
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case VarDefinition: {
                QString room, identifier, definition;
                iStream >> room >> identifier >> definition;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit definedVariable(room, identifier, definition);
                    connectionState = Loop;
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case NewRoom: {
                QString name;
                iStream >> name;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit newRoom(name);
                    connectionState = Loop;
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case Msg: {
                QString room, sender, msg;
                iStream >> room >> sender >> msg;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit msgInRoom(room, sender, msg);
                    connectionState = Loop;
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case PkgLoaded: {
                ModulePackage pkgBuffer;
                iStream >> pkgBuffer;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    connectionState = Loop;
                    emit pkgLoaded(pkgBuffer);
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case PkgUnloaded: {
                QString pkgName;
                iStream >> pkgName;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    connectionState = Loop;
                    emit pkgUnloaded(pkgName);
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case RoomInfoHead: {
                quint8 success;
                iStream >> requestID >> success;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    if (success) connectionState = RoomInfo;
                    else {
                        requestBuffer.erase(requestID);
                        emit error(AlreadyInRoom);
                        connectionState = Loop;
                    }
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case RoomInfo: {
                Room roomBuffer;
                iStream >> roomBuffer;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    connectionState = Loop;
                    emit enteredRoom(requestBuffer[requestID], roomBuffer);
                    requestBuffer.erase(requestID);
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case RoomDeleted: {
                QString name;
                iStream >> name;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    connectionState = Loop;
                    emit deletedRoom(name);
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
        }
        if (socket.bytesAvailable()) streamBuf += socket.readAll();
    } while (! streamBuf.isEmpty());
}


void JarvisClient::enterRoom(const QString &name)
{
    quint8 requestID;
    if (requestBuffer.empty() || requestBuffer.begin()->first != 0) requestID = 0;
    else {
        requestID = std::adjacent_find(requestBuffer.begin(), requestBuffer.end(), [](const std::pair<const quint8, QString> &first, const std::pair<const quint8, QString> &second) {
                return (second.first - first.first) > 1;
            })->first;
    }
    oStream << static_cast<quint8>(0) << requestID << name;
    requestBuffer.insert(std::make_pair(requestID, name));
}


void JarvisClient::connectSlots()
{
    QObject::connect(&socket, SIGNAL(connected()), SLOT(connected()));
    QObject::connect(&socket, SIGNAL(readyRead()), SLOT(readyRead()));
    QObject::connect(&socket, SIGNAL(disconnected()), SIGNAL(disconnected()));
}
