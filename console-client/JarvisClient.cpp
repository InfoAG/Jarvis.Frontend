#include "JarvisClient.h"

JarvisClient::JarvisClient(const QString &server, quint16 port, const QString &name, const QString &pwd) : stream(&socket), listStream(&listBuffer, QIODevice::ReadOnly)
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
    stream << static_cast<quint8>(1); //version
    connectionState = Version;
}

bool JarvisClient::receiveString(QString &dest)
{
    do {
        if (stringReceiveState == StringSize) {
            if (socket.bytesAvailable() >= sizeof(quint32)) {
                QDataStream(socket.peek(sizeof(quint32))) >> nextBlockSize;
                nextBlockSize += sizeof(quint32);
                stringReceiveState = String;
            } else return false;
        } else {
            if (socket.bytesAvailable() >= nextBlockSize) {
                stream >> dest;
                stringReceiveState = StringSize;
                return true;
            } else return false;
        }
    } while (socket.bytesAvailable());
}

bool JarvisClient::receiveStringList(QList<QString> &dest)
{
    do {
        if (listReceiveState == ListSize) {
            if (socket.bytesAvailable() >= sizeof(quint32)) {
                listBuffer += socket.read(sizeof(quint32));
                listStream.device()->reset();
                listStream >> nextBlockSize;
                if (! nextBlockSize) {
                    dest = QList<QString>();
                    return true;
                } else listReceiveState = ItemSize;
            } else return false;
        } else if (listReceiveState == ItemSize) {
            if (socket.bytesAvailable() >= sizeof(quint32)) {
                QDataStream(socket.peek(sizeof(quint32))) >> itemBlockSize;
                listBuffer +=  socket.read(sizeof(quint32));
                listReceiveState = ItemBody;
            } else return false;
        } else if (socket.bytesAvailable() >= itemBlockSize) {
            listBuffer +=  socket.read(itemBlockSize);
            if (! --nextBlockSize) {
                listStream.device()->reset();
                listStream >> dest;
                listReceiveState = ListSize;
                return true;
            } else listReceiveState = ItemSize;
        } else return false;
    } while (socket.bytesAvailable());
}

void JarvisClient::readyRead()
{
    QString buffer_3;
    do {
        switch (connectionState) {
        case Version:
            if (socket.read(1).at(0)) {
                stream << static_cast<quint8>(1) << name << pwd;
                connectionState = Login;
            } else connectionState = ServerVersion;
            break;
        case ServerVersion:
            serverVersion = socket.read(1).toUInt();
            emit error(WrongVersion);
            connectionState = Version;
        case Login:
            if (socket.read(1).at(0)) connectionState = Loop;
            else {
                emit error(BadLogin);
                connectionState = ClientAction;
            }
            break;
        case Loop:
            switch (socket.read(1).at(0)) {
            case 0: connectionState = EnterScope; break;
            case 1: connectionState = FuncScope; break;
            case 2: connectionState = VarScope; break;
            case 3: connectionState = NewScope; break;
            case 4: connectionState = MsgScope; break;
            }
            break;
        case EnterScope:
            if (receiveString(buffer)) connectionState = EnterClient;
            else return;
            break;
        case EnterClient:
            if (receiveString(buffer_2)) {
                emit newClient(buffer, buffer_2);
                connectionState = Loop;
            } else return;
            break;
        case LeaveScope:
            if (receiveString(buffer)) connectionState = LeaveClient;
            else return;
            break;
        case LeaveClient:
            if (receiveString(buffer_2)) {
                emit clientLeft(buffer, buffer_2);
                connectionState = Loop;
            } else return;
            break;
        case FuncScope:
            if (receiveString(buffer)) connectionState = FuncDef;
            else return;
            break;
        case FuncDef:
            if (receiveString(buffer_2)) {
                emit newFunction(buffer, buffer_2);
                connectionState = Loop;
            } else return;
            break;
        case VarScope:
            if (receiveString(buffer)) connectionState = VarDef;
            else return;
            break;
        case VarDef:
            if (receiveString(buffer_2)) {
                emit newVariable(buffer, buffer_2);
                connectionState = Loop;
            } else return;
            break;
        case NewScope:
            if (receiveString(buffer)) {
                emit newScope(buffer);
                connectionState = Loop;
            } else return;
            break;
        case MsgScope:
            if (receiveString(buffer)) connectionState = MsgClient;
            else return;
            break;
        case MsgClient:
            if (receiveString(buffer_2)) connectionState = Msg;
            else return;
            break;
        case Msg:
            if (receiveString(buffer_3)) {
                emit msgInScope(buffer, buffer_2, buffer_3);
                connectionState = Loop;
            } else return;
            break;
        case ScopeInfo:
            if (receiveStringList(userLists[buffer])) connectionState = Loop;
            else return;
            break;
        }
    } while (socket.bytesAvailable());
}


void JarvisClient::enterScope(const QString &name)
{
    if (connectionState != Loop) throw 0;
    else {
        stream << static_cast<quint8>(0) << name;
        buffer = name;
        connectionState = ScopeInfo;
    }
}

void JarvisClient::leaveScope(const QString &name)
{
    if (connectionState != Loop) throw 0;
    else {
        stream << static_cast<quint8>(1) << name;
    }
}

void JarvisClient::msgToScope(const QString &scope, const QString &msg)
{
    if (connectionState != Loop) throw 0;
    else {
        stream << static_cast<quint8>(2) << scope << msg;
    }
}
