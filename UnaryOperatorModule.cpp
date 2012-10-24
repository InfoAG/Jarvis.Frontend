#include "UnaryOperatorModule.h"

QDataStream &operator>>(QDataStream &stream, UnaryOperatorModule &module)
{
    stream >> static_cast<OperatorModule&>(module);
    quint8 byteBuf;
    stream >> byteBuf;
    module.alignment = static_cast<UnaryOperatorModule::Alignment>(byteBuf);
    return stream;
}
