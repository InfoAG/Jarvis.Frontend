#ifndef FUNCTIONMODULE_H
#define FUNCTIONMODULE_H

#include "Module.h"
#include <QString>
#include <memory>
#include <QPair>
#include <QDataStream>

struct FunctionModule : public Module
{
    std::shared_ptr<QPair<QString, unsigned int>> matches;
    std::pair<bool, unsigned int> priority;
};

QDataStream &operator>>(QDataStream &stream, FunctionModule &module);

#endif // FUNCTIONMODULE_H
