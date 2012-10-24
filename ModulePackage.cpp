#include "ModulePackage.h"

QDataStream &operator>>(QDataStream &stream, ModulePackage &modulePkg)
{
    stream >> modulePkg.name >> modulePkg.terminals >> modulePkg.binaryOperators >> modulePkg.unaryOperators >> modulePkg.functions;
    return stream;
}
