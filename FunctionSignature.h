#ifndef FUNCTIONSIGNATURE_H
#define FUNCTIONSIGNATURE_H

#include <QString>

struct FunctionSignature
{
    QString identifier;
    QStringList argumentTypes;

    bool operator<(const FunctionSignature &other) const { if (identifier == other.identifier) return std::lexicographical_compare(argumentTypes.begin(), argumentTypes.end(), other.argumentTypes.begin(), other.argumentTypes.end()); else return identifier < other.identifier; }
};

inline QDataStream &operator>>(QDataStream &stream, FunctionSignature &sig)
{
    stream >> sig.identifier >> sig.argumentTypes;
    return stream;
}

#endif // FUNCTIONSIGNATURE_H
