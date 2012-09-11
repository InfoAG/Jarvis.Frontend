#ifndef OPERATORMODULE_H
#define OPERATORMODULE_H

#include "Module.h"
#include <QString>
#include <memory>
#include <QDataStream>

struct OperatorModule : public Module
{
    enum AssociativityType {
        LEFT,
        RIGHT
    };

    std::shared_ptr<QString> matches;
    std::pair<bool, unsigned int> priority;
    std::pair<bool, AssociativityType> associativity;
    bool needsParseForMatch;
};

QDataStream &operator>>(QDataStream &stream, OperatorModule &module);

#endif // OPERATORMODULE_H
