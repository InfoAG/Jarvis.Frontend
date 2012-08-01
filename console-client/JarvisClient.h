#ifndef JARVISCLIENT_H
#define JARVISCLIENT_H

#include <QTcpSocket>

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
        EnterScope,
        EnterClient,
        LeaveScope,
        LeaveClient,
        FuncScope, //new function definition, waiting for scope name size
        FuncDef,
        VarScope, //new variable definition, waiting for scope name size
        VarDef,
        NewScope, //new scope, waiting for scope name size
        MsgScope, //message in scope, waiting for scope name size
        MsgClient,
        Msg,
        ScopeInfo
    } connectionState;

    enum {
        StringSize,
        String
    } stringReceiveState = StringSize;

    QTcpSocket socket;
    QDataStream stream;
    QString name;
    QString pwd;
    quint8 serverVersion;
    QString buffer;
    quint32 nextBlockSize;
    QMap<QString, QList<QString> > userLists;

    bool receiveString(QString &dest);

public:
    enum ClientError {
        BadLogin,
        WrongVersion
    };

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
    
public slots:
    void connected();
    void readyRead();

    void enterScope(const QString &);
    void leaveScope(const QString &);
    void msgToScope(const QString &, const QString &);
};

#endif // JARVISCLIENT_H
