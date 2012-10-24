#include "BinaryOperatorModule.h"

QDataStream &operator>>(QDataStream &stream, BinaryOperatorModule &module)
{
    stream >> static_cast<OperatorModule&>(module);
    quint8 byteBuf;
    stream >> byteBuf;
    module.associativity = static_cast<BinaryOperatorModule::AssociativityType>(byteBuf);
    return stream;
}
