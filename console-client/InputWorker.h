#ifndef INPUTWORKER_H
#define INPUTWORKER_H

#include "JarvisClient.h"

class Worker : public QObject
{
    Q_OBJECT

private:
    JarvisClient &client;

public:
    Worker(JarvisClient &client) : client(client) {};

public slots:
    void doWork() {
        QTextStream qtin(stdin);
        QTextStream qtout(stdout);
        QString input;
        QString currentScope;
        for (;;) {
            qtout << "(" << currentScope << ")->";
            qtout.flush();
            input = qtin.readLine();
            if (input.startsWith("/enter ")) QMetaObject::invokeMethod(&client, "enterScope", Q_ARG(QString, input.right(input.length() - 7)));
            else if (input.startsWith("/leave ")) QMetaObject::invokeMethod(&client, "leaveScope", Q_ARG(QString, input.right(input.length() - 7)));
            else if (input.startsWith("/open ")) currentScope = input.right(input.length() - 6);
            else QMetaObject::invokeMethod(&client, "msgToScope", Q_ARG(QString, currentScope), Q_ARG(QString, input));
        }
    }
};

#endif // INPUTWORKER_H
