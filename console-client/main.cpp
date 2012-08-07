#include <QCoreApplication>
#include <QThread>
#include "TerminalPrinter.h"
#include "InputWorker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream qtout(stdout);
    QTextStream qtin(stdin);
    QString nick;
    QString server;
    qtout << "Server:\t";
    qtout.flush();
    qtin >> server;
    qtout << "Nick:\t";
    qtout.flush();
    qtin >> nick;
    JarvisClient client(server, 4200, nick, "supersecret");
    TerminalPrinter tp(client);
    QThread thread;
    Worker worker(client, tp);
    worker.moveToThread(&thread);
    thread.start();
    QMetaObject::invokeMethod(&worker, "doWork", Qt::QueuedConnection);
    return a.exec();
}
