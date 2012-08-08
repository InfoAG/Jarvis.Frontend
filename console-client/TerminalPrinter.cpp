 #include "TerminalPrinter.h"

TerminalPrinter::TerminalPrinter(JarvisClient &client) : client(client), qtout(stdout)
{
    connect(&client, SIGNAL(msgInScope(const QString &, const QString &, const QString &)), SLOT(msgInScope(const QString &, const QString &, const QString &)));
    connect(&client, SIGNAL(newFunction(const QString &, const QString &)), SLOT(newFunction(const QString &, const QString &)));
    connect(&client, SIGNAL(newScope(const QString &)), SLOT(newScope(const QString &)));
    connect(&client, SIGNAL(deletedScope(const QString &)), SLOT(deletedScope(const QString &)));
    connect(&client, SIGNAL(newVariable(const QString &, const QString &)), SLOT(newVariable(const QString &, const QString &)));
    connect(&client, SIGNAL(newClient(const QString &, const QString &)), SLOT(newClient(const QString &, const QString &)));
    connect(&client, SIGNAL(clientLeft(const QString &, const QString &)), SLOT(clientLeft(const QString &, const QString &)));
    connect(&client, SIGNAL(error(JarvisClient::ClientError)), SLOT(error(JarvisClient::ClientError)));
    connect(&client, SIGNAL(pkgLoaded(const ModulePackage &)), SLOT(pkgLoaded(const ModulePackage &)));
    connect(&client, SIGNAL(pkgUnloaded(const QString &)), SLOT(pkgUnloaded(const QString &)));
    connect(&client, SIGNAL(enteredScope(const QString &, const Scope &)), SLOT(enteredScope(const QString &, const Scope &)));
    connect(&client, SIGNAL(receivedInitInfo(const QList<QString> &, const QList<ModulePackage> &)), SLOT(receivedInitInfo(const QList<QString> &, const QList<ModulePackage> &)));
    connect(&client, SIGNAL(disconnected()), SLOT(disconnected()));
}

void TerminalPrinter::newScope(const QString &name)
{
    qtout << "\nNew Scope:\t" << name << "\n";
    qtout << "(" << currentScope << ")->";
    qtout.flush();
    scopeByName.insert(name, Scope());
}

void TerminalPrinter::newFunction(const QString &scope, const QString &def)
{
    qtout << "\nNew function definition (scope " << scope << "):\t" << def << "\n";
    qtout << "(" << currentScope << ")->";
    qtout.flush();
}

void TerminalPrinter::newVariable(const QString &scope, const QString &def)
{
    qtout << "\nNew variable definition (scope " << scope << "):\t" << def << "\n";
    qtout << "(" << currentScope << ")->";
    qtout.flush();
}

void TerminalPrinter::newClient(const QString &scope, const QString &name)
{
    qtout << "\nNew client (scope " << scope << "):\t" << name << "\n";
    qtout << "(" << currentScope << ")->";
    qtout.flush();
    scopeByName[scope].clients.append(name);
}

void TerminalPrinter::clientLeft(const QString &scope, const QString &name)
{
    qtout << "\nClient left (scope " << scope << "):\t" << name << "\n";
    qtout << "(" << currentScope << ")->";
    qtout.flush();
    scopeByName[scope].clients.removeOne(name);
}

void TerminalPrinter::msgInScope(const QString &scope, const QString &sender, const QString &msg)
{
    qtout << "\n[" << scope << "] " << sender << ":\t" << msg << "\n";
    qtout << "(" << currentScope << ")->";
    qtout.flush();
}

void TerminalPrinter::error(JarvisClient::ClientError error)
{
    qtout << "\nClient Error " << error << "\n";
    qtout << "(" << currentScope << ")->";
    qtout.flush();
}

void TerminalPrinter::pkgLoaded(const ModulePackage &pkg)
{
    qtout << "\nPackage loaded:\n";
    printPackage(pkg);
    qtout << "(" << currentScope << ")->";
    qtout.flush();
    pkgs.append(pkg);
}

void TerminalPrinter::pkgUnloaded(const QString &name)
{
    qtout << "\nPackage unloaded: " << name << "\n";
    qtout << "(" << currentScope << ")->";
    qtout.flush();
    pkgs.erase(std::remove_if(pkgs.begin(), pkgs.end(), [&](const ModulePackage &pkg) { return pkg.name == name; }));
}

void TerminalPrinter::enteredScope(const QString &name, const Scope &info)
{
    qtout << "\nEntered scope " << name << "; Clients:\n";
    for (const auto &client : info.clients) qtout << client << "\t";
    qtout << "\n(" << currentScope << ")->";
    qtout.flush();
    scopeByName.insert(name, info);
}

void TerminalPrinter::receivedInitInfo(const QList<QString> &scopes, const QList<ModulePackage> &pkgs)
{
    qtout << "InitInfo:\n\n";
    qtout << "Scopes:\n";
    for (const auto &scope : scopes) {
        qtout << scope << "\t";
        scopeByName.insert(scope, Scope());
    }
    qtout << "Packages:\n";
    for (const auto &pkg : pkgs) {
        printPackage(pkg);
        this->pkgs = pkgs;
    }
    qtout << "(" << currentScope << ")->";
    qtout.flush();
}

void TerminalPrinter::printClients()
{
    for (const auto &client : scopeByName[currentScope].clients) qtout << client << "\t";
    qtout << "\n(" << currentScope << ")->";
    qtout.flush();
}

void TerminalPrinter::printScopes()
{
    for (const auto &scope : scopeByName.keys()) qtout << scope << "\t";
    qtout << "\n(" << currentScope << ")->";
    qtout.flush();
}

void TerminalPrinter::deletedScope(const QString &name)
{
    qtout << "Deleted scope " << name << "\n";
    qtout << "(" << currentScope << ")->";
    qtout.flush();
    scopeByName.remove(name);
}

void TerminalPrinter::printModules()
{
    for (const auto &pkg : pkgs) printPackage(pkg);
    qtout << "\n(" << currentScope << ")->";
    qtout.flush();
}

void TerminalPrinter::printPackage(const ModulePackage &pkg)
{
    qtout << "Package Name\tModule Name\tModule Description\n";
    qtout << pkg.name << "\n";
    qtout << "\tTerminals:\n";
    for (const auto &mod : pkg.terminals) {
        qtout << "\t\t" << mod.name << "\t" << mod.description << "\n";
    }
    qtout << "\tOperators:\n";
    for (const auto &mod : pkg.operators) {
        qtout << "\t\t" << mod.name << "\t" << mod.description << "\n";
    }
    qtout << "\tFunctions:\n";
    for (const auto &mod : pkg.functions) {
        qtout << "\t\t" << mod.name << "\t" << mod.description << "\n";
    }
    qtout.flush();
}
