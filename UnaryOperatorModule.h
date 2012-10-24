#ifndef UNARYOPERATORMODULE_H
#define UNARYOPERATORMODULE_H

#include "OperatorModule.h"

struct UnaryOperatorModule : public OperatorModule
{
    enum Alignment { PRE, POST, DYNAMIC };
    Alignment alignment;
};

QDataStream &operator>>(QDataStream &stream, UnaryOperatorModule &module);

#endif // UNARYOPERATORMODULE_H
