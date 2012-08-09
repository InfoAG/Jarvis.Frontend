#ifndef SCOPE_H
#define SCOPE_H

#include <QList>
#include <QString>
#include <QDataStream>

/**
 * Contains all info about a scope
 */
struct Scope
{
    QList<QString> clients; //!< List of clients subscribed to this scope
    //TODO: definitions
};

/**
 * Extracts a scope from the stream
 * @return A reference to the stream
 */
QDataStream &operator>>(QDataStream &stream, Scope &scope);

#endif // SCOPE_H
