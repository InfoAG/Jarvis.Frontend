#ifndef OPERATORMODULE_H
#define OPERATORMODULE_H

#include "Module.h"
#include <QString>
#include <memory>
#include <QDataStream>

struct OperatorModule : public Module
{
    std::shared_ptr<QString> matches;
    std::pair<bool, unsigned int> priority;
    bool needsParseForMatch;
};

QDataStream &operator>>(QDataStream &stream, OperatorModule &module);

#endif // OPERATORMODULE_H
