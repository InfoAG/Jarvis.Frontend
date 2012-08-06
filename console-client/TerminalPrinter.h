#ifndef TERMINALPRINTER_H
#define TERMINALPRINTER_H

#include <QObject>
#include "JarvisClient.h"
//#include <iostream>
#include <QTextStream>

class TerminalPrinter : public QObject
{
    Q_OBJECT

private:
    JarvisClient &client;
    QTextStream qtout;

public:
    explicit TerminalPrinter(JarvisClient &client);
    
signals:
    
public slots:
    void newScope(const QString &name);
    void newFunction(const QString &scope, const QString &def);
    void newVariable(const QString &scope, const QString &def);
    void newClient(const QString &scope, const QString &name);
    void clientLeft(const QString &scope, const QString &name);
    void msgInScope(const QString &scope, const QString &sender, const QString &msg);
    void error(JarvisClient::ClientError error);
    void receivedModules(QList<ModulePackage> modulePkgs);
    
};

#endif // TERMINALPRINTER_H
