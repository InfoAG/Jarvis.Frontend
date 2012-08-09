#ifndef MODULE_H
#define MODULE_H

#include <QString>

/**
 * Server module
 * @author Alexander Schlüter
 */
struct Module
{
    QString name; //!< Name
    QString description; //!< Description
};

/**
 * Extracts a module from the stream
 * @return A reference to the stream
 */
QDataStream &operator>>(QDataStream &stream, Module &module);

#endif // MODULE_H
