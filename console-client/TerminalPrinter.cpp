#include "TerminalPrinter.h"

TerminalPrinter::TerminalPrinter(JarvisClient &client) : client(client), qtout(stdout)
{
    connect(&client, SIGNAL(msgInScope(const QString &, const QString &, const QString &)), this, SLOT(msgInScope(const QString &, const QString &, const QString &)));
    connect(&client, SIGNAL(newFunction(const QString &, const QString &)), this, SLOT(newFunction(const QString &, const QString &)));
    connect(&client, SIGNAL(newScope(const QString &)), this, SLOT(newScope(const QString &)));
    connect(&client, SIGNAL(newVariable(const QString &, const QString &)), this, SLOT(newVariable(const QString &, const QString &)));
    connect(&client, SIGNAL(newClient(const QString &, const QString &)), this, SLOT(newClient(const QString &, const QString &)));
    connect(&client, SIGNAL(clientLeft(const QString &, const QString &)), this, SLOT(clientLeft(const QString &, const QString &)));
    connect(&client, SIGNAL(error(JarvisClient::ClientError)), this, SLOT(error(JarvisClient::ClientError)));
    connect(&client, SIGNAL(receivedModules(QList<ModulePackage>)), this, SLOT(receivedModules(QList<ModulePackage>)));
}

void TerminalPrinter::newScope(const QString &name)
{
    qtout << "\nNew Scope:\t" << name << "\n";
    qtout.flush();
}

void TerminalPrinter::newFunction(const QString &scope, const QString &def)
{
    qtout << "\nNew function definition (scope " << scope << "):\t" << def << "\n";
    qtout.flush();
}

void TerminalPrinter::newVariable(const QString &scope, const QString &def)
{
    qtout << "\nNew variable definition (scope " << scope << "):\t" << def << "\n";
    qtout.flush();
}

void TerminalPrinter::newClient(const QString &scope, const QString &name)
{
    qtout << "\nNew client (scope " << scope << "):\t" << name << "\n";
    qtout.flush();
}

void TerminalPrinter::clientLeft(const QString &scope, const QString &name)
{
    qtout << "\nClient left (scope " << scope << "):\t" << name << "\n";
    qtout.flush();
}

void TerminalPrinter::msgInScope(const QString &scope, const QString &sender, const QString &msg)
{
    qtout << "\n" << scope << " << " << sender << ":\t" << msg << "\n";
    qtout.flush();
}

void TerminalPrinter::error(JarvisClient::ClientError error)
{
    qtout << "\nClient Error " << error << "\n";
    qtout.flush();
}

void TerminalPrinter::receivedModules(QList<ModulePackage> modulePkgs)
{
    qtout << "\nPackage Name\tModule Name\tModule Description\n";
    for (const auto &modpkg : modulePkgs) {
        qtout << modpkg.name << "\n";
        qtout << "\tTerminals:\n";
        for (const auto &mod : modpkg.terminals) {
            qtout << "\t\t" << mod.name << "\t" << mod.description << "\n";
        }
        qtout << "\tOperators:\n";
        for (const auto &mod : modpkg.operators) {
            qtout << "\t\t" << mod.name << "\t" << mod.description << "\n";
        }
        qtout << "\tFunctions:\n";
        for (const auto &mod : modpkg.functions) {
            qtout << "\t\t" << mod.name << "\t" << mod.description << "\n";
        }
    }
    qtout.flush();
}
