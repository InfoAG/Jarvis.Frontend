#ifndef FUNCTIONDEFINITION_H
#define FUNCTIONDEFINITION_H

#include <QStringList>

struct FunctionDefinition
{
    QString returnType, definition;
    QStringList argumentNames;

    FunctionDefinition() {}
    FunctionDefinition(QString returnType) : returnType(returnType) {}
};

inline QDataStream &operator>>(QDataStream &stream, FunctionDefinition &def)
{
    stream >> def.returnType >> def.argumentNames >> def.definition;
    return stream;
}

#endif // FUNCTIONDEFINITION_H
