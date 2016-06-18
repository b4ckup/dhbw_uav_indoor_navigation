#ifndef CLISTENERTHREAD_H
#define CLISTENERTHREAD_H

#include <QThread>
#include "clistener.h"

using namespace std;

class cListenerThread : public QThread
{
private:
    cListener *listener;
    QObject *connectObj;
public:
    cListenerThread(QObject *connectObj, QObject *parent = NULL);
    ~cListenerThread();

    cListener *getListener();

    void run();
};

#endif // CLISTENERTHREAD_H
