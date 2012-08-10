#ifndef JARVISCLIENT_H
#define JARVISCLIENT_H

#include <QTcpSocket>
#include <QStringList>
#include "ModulePackage.h"
#include "Scope.h"
#include <map>
#include <algorithm>
#include <utility>

/**
 * Speaks to a Jarvis backend over network, maintains connection state and communicates changes through signals
 * @author Alexander Schlüter
 */
class JarvisClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nick READ nick)
    Q_PROPERTY(quint8 serverVersion READ serverVersion)

private:
    enum {
        ClientAction,   //!< Waiting for client side action
        Version,        //!< Sent client version, waiting for validation
        ServerVersion,  //!< Version mismatch, awaiting server version
        Login,          //!< Sent login info, waiting for validation
        InitInfo,       //!< Logged in, awaiting scope names & modules
        Loop,           //!< Expecting packet type id
        ClientEntered,  //!< Client entered a scope I am subscribed to, expecting scope name & client name
        ClientLeft,     //!< Client left a scope I am subscribed to, expecting scope name & client name
        FuncDef,        //!< New function definition in one of my scopes, awaiting scope name & definition
        VarDef,         //!< New variable definition in one of my scopes, awaiting scope name & definition
        NewScope,       //!< New scope, need scope name
        Msg,            //!< Message in one of my scopes, expecting scope name, sender name & msg string
        PkgLoaded,      //!< Modulepackage unloaded, awaiting name
        PkgUnloaded,    //!< Modulepackage loaded, waiting for pkg info
        ScopeInfo,      //!< Entered scope, getting clients & definitions
        ScopeDeleted    //!< Scope deleted, receiving scope name
    } connectionState; //!< Connection state

    QTcpSocket socket; //!< Socket with server connection
    QDataStream iStream, oStream;
    QString nick_; //!< Client name
    QString pwd; //!< Password
    quint8 serverVersion_; //!< Server version
    QByteArray streamBuf; //!< Stream buffer, needed because QDataStream can fail after stealing all the data from QTcpSocket
    std::map<quint8, QString> requestBuffer; //!< Maps string buffers to request ids until server answers

    /**
     * Pop first byte from streamBuf
     * @return the byte bro. just the byte.
     */
    quint8 pop_front() { quint8 result(streamBuf.at(0)); streamBuf.remove(0, 1); return result; }
    void resetStreamBuf() { streamBuf.remove(0, iStream.device()->pos()); } //!< Remove all bytes from streamBuf which have been read through iStream
    void connectSlots(); //!< Connect all private slots to signals from socket

public:
    enum ClientError {
        BadLogin,       //!< Server rejected login
        WrongVersion    //!< Version mismatch
    };

    JarvisClient() : iStream(&streamBuf, QIODevice::ReadOnly), oStream(&socket) { connectSlots(); } //!< Constructor
    /**
     * Construct a JarvisClient and connect immediately
     * @param server Server
     * @param port Port
     * @param name Nick
     * @param pwd Password
     */
    JarvisClient(const QString &server, quint16 port, const QString &nick, const QString &pwd);
    /**
     * Connect to a Jarvis server
     * @param server Server
     * @param port Port
     * @param name Nick
     * @param pwd Password
     */
    Q_INVOKABLE void connect(const QString &server, quint16 port, const QString &nick, const QString &pwd);
    void disconnect();

    quint8 serverVersion() const { return serverVersion_; } //!< @return Server version
    QString nick() const { return nick_; } //!< @return Nick

signals:
    /**
     * New scope added
     * @param name Scope name
     */
    void newScope(const QString &name);
    /**
     * New function definition
     * @param scope Scope name
     * @param def Definition
     */
    void newFunction(const QString &scope, const QString &def);
    /**
     * New variable definition
     * @param scope Scope name
     * @param def Definition
     */
    void newVariable(const QString &scope, const QString &def);
    /**
     * New client
     * @param scope Scope name
     * @param nick Nick
     */
    void newClient(const QString &scope, const QString &nick);
    /**
     * Client left a scope
     * @param scope Scope name
     * @param nick Nick
     */
    void clientLeft(const QString &scope, const QString &nick);
    /**
     * Message arrived
     * @param scope Scope name
     * @param sender Sender nick
     * @param msg Message string
     */
    void msgInScope(const QString &scope, const QString &sender, const QString &msg);
    /**
     * An error occurred
     * @param error Error type
     */
    void error(JarvisClient::ClientError error);
    /**
     * New module package loaded
     * @param pkg Package content
     */
    void pkgLoaded(const ModulePackage &pkg);
    /**
     * Module package unloaded
     * @param name Package name
     */
    void pkgUnloaded(const QString &name);
    /**
     * Successfully entered a scope
     * @param name Scope name
     * @param info Scope clients and definitions
     */
    void enteredScope(const QString &name, const Scope &info);
    /**
     * Received server wide info after login
     * @param scopes List of scope names
     * @param pkgs List of module packages
     */
    void receivedInitInfo(const QList<QString> &scopes, const QList<ModulePackage> &pkgs);
    /**
     * A scope was deleted
     * @param name Scope name
     */
    void deletedScope(const QString &name);
    void disconnected(); //!< Client disconnected from server

private slots:
    void connected(); //!< Socket connected to server
    void readyRead(); //!< Data available to read from socket
    
public slots:
    /**
     * Enter a scope
     * @param name Scope name
     */
    void enterScope(const QString &name);
    /**
     * Leave a scope
     * @param name Scope name
     */
    void leaveScope(const QString &name) {oStream << static_cast<quint8>(1) << name; }
    /**
     * Send a message to a scope
     * @param scope Scope name
     * @param msg Message string
     */
    void msgToScope(const QString &scope, const QString &msg) { oStream << static_cast<quint8>(2) << scope << msg; }
    /**
     * Unload a module package
     * @param name Package name
     */
    void unloadPkg(const QString &name) { oStream << static_cast<quint8>(4) << name; }
    /**
     * Load a module package
     * @param name Package name
     */
    void loadPkg(const QString &name) { oStream << static_cast<quint8>(3) << name; }
    /**
     * Delete a scope
     * @param name Scope name
     */
    void deleteScope(const QString &name) { oStream << static_cast<quint8>(5) << name; }
};

#endif // JARVISCLIENT_H
