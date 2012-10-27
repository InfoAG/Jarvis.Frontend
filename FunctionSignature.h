#ifndef FUNCTIONSIGNATURE_H
#define FUNCTIONSIGNATURE_H

#include <QString>

struct FunctionSignature
{
    QString identifier;
    QStringList argumentTypes;

    bool operator<(const FunctionSignature &other) const { return identifier < other.identifier; }
};

inline QDataStream &operator>>(QDataStream &stream, FunctionSignature &sig)
{
    stream >> sig.identifier >> sig.argumentTypes;
    return stream;
}

#endif // FUNCTIONSIGNATURE_H
