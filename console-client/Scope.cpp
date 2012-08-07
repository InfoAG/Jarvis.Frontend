#include "Scope.h"

QDataStream &operator>>(QDataStream &stream, Scope &scope)
{
    stream >> scope.clients;
    return stream;
}
