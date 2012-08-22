#ifndef SCOPE_H
#define SCOPE_H

#include <QList>
#include <QString>
#include <QDataStream>
#include <QMap>
#include <QMetaType>
#include <QPair>
#include <QStringList>

/**
 * Contains all info about a scope
 */
struct Scope
{
    QList<QString> clients; //!< List of clients subscribed to this scope
    QMap<QString, QString> variables;
    QMap<QString, QPair<QStringList, QString>> functions;
};

/**
 * Extracts a scope from the stream
 * @return A reference to the stream
 */
QDataStream &operator>>(QDataStream &stream, Scope &scope);

#endif // SCOPE_H
