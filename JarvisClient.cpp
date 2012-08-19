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
                    emit receivedInitInfo(QVariant::fromValue(stringListBuffer), QVariant::fromValue(moduleListBuffer));
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
            case 1: connectionState = FuncDef; break;
            case 2: connectionState = VarDef; break;
            case 3: connectionState = NewScope; break;
            case 4: connectionState = Msg; break;
            case 5: connectionState = ClientLeft; break;
            case 6: connectionState = PkgLoaded; break;
            case 7: connectionState = PkgUnloaded; break;
            case 8: connectionState = ScopeInfoHead; break;
            case 9: connectionState = ScopeDeleted; break;
            }
            break;
        case ClientEntered: {
                QString scope, name;
                iStream >> scope >> name;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit newClient(scope, name);
                    connectionState = Loop;
                } else  {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case ClientLeft: {
                QString scope, name;
                iStream >> scope >> name;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit clientLeft(scope, name);
                    connectionState = Loop;
                } else  {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case FuncDef: {
                QString scope, id, def;
                QStringList args;
                iStream >> scope >> id >> args >> def;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit newFunction(scope, id, args, def);
                    connectionState = Loop;
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case VarDef: {
                QString scope, identifier, definition;
                iStream >> scope >> identifier >> definition;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit newVariable(scope, identifier, definition);
                    connectionState = Loop;
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case NewScope: {
                QString name;
                iStream >> name;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit newScope(name);
                    connectionState = Loop;
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case Msg: {
                QString scope, sender, msg;
                iStream >> scope >> sender >> msg;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit msgInScope(scope, sender, msg);
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
                    emit pkgLoaded(QVariant::fromValue(pkgBuffer));
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
        case ScopeInfoHead: {
                quint8 success;
                iStream >> requestID >> success;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    if (success) connectionState = ScopeInfo;
                    else {
                        requestBuffer.erase(requestID);
                        emit error(AlreadyInScope);
                    }
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case ScopeInfo: {
                Scope scopeBuffer;
                iStream >> scopeBuffer;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    connectionState = Loop;
                    emit enteredScope(requestBuffer[requestID], QVariant::fromValue(scopeBuffer));
                    requestBuffer.erase(requestID);
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
            break;
        case ScopeDeleted: {
                QString name;
                iStream >> name;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    connectionState = Loop;
                    emit deletedScope(name);
                } else {
                    iStream.resetStatus();
                    return;
                }
            }
        }
        if (socket.bytesAvailable()) streamBuf += socket.readAll();
    } while (! streamBuf.isEmpty());
}


void JarvisClient::enterScope(const QString &name)
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
