#include "OperatorModule.h"

QDataStream &operator>>(QDataStream &stream, OperatorModule &module)
{
    quint8 byteBuf;
    stream >> module.name >> module.description >> byteBuf;
    if (byteBuf) module.matches = nullptr;
    else {
        module.matches = std::make_shared<QString>();
        stream >> *module.matches;
    }
    stream >> byteBuf;
    if (byteBuf) module.priority.first = false;
    else {
        module.priority.first = true;
        stream >> module.priority.second;
    }
    /*
    stream >> byteBuf;
    if (byteBuf) module.associativity.first = false;
    else {
        module.associativity.first = true;
        stream >> byteBuf;
        module.associativity.second = static_cast<OperatorModule::AssociativityType>(byteBuf);
    }
    */
    stream >> byteBuf;
    module.needsParseForMatch = byteBuf;
    return stream;
}
