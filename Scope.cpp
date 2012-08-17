#include "Scope.h"

QDataStream &operator>>(QDataStream &stream, Scope &scope)
{
    stream >> scope.clients >> scope.variables >> scope.functions;
    return stream;
}
