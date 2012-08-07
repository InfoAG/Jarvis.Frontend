#ifndef SCOPE_H
#define SCOPE_H

#include <QList>
#include <QString>
#include <QDataStream>

struct Scope
{
    QList<QString> clients;
};

QDataStream &operator>>(QDataStream &stream, Scope &scope);

#endif // SCOPE_H
