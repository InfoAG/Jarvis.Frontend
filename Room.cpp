#include "Room.h"

QDataStream &operator>>(QDataStream &stream, Room &scope)
{
    stream >> scope.clients >> scope.variables >> scope.functions;
    return stream;
}
