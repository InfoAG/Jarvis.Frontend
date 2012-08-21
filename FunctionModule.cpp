#include "FunctionModule.h"

QDataStream &operator>>(QDataStream &stream, FunctionModule &module)
{
    quint8 isDynamic;
    stream >> module.name >> module.description >> isDynamic;
    if (isDynamic) module.matches = nullptr;
    else {
        module.matches = std::make_shared<QPair<QString, unsigned int>>();
        stream >> *module.matches;
    }
    stream >> isDynamic;
    if (isDynamic) module.priority.first = false;
    else {
        module.priority.first = true;
        stream >> module.priority.second;
    }
    return stream;
}
