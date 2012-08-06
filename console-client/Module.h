#ifndef MODULE_H
#define MODULE_H

#include <QString>

struct Module
{
    QString name;
    QString description;
};

QDataStream &operator>>(QDataStream &stream, Module &module);

#endif // MODULE_H
