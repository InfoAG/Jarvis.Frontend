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
    qtout << "Choose nick:\t";
    qtout.flush();
    qtin >> nick;
    JarvisClient client("localhost", 4200, nick, "supersecret");
    TerminalPrinter tp(client);
    QThread thread;
    Worker worker(client);
    worker.moveToThread(&thread);
    thread.start();
    QMetaObject::invokeMethod(&worker, "doWork", Qt::QueuedConnection);
    return a.exec();
}
