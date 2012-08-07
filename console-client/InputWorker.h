#ifndef INPUTWORKER_H
#define INPUTWORKER_H

#include "../JarvisClient.h"
#include "TerminalPrinter.h"

class Worker : public QObject
{
    Q_OBJECT

private:
    JarvisClient &client;
    TerminalPrinter &printer;

public:
    Worker(JarvisClient &client, TerminalPrinter &printer) : client(client), printer(printer) {};

public slots:
    void doWork() {
        QTextStream qtin(stdin);
        QString input;
        for (;;) {
            input = qtin.readLine();
            if (input.startsWith("/enter ")) QMetaObject::invokeMethod(&client, "enterScope", Q_ARG(QString, input.right(input.length() - 7)));
            else if (input.startsWith("/leave ")) QMetaObject::invokeMethod(&client, "leaveScope", Q_ARG(QString, input.right(input.length() - 7)));
            else if (input.startsWith("/open ")) QMetaObject::invokeMethod(&printer, "openScope", Q_ARG(QString, input.right(input.length() - 6)));
            else if (input == "/modules") QMetaObject::invokeMethod(&printer, "printModules");
            else if (input.startsWith("/unload ")) QMetaObject::invokeMethod(&client, "unloadPkg", Q_ARG(QString, input.right(input.length() - 8)));
            else if (input.startsWith("/load ")) QMetaObject::invokeMethod(&client, "loadPkg", Q_ARG(QString, input.right(input.length() - 6)));
            else if (input.startsWith("/delete ")) QMetaObject::invokeMethod(&client, "deleteScope", Q_ARG(QString, input.right(input.length() - 8)));
            else if (input == "/clients") QMetaObject::invokeMethod(&printer, "printClients");
            else if (input == "/scopes") QMetaObject::invokeMethod(&printer, "printScopes");
            else QMetaObject::invokeMethod(&printer, "msgToScope", Q_ARG(QString, input));
        }
    }
};

#endif // INPUTWORKER_H
