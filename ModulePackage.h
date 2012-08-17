#ifndef MODULEPACKAGE_H
#define MODULEPACKAGE_H

#include <QString>
#include <QDataStream>
#include <QList>
#include <QMetaType>
#include "Module.h"

/**
 * Package of server modules
 * @author Alexander Schlüter
 */
struct ModulePackage
{
    QString name;
    QList<Module> terminals; //!< List of terminal modules
    QList<Module> operators; //!< List of operator modules
    QList<Module> functions; //!< List of function modules
};

Q_DECLARE_METATYPE(QList<ModulePackage>)
Q_DECLARE_METATYPE(ModulePackage)

/**
 * Extracts a module package from stream
 * @return A reference to the stream
 */
QDataStream &operator>>(QDataStream &stream, ModulePackage &modulePkg);

#endif // MODULEPACKAGE_H
