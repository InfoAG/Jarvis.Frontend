#ifndef ROOM_H
#define ROOM_H

#include <QStringList>
#include <QString>
#include <QDataStream>
#include <QMap>
#include <QPair>
#include <QStringList>
#include "FunctionSignature.h"

/**
 * Contains all info about a scope
 */
struct Room
{
    QStringList clients; //!< List of clients subscribed to this scope
    QMap<QString, QPair<QString, QString>> variables; //!< maps variable identifier to pair of type string and definition string
    QMap<FunctionSignature, QPair<QStringList, QString>> functions; //!< maps functions signature to pair of argument names and definition string
};

/**
 * Extracts a scope from the stream
 * @return A reference to the stream
 */
QDataStream &operator>>(QDataStream &stream, Room &scope);

#endif // ROOM_H
