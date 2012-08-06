#ifndef JARVISCLIENT_H
#define JARVISCLIENT_H

#include <QTcpSocket>
#include <QStringList>
#include "ModulePackage.h"

class JarvisClient : public QObject
{
    Q_OBJECT

private:
    enum {
        ClientAction,
        Version, //expecting answer after sending client version
        ServerVersion, //expecting server version
        Login, //expecting answer after sending login info
        Loop, //expecting type id
        ClientEntered,
        ClientLeft,
        FuncDef, //new function definition, waiting for scope name size
        VarDef, //new variable definition, waiting for scope name size
        NewScope, //new scope, waiting for scope name size
        Msg,
        ScopeInfo,
        Modules
    } connectionState;

    enum {
        StringSize,
        String
    } stringReceiveState = StringSize;

    enum {
        ListSize,
        ItemSize,
        ItemBody
    } listReceiveState = ListSize;

    QTcpSocket socket;
    QDataStream iStream, oStream, sendQueueStream;
    QString name;
    QString pwd;
    quint8 serverVersion;
    QByteArray streamBuf, sendQueue;
    QString buffer;

    void setLoop() { connectionState = Loop; dispatchQueue(); }
    void addedToQueue() { if (connectionState == Loop) dispatchQueue(); }
    void dispatchQueue() { socket.write(sendQueue); sendQueue.clear(); sendQueueStream.device()->reset(); }
    quint8 pop_front() { quint8 result(streamBuf.at(0)); streamBuf.remove(0, 1); return result; }
    void resetStreamBuf() { streamBuf.remove(0, iStream.device()->pos()); }

public:
    enum ClientError {
        BadLogin,
        WrongVersion
    };
    QMap<QString, QList<QString> > userLists;

    JarvisClient(const QString &server, quint16 port, const QString &name, const QString &pwd);
    void connect(const QString &server, quint16 port, const QString &name, const QString &pwd);
    void disconnect();

    quint8 getServerVersion() const { return serverVersion; };

signals:
    void newScope(const QString &name);
    void newFunction(const QString &scope, const QString &def);
    void newVariable(const QString &scope, const QString &def);
    void newClient(const QString &scope, const QString &name);
    void clientLeft(const QString &scope, const QString &name);
    void msgInScope(const QString &scope, const QString &sender, const QString &msg);
    void error(JarvisClient::ClientError error);
    void receivedModules(QList<ModulePackage>);
    
public slots:
    void connected();
    void readyRead();

    void enterScope(const QString &);
    void leaveScope(const QString &);
    void msgToScope(const QString &, const QString &);
    void requestModules();
};

#endif // JARVISCLIENT_H
