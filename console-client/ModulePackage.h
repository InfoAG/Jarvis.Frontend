#ifndef MODULEPACKAGE_H
#define MODULEPACKAGE_H

#include <QString>
#include <QDataStream>
#include <QList>
#include "Module.h"

struct ModulePackage
{
    QString name;
    QList<Module> terminals;
    QList<Module> operators;
    QList<Module> functions;
};

QDataStream &operator>>(QDataStream &stream, ModulePackage &modulePkg);

#endif // MODULEPACKAGE_H
