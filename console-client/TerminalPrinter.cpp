#include "TerminalPrinter.h"

TerminalPrinter::TerminalPrinter(JarvisClient &client) : client(client)
{
    connect(&client, SIGNAL(msgInScope(const QString &, const QString &, const QString &)), this, SLOT(msgInScope(const QString &, const QString &, const QString &)));
    connect(&client, SIGNAL(newFunction(const QString &, const QString &)), this, SLOT(newFunction(const QString &, const QString &)));
    connect(&client, SIGNAL(newScope(const QString &)), this, SLOT(newScope(const QString &)));
    connect(&client, SIGNAL(newVariable(const QString &, const QString &)), this, SLOT(newVariable(const QString &, const QString &)));
    connect(&client, SIGNAL(newClient(const QString &, const QString &)), this, SLOT(newClient(const QString &, const QString &)));
    connect(&client, SIGNAL(clientLeft(const QString &, const QString &)), this, SLOT(clientLeft(const QString &, const QString &)));
    connect(&client, SIGNAL(error(JarvisClient::ClientError)), this, SLOT(error(JarvisClient::ClientError)));
}

void TerminalPrinter::newScope(const QString &name)
{
    std::cout << "New Scope:\t" << name.toStdString() << std::endl;
}

void TerminalPrinter::newFunction(const QString &scope, const QString &def)
{
    std::cout << "New function definition (scope " << scope.toStdString() << "):\t" << def.toStdString() << std::endl;
}

void TerminalPrinter::newVariable(const QString &scope, const QString &def)
{
    std::cout << "New variable definition (scope " << scope.toStdString() << "):\t" << def.toStdString() << std::endl;
}

void TerminalPrinter::newClient(const QString &scope, const QString &name)
{
    std::cout << "New client (scope " << scope.toStdString() << "):\t" << name.toStdString() << std::endl;
}

void TerminalPrinter::clientLeft(const QString &scope, const QString &name)
{
    std::cout << "Client left (scope " << scope.toStdString() << "):\t" << name.toStdString() << std::endl;
}

void TerminalPrinter::msgInScope(const QString &scope, const QString &sender, const QString &msg)
{
    std::cout << scope.toStdString() << " << " << sender.toStdString() << ":\t" << msg.toStdString() << std::endl;
}

void TerminalPrinter::error(JarvisClient::ClientError error)
{
    std::cout << "Client Error " << error << std::endl;
}
