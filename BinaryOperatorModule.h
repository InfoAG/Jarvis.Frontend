#ifndef BINARYOPERATORMODULE_H
#define BINARYOPERATORMODULE_H

#include "OperatorModule.h"

struct BinaryOperatorModule : public OperatorModule
{
    enum AssociativityType {
        LEFT,
        RIGHT,
        DYNAMIC
    };

    AssociativityType associativity;
};

QDataStream &operator>>(QDataStream &stream, BinaryOperatorModule &module);

#endif // BINARYOPERATORMODULE_H
