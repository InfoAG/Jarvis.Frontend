#ifndef JARVISCLIENT_H
#define JARVISCLIENT_H

#include <QTcpSocket>
#include <QStringList>
#include "ModulePackage.h"
#include "Room.h"
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
        InitInfo,       //!< Logged in, awaiting room names & modules
        Loop,           //!< Expecting packet type id
        ClientEntered,  //!< Client entered a room I am subscribed to, expecting room name & client name
        ClientLeft,     //!< Client left a room I am subscribed to, expecting room name & client name
        FuncDef,        //!< New function definition in one of my rooms, awaiting room name & definition
        VarDeclaration, //!< New variable declaration in one of my rooms, awaiting room name & definition
        VarDefinition,
        NewRoom,        //!< New room, need room name
        Msg,            //!< Message in one of my rooms, expecting room name, sender name & msg string
        PkgLoaded,      //!< Modulepackage unloaded, awaiting name
        PkgUnloaded,    //!< Modulepackage loaded, waiting for pkg info
        RoomInfoHead,
        RoomInfo,      //!< Entered room, getting clients & definitions
        RoomDeleted    //!< Room deleted, receiving room name
    } connectionState; //!< Connection state

    QTcpSocket socket; //!< Socket with server connection
    QDataStream iStream, oStream; //!< Input and output streams for socket
    QString nick_; //!< Client name
    QString pwd; //!< Password
    quint8 requestID, serverVersion_; //!< Server version
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
        WrongVersion,    //!< Version mismatch
        AlreadyInRoom
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

    quint8 serverVersion() const { return serverVersion_; } //!< @return Server version
    QString nick() const { return nick_; } //!< @return Nick

signals:
    /**
     * New room added
     * @param name Room name
     */
    void newRoom(const QString &name);
    void newFunction(const QString &room, const QString &identifier, const QList<QPair<QString, QString>> &arguments, const QString &def);
    void declaredVariable(const QString &room, const QString &identifier, const QString &type);
    void definedVariable(const QString &room, const QString &identifier, const QString &definition);
    /**
     * New client
     * @param room Room name
     * @param nick Nick
     */
    void newClient(const QString &room, const QString &nick);
    /**
     * Client left a room
     * @param room Room name
     * @param nick Nick
     */
    void clientLeft(const QString &room, const QString &nick);
    /**
     * Message arrived
     * @param room Room name
     * @param sender Sender nick
     * @param msg Message string
     */
    void msgInRoom(const QString &room, const QString &sender, const QString &msg);
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
     * Successfully entered a room
     * @param name Room name
     * @param info Room clients and definitions
     */
    void enteredRoom(const QString &name, const Room &info);
    /**
     * Received server wide info after login
     * @param rooms List of room names
     * @param pkgs List of module packages
     */
    void receivedInitInfo(const QStringList &rooms, const QList<ModulePackage> &pkgs);
    /**
     * A room was deleted
     * @param name Room name
     */
    void deletedRoom(const QString &name);
    void disconnected(); //!< Client disconnected from server

private slots:
    void connected(); //!< Socket connected to server
    void readyRead(); //!< Data available to read from socket
    
public slots:
    /**
     * Enter a room
     * @param name Room name
     */
    void enterRoom(const QString &name);
    /**
     * Leave a room
     * @param name Room name
     */
    void leaveRoom(const QString &name) {oStream << static_cast<quint8>(1) << name; }
    /**
     * Send a message to a room
     * @param room Room name
     * @param msg Message string
     */
    void msgToRoom(const QString &room, const QString &msg) { oStream << static_cast<quint8>(2) << room << msg; }
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
     * Delete a room
     * @param name Room name
     */
    void deleteRoom(const QString &name) { oStream << static_cast<quint8>(5) << name; }
    void disconnect() { socket.disconnectFromHost(); } //!< Disconnect from server
};

#endif // JARVISCLIENT_H
