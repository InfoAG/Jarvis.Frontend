#ifndef MODULEPACKAGE_H
#define MODULEPACKAGE_H

#include <QString>
#include <QDataStream>
#include <QList>
#include <QMetaType>
#include "Module.h"
#include "BinaryOperatorModule.h"
#include "UnaryOperatorModule.h"
#include "FunctionModule.h"

/**
 * Package of server modules
 * @author Alexander Schlüter
 */
struct ModulePackage
{
    QString name;
    QList<Module> terminals; //!< List of terminal modules
    QList<BinaryOperatorModule> binaryOperators; //!< List of operator modules
    QList<UnaryOperatorModule> unaryOperators;
    QList<FunctionModule> functions; //!< List of function modules
};

/**
 * Extracts a module package from stream
 * @return A reference to the stream
 */
QDataStream &operator>>(QDataStream &stream, ModulePackage &modulePkg);

#endif // MODULEPACKAGE_H
