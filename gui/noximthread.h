#ifndef NOXIMTHREAD_H
#define NOXIMTHREAD_H

#include <QObject>
#include <QThread>

class NoximThread : public QThread
{
    Q_OBJECT

protected:
    void run();
};

#endif // NOXIMTHREAD_H
