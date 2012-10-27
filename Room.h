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
    QMap<QString, QPair<QString, QString>> variables;
    QMap<FunctionSignature, QPair<QStringList, QString>> functions;
};

/**
 * Extracts a scope from the stream
 * @return A reference to the stream
 */
QDataStream &operator>>(QDataStream &stream, Room &scope);

#endif // ROOM_H
