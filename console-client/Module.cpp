#include "Module.h"

QDataStream &operator>>(QDataStream &stream, Module &module)
{
    stream >> module.name >> module.description;
    return stream;
}
