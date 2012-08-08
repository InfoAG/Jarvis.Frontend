#include "ModulePackage.h"

QDataStream &operator>>(QDataStream &stream, ModulePackage &modulePkg)
{
    stream >> modulePkg.name >> modulePkg.terminals >> modulePkg.operators >> modulePkg.functions;
    return stream;
}
