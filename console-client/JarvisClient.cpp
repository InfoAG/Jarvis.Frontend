#include "JarvisClient.h"

JarvisClient::JarvisClient(const QString &server, quint16 port, const QString &name, const QString &pwd) : iStream(&streamBuf, QIODevice::ReadOnly), oStream(&socket)
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
                } else return;
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
            case 8: connectionState = ScopeInfo; break;
            }
            break;
        case ClientEntered: {
                QString scope, name;
                iStream >> scope >> name;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit newClient(scope, name);
                    connectionState = Loop;
                } else return;
            }
            break;
        case ClientLeft: {
                QString scope, name;
                iStream >> scope >> name;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit clientLeft(scope, name);
                    connectionState = Loop;
                } else return;
            }
            break;
        case FuncDef: {
                QString scope, def;
                iStream >> scope >> def;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit newFunction(scope, def);
                    connectionState = Loop;
                } else return;
            }
            break;
        case VarDef: {
                QString scope, def;
                iStream >> scope >> def;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit newVariable(scope, def);
                    connectionState = Loop;
                } else return;
            }
            break;
        case NewScope: {
                QString name;
                iStream >> name;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit newScope(name);
                    connectionState = Loop;
                } else return;
            }
            break;
        case Msg: {
                QString scope, sender, msg;
                iStream >> scope >> sender >> msg;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    emit msgInScope(scope, sender, msg);
                    connectionState = Loop;
                } else return;
            }
            break;
        case PkgLoaded: {
                ModulePackage pkgBuffer;
                iStream >> pkgBuffer;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    connectionState = Loop;
                    emit pkgLoaded(pkgBuffer);
                } else return;
            }
            break;
        case PkgUnloaded: {
                ModulePackage pkgBuffer;
                iStream >> pkgBuffer;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    connectionState = Loop;
                    emit pkgUnloaded(pkgBuffer);
                } else return;
            }
            break;
        case ScopeInfo: {
                quint8 byteBuffer;
                Scope scopeBuffer;
                iStream >> byteBuffer >> scopeBuffer;
                if (iStream.status() == QDataStream::Ok) {
                    resetStreamBuf();
                    connectionState = Loop;
                    emit enteredScope(requestBuffer[byteBuffer], scopeBuffer);
                    requestBuffer.erase(byteBuffer);
                } else return;
            }
            break;
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

void JarvisClient::leaveScope(const QString &name)
{
    oStream << static_cast<quint8>(1) << name;
}

void JarvisClient::msgToScope(const QString &scope, const QString &msg)
{
    oStream << static_cast<quint8>(2) << scope << msg;
}

void JarvisClient::loadPkg(const QString &module)
{
    oStream << static_cast<quint8>(3) << module;
}

void JarvisClient::unloadPkg(const QString &module)
{
    oStream << static_cast<quint8>(4) << module;
}

